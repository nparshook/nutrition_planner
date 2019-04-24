#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace np {
namespace database {

class DatabaseManager::Implementation
{
public:
    Implementation(DatabaseManager* _dbController, const QString& dbName, const QString& dbConn)
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

    DatabaseManager* dbController{nullptr};
    QSqlDatabase db;
    bool ready = false;
};

DatabaseManager::DatabaseManager(QObject* parent, const QString& databasename, const QString& dbConnName)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, databasename, dbConnName));
}

DatabaseManager::~DatabaseManager() {}

bool DatabaseManager::isReady() {
    return implementation->ready;
}

QSqlQuery* DatabaseManager::createPreparedQuery(const QString &queryString)
{
    QSqlQuery* query = new QSqlQuery(implementation->db);
    query->prepare(queryString);
    return query;
}

bool DatabaseManager::hasTable(const QString &tableName) {
    return implementation->db.tables().contains(tableName);
}

}}
