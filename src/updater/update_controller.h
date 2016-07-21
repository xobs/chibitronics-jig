#include <QObject>
#include <QThread>
#include <QTimer>

class NetworkUpdater;
class DiskUpdater;

class UpdateController : public QObject
{
    Q_OBJECT

    QTimer networkCheckTimer;
    QTimer diskCheckTimer;
    QThread checkThread;

    NetworkUpdater *networkUpdater;
    DiskUpdater *diskUpdater;

public:
    UpdateController();
    ~UpdateController();

public slots:
    void startChecking(unsigned int networkCheckTime, unsigned int diskCheckTime);

signals:
    void updateAvailable(const QVariant &);
};
