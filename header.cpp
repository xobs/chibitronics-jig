#include "header.h"
#include <QString>

Header::Header(QString m)
    : message(m)
{
    name = QString("Message: ") + m;
}

void Header::runTest()
{
    emit testMessage(testName(), setHeader, 0, message);
}
