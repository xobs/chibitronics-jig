#ifndef CHIBITESTREGISTRY_H
#define CHIBITESTREGISTRY_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include "testmodule.h"

class ChibiTestRegistry : public QObject
{
    Q_OBJECT

private:
    QHash<QString, QVariant> registry;

public:
    ChibiTestRegistry();
    bool addModule(const struct test_module *module);
    const TestModule *getModule(const QString &name);
};

#endif // CHIBITESTREGISTRY_H
