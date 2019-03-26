#ifndef FOODITEMLIST_H
#define FOODITEMLIST_H

#include <QObject>
#include <QScopedPointer>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "fooditem.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodItemList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<np::models::FoodItem> foodItems READ foodItems NOTIFY foodItemsChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodEqChanged)

public:
    FoodItemList(QObject *parent = nullptr);
    ~FoodItemList();
    QQmlListProperty<FoodItem> foodItems();
    Q_INVOKABLE void appendFood(FoodItem *foodItem);
    Q_INVOKABLE void deleteFood(int index);

    QString name() const;
    void setName(const QString &name);

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();

signals:
    void foodItemsChanged();
    void nameChanged();
    void foodEqChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODITEMLIST_H
