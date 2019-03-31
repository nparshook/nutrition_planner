#include "databasecontroller.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace np {
namespace controllers {

class DatabaseController::Implementation
{
public:
    Implementation(DatabaseController* _dbController, const QString& dbName, const QString& dbConn)
        : dbController(_dbController)
    {
        initialize(dbName, dbConn);
    }

    ~Implementation() {
        db.close();
    }

    void initialize(const QString& dbName, const QString& dbConn) {
        qDebug() << dbName;
        db = QSqlDatabase::addDatabase("QSQLITE", dbConn);
        db.setDatabaseName(dbName);
        if (!db.open()) {
            ready = false;
            return;
        }
        ready = true;
    }

    DatabaseController* dbController{nullptr};
    QSqlDatabase db;
    bool ready = false;
};

DatabaseController::DatabaseController(QObject* parent, const QString& databasename, const QString& dbConnName)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, databasename, dbConnName));
}

DatabaseController::~DatabaseController() {}

bool DatabaseController::isReady() {
    return implementation->ready;
}

QSqlQuery* DatabaseController::createPreparedQuery(const QString &queryString)
{
    QSqlQuery* query = new QSqlQuery(implementation->db);
    query->prepare(queryString);
    return query;
}

bool DatabaseController::hasTable(const QString &tableName) {
    return implementation->db.tables().contains(tableName);
}

}}
