#ifndef HEADER_H
#define HEADER_H
#include "chibitest.h"

class Header : public ChibiTest
{
	Q_OBJECT
public:
    Header(QString message);
    void runTest();

private:
    QString message;
};

#endif // HEADER_H
