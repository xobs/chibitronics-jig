#include "testaudio.h"
#include <QString>

TestAudio::TestAudio(int e)
{
    name = "Test audio sensor";
    equalizationMsecs = e;
}

void TestAudio::runTest() {
    int passed;
    int passTry;

    selectSticker(6);

    /* Not sure what this does */
    setGpio(spiResetGpio, 1);
    setGpio(tpiSignalGpio, 1);

    /* Let the audio level equalize */
    testInfo("Equalizing silence level...");
    msleep(equalizationMsecs);

    /* Pre-measure to make sure it's off */
    testInfo("Trying to listen for buzzer");
    if (getGpio(outputGpio)) {
        testError("Room too noisy for silence test");
        return;
    }

    /* Measure audio level */
    passed = 0;
    for (passTry = 0; passTry < 3 && !passed; passTry++) {
        setGpio(buzzerGpio, 1);
        msleep(500);

        passed = getGpio(outputGpio);
        setGpio(buzzerGpio, 0);

        if (!passed) {
            testInfo("Buzzer not heard");
            if (passTry < 2)
                msleep(equalizationMsecs);
        }
    }

    /* Tristate the programming outputs */
    setGpio(tpiSignalGpio, 0);

    if (passed)
        emit testMessage(testName(), testPass, 6, "Test passed");
    else
        testError("Buzzer sticker not working");
}
