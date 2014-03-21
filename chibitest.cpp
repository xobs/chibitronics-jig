#include "chibitest.h"

ChibiTest::ChibiTest()
    : lastString("")
{
    testNumber = -1;
    lastResult = 0;
    engine = NULL;
}

const QString &ChibiTest::getStatusString()
{
    return lastString;
}

int ChibiTest::getStatusValue()
{
    return lastResult;
}

void ChibiTest::setTestNumber(int number)
{
    testNumber = number;
}

void ChibiTest::setEngine(ChibiTestEngine *engie)
{
    engine = engie;
}

const QString ChibiTest::testName()
{
    return name;
}

void ChibiTest::testInfo(const QString string)
{
    emit testMessage(testName(), infoMessage, 0, string);
}

void ChibiTest::testError(const QString string)
{
    emit testMessage(testName(), errorMessage, 0, string);
}

void ChibiTest::testDebug(const QString string)
{
    emit testMessage(testName(), debugMessage, 0, string);
}
