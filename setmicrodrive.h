#ifndef SETMICRODRIVE_H
#define SETMICRODRIVE_H
#include "chibitest.h"

class SetMicroDrive : public ChibiTest
{
public:
    enum microDrive {
        program,
        execute,
    };
    SetMicroDrive(enum microDrive);
    void runTest();

private:
    enum microDrive microDrive;
};

#endif // SETMICRODRIVE_H
