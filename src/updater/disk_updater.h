#include <QObject>
#include <QProcess>

class DiskUpdater : public QObject
{
    Q_OBJECT

public:
    DiskUpdater();

public slots:
    void doCheck();
    void updaterTerminated(int result);

signals:
    void updateAvailable(const QVariant & description);
    void updateStart(const QVariant & progress);
    void updateProgress(const QVariant & progress);
    void updateFinished(const QVariant & message);
    void updateFailed(const QVariant & message);

private:
    QProcess *updateProcess;
};
