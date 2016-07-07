#ifndef CHIBITESTREGISTRY_H
#define CHIBITESTREGISTRY_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QDir>
#include "testmodule.h"

class ChibiTestRegistry : public QObject
{
    Q_OBJECT

private:
    QHash<QString, QVariant> registry;

public:
    ChibiTestRegistry();
    bool addDirectory(const QDir & dir);
    bool addModule(const void *module);
    const QStringList moduleNames();
    const TestModule *getModule(const QString &name);
};

#endif // CHIBITESTREGISTRY_H
