#include "header.h"
#include <QString>

Header::Header(const QHash<QString, QVariant> &settings)
    : ChibiTest(settings), message(settings["message"].toString())
{
    name = QString("Message: ") + message;
}

void Header::runTest()
{
    emit testMessage(testName(), setHeader, 0, message);
}
