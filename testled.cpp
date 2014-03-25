#include <QDebug>
#include <QString>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "testled.h"

#define FPGA_I2C_ADDR  0x1e

static int i2cfd;

static int i2c_open(void)
{
    int slave_address = FPGA_I2C_ADDR;
    i2cfd = open("/dev/i2c-2", O_RDWR);
    if (i2cfd == -1 ) {
        qDebug() << "Unable to open /dev/i2c-2";
        i2cfd = 0;
        return -1;
    }
    if (ioctl(i2cfd, I2C_SLAVE, slave_address) < 0) {
        qDebug() << "Unable to set I2C slave device";
        close(i2cfd);
        return -1;
    }
    return 0;
}

int i2c_write(unsigned char addr, unsigned char data)
{
    char i2cbuf[2];
    struct i2c_msg msg[2];
    int slave_address = FPGA_I2C_ADDR;

    struct i2c_ioctl_rdwr_data {
        struct i2c_msg *msgs;  /* ptr to array of simple messages */
        int nmsgs;             /* number of messages to exchange */
    } msgst;

    if (!i2cfd)
        i2c_open();

    i2cbuf[0] = addr;
    i2cbuf[1] = data;
    // set address for read
    msg[0].addr  = slave_address;
    msg[0].flags = 0; // no flag means do a write
    msg[0].len   = 2;
    msg[0].buf   = i2cbuf;

    msgst.msgs  = msg;
    msgst.nmsgs = 1;

    if (ioctl(i2cfd, I2C_RDWR, &msgst) < 0) {
        qDebug() << "Transaction failed";
        return -1;
    }

    return 0;
}


static int i2c_read(unsigned char addr, unsigned char *data)
{
    int slave_address = -1;
    char i2cbuf[2];

    struct i2c_msg msg[2];

    struct i2c_ioctl_rdwr_data {
        struct i2c_msg *msgs;  /* ptr to array of simple messages */
        int nmsgs;             /* number of messages to exchange */
    } msgst;

    slave_address = FPGA_I2C_ADDR;

    if (!i2cfd)
        i2c_open();

    i2cbuf[0] = addr;
    // set address for read
    msg[0].addr  = slave_address;
    msg[0].flags = 0; // no flag means do a write
    msg[0].len   = 1;
    msg[0].buf   = i2cbuf;

    // set readback buffer
    msg[1].addr  = slave_address;
    msg[1].flags = I2C_M_NOSTART | I2C_M_RD;
    msg[1].len   = 1;
    msg[1].buf   = (char *) data;

    msgst.msgs  = msg;
    msgst.nmsgs = 2;

    if (ioctl(i2cfd, I2C_RDWR, &msgst) < 0) {
        perror("Transaction failed\n");
        return -1;
    }

    return 0;
}

static unsigned int read_adc(char channel)
{
    unsigned char d = 0;
    unsigned int retval = 0;

    i2c_write(0x2, (channel & 0x7));
    i2c_write(0x2, (channel & 0x7) | 0x8);

    do {
        i2c_read(0x42, &d);
    } while ((d & 0x1) == 0);

    i2c_write(0x2, (channel & 0x7));
    i2c_write(0x2, (channel & 0x7) | 0x8);

    do {
        i2c_read(0x42, &d);
    } while ((d & 0x1) == 0);

    i2c_read(0x40, &d);
    retval = d & 0xff;
    i2c_read(0x41, &d);
    retval |= ((d & 0xff) << 8);

    return retval;
}


TestLed::TestLed(int t)
{
    name = "Test LED sensor";
    threshold = t;
}

void TestLed::runTest() {
    int startLevel;
    int endLevel;

    selectSticker(7);

    /* Not sure what this does */
    setGpio(tpiSignalGpio, 1);
    setGpio(spiResetGpio, 1);

    /* Turn off LED and let the light level equalize */
    setGpio(ledGpio, 0);
    msleep(1000);

    startLevel = read_adc(0);
    testInfo(QString("LED start intensity: ") + QString::number(startLevel));

    /* Turn on LED and let the light level equalize */
    setGpio(ledGpio, 1);
    msleep(1000);

    endLevel = read_adc(0);

    testInfo(QString("LED end intensity: ") + QString::number(endLevel));

    setGpio(ledGpio, 0);
    if ((endLevel - startLevel) < threshold)
        testError("LED sensor did not detect test light");
    else
        emit testMessage(testName(), testPass, 7, "Test passed");
}
