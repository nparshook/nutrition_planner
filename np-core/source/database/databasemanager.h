#ifndef FOODDATABASECONTROLLER_H
#define FOODDATABASECONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QDir>

#include "../np-core_global.h"

class QSqlQuery;

namespace np {
namespace database {

class NPCORESHARED_EXPORT DatabaseManager : public QObject
{
    Q_OBJECT

public:
    DatabaseManager(QObject* parent = nullptr, const QString& databaseName = QDir::currentPath() + "dbName.db", const QString& dbConnName = "conn");
    ~DatabaseManager();
    bool isReady();
    bool hasTable(const QString & tableName);
    QSqlQuery* createPreparedQuery(const QString& queryString);

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODDATABASECONTROLLER_H
