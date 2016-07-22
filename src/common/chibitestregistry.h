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

    // A list of variables constant throughout a test sequence.
    QMap<QVariant, QVariant> variables;

public:
    ChibiTestRegistry();
    bool addDirectory(const QDir & dir);
    bool addModule(const void *module);
    const QStringList moduleNames();
    const TestModule *getModule(const QString &name);

    const QVariant & getVariable(const QVariant & key);
    void setVariable(const QVariant & key, const QVariant & value);
    void resetVariables();  // Call when the test sequence starts again

signals:
    void testMessage(const QString & name, int testMessageType,
                     const QVariant & message, const QVariant & param);
};

#endif // CHIBITESTREGISTRY_H
