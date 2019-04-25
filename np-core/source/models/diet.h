#ifndef DIET_H
#define DIET_H

#include <QObject>
#include <QScopedPointer>
#include <QQmlListProperty>
#include <QList>

#include "../np-core_global.h"
#include "fooditem.h"
#include "foodsearch.h"
#include "day.h"
#include "../database/databasemanager.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT Diet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodAvgChanged)
    Q_PROPERTY(QQmlListProperty<np::models::Day> ui_days READ days NOTIFY daysChanged)

public:
    Diet(QObject *parent = nullptr);
    Diet(np::database::DatabaseManager *manager, FoodSearch* searcher, QObject *parent = nullptr);
    Diet(int dbID, np::database::DatabaseManager *manager, FoodSearch* searcher, QObject *parent = nullptr);
    ~Diet();

    QString name() const;
    void setName(const QString &name);

    void setKey(int key);
    int key();

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();

    QQmlListProperty<np::models::Day> days();
    Q_INVOKABLE QVariant newDay();

signals:
    void nameChanged();
    void foodEqChanged();
    void foodAvgChanged();
    void daysChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // DIET_H
