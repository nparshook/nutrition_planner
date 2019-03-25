#include "databasecontroller.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace np {
namespace controllers {

class DatabaseController::Implementation
{
public:
    Implementation(DatabaseController* _dbController, const QString& dbName)
        : dbController(_dbController)
    {
        initialize(dbName);
    }

    ~Implementation() {
        db.close();
    }

    void initialize(const QString& dbName) {
        qDebug() << dbName;
        db = QSqlDatabase::addDatabase("QSQLITE");
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

DatabaseController::DatabaseController(QObject* parent, const QString& databasename)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, databasename));
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

}}
