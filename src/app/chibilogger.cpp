#include "chibilogger.h"

ChibiLogger::ChibiLogger(const QString &db_path) :
    dbPath(db_path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

void ChibiLogger::addMessage(const QString &message) {
    ;
}
