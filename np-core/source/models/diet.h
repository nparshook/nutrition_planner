#ifndef DIET_H
#define DIET_H

#include <QObject>
#include <QScopedPointer>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "fooditem.h"
#include "../database/databasemanager.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT Diet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodAvgChanged)

public:
    Diet(QObject *parent = nullptr);
    Diet(np::database::DatabaseManager *manager, QObject *parent = nullptr);
    Diet(int dbID, np::database::DatabaseManager *manager, QObject *parent = nullptr);
    ~Diet();

    QString name() const;
    void setName(const QString &name);

    void setKey(int key);
    int key();

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();

signals:
    void nameChanged();
    void foodEqChanged();
    void foodAvgChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // DIET_H
