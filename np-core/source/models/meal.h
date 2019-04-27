#ifndef MEAL_H
#define MEAL_H

#include <QObject>
#include <QScopedPointer>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "fooditem.h"
#include "foodsearch.h"
#include "../database/databasemanager.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT Meal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodAvgChanged)
    Q_PROPERTY(QQmlListProperty<np::models::FoodItem> ui_foods READ foods NOTIFY foodsChanged)


public:
    Meal(QObject *parent = nullptr);
    Meal(int dietID, np::database::DatabaseManager *manager, FoodSearch* searcher, bool isNew = true, QObject *parent = nullptr);
    ~Meal();

    QString name() const;
    void setName(const QString &name);

    void setKey(int key);
    int key();

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();

    QQmlListProperty<np::models::FoodItem> foods();
    Q_INVOKABLE void appendFood(FoodItem* item);
    Q_INVOKABLE void removeFood(FoodItem* item);
    void remove();
    void load();
signals:
    void nameChanged();
    void foodsChanged();
    void foodEqChanged();
    void foodAvgChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}


#endif // MEAL_H
