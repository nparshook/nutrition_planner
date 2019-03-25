#ifndef FOODDATABASECONTROLLER_H
#define FOODDATABASECONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QDir>

#include "../np-core_global.h"

class QSqlQuery;

namespace np {
namespace controllers {

class NPCORESHARED_EXPORT DatabaseController : public QObject
{
    Q_OBJECT

public:
    DatabaseController(QObject* parent = nullptr, const QString& databaseName = QDir::currentPath() + "/usda.db");
    ~DatabaseController();
    bool isReady();
    QSqlQuery* createPreparedQuery(const QString& queryString);
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODDATABASECONTROLLER_H
