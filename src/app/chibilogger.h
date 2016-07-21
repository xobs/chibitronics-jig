#include <QObject>
#include <QString>
#include <QSqlDatabase>

class ChibiLogger : public QObject {
    Q_OBJECT

public:
    ChibiLogger(const QString & db_path);

public slots:
    void addMessage(const QString & message);

private:
    const QString dbPath;
    QSqlDatabase db;
};
