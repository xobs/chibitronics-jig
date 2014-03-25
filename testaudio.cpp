#include "testaudio.h"
#include <QString>

TestAudio::TestAudio(void)
{
    name = "Test audio sensor";
}

void TestAudio::runTest() {
    int passed;
    int passTry;

    /* Let the audio level equalize */
    msleep(3 * 1000);

    /* Measure audio level */
    testInfo("Trying to listen for buzzer");
    passed = 0;
    for (passTry = 0; passTry < 3 && !passed; passTry++) {
        setGpio(buzzerGpio, 1);
        msleep(500);

        passed = getGpio(outputGpio);
        setGpio(buzzerGpio, 0);

        if (!passed) {
            testInfo("Buzzer not heard");
            msleep(500);
        }
    }

    if (!passed)
        testError("Buzzer sticker not working");
}
