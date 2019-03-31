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
    Q_PROPERTY(QQmlListProperty<np::models::FoodItemList> ui_subFoodLists READ subFoodLists NOTIFY subFoodListsChanged)
    Q_PROPERTY(QString ui_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodAvgChanged)
    Q_PROPERTY(float ui_gmWgt READ gmWgt NOTIFY gmWgtChanged)
    Q_PROPERTY(float ui_amount READ amount WRITE setAmount NOTIFY amountChanged)

public:
    FoodItemList(QObject *parent = nullptr);
    ~FoodItemList();
    QQmlListProperty<FoodItem> foodItems();
    QQmlListProperty<FoodItemList> subFoodLists();
    Q_INVOKABLE void appendFood(FoodItem *foodItem);
    Q_INVOKABLE void deleteFood(int index);
    Q_INVOKABLE void addSubFoodList();
    Q_INVOKABLE void deleteSubFoodList(int index);

    QString name() const;
    void setName(const QString &name);

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();
    float gmWgt();
    float amount();
    void setAmount(float amount);

    void setKey(int key);
    int key();

public slots:
    void recalculate();

signals:
    void foodItemsChanged();
    void subFoodListsChanged();
    void nameChanged();
    void foodEqChanged();
    void foodAvgChanged();
    void gmWgtChanged();
    void amountChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODITEMLIST_H
