#include "fooditemlist.h"
#include <QDebug>
#include <QList>

namespace np {
namespace models {

class FoodItemList::Implementation {
public:
    Implementation(FoodItemList *_foodItemList)
        : foodItemList(_foodItemList)
    {
        name = "New FoodItem List...";
        foodID = new FoodID("0", name, name, foodItemList);
        createFoodEqs();
    }

    void createFoodEqs()
    {
        QHash<int, FoodNutr *> nutrsTable;

        for(int i = 0; i < foodItems.size(); i++) {
            qDebug() << foodItems.size();
            qDebug() << i;
            qDebug() << foodItems.isEmpty();
            qDebug() << foodItems[i];
            QList<FoodNutr *> nutrs = foodItems[i]->nutrients();
            float scaleFactor = foodItems[i]->scaleFactor;
            float amount = foodItems[i]->amount;
            for(int j = 0; j < nutrs.size(); j++) {
                FoodNutr *nutr = nutrs[j];
                if(!nutrsTable.contains(nutr->nutrNo()))
                {
                   nutrsTable[nutr->nutrNo()] = new FoodNutr(nutr->nutrNo(), nutr->nutrVal() * scaleFactor * amount / 100, nutr->nutrDesc(), nutr->tagName(), nutr->units());
                } else {
                    nutrsTable[nutr->nutrNo()]->setNutrVal(nutr->nutrVal() * scaleFactor * amount / 100 + nutrsTable[nutr->nutrNo()]->nutrVal());
                }
            }
        }

        FoodItem* totalItem = new FoodItem(foodItemList);
        FoodItem* avgItem = new FoodItem(foodItemList);
        totalItem->setFoodID(foodID);
        avgItem->setFoodID(foodID);
        QHashIterator<int, FoodNutr *> h(nutrsTable);
        while (h.hasNext()) {
            h.next();
            FoodNutr *nutr = h.value();
            totalItem->appendNutrient(nutr);
            avgItem->appendNutrient(new FoodNutr(nutr->nutrNo(), nutr->nutrVal() / foodItems.size(), nutr->nutrDesc(), nutr->tagName(), nutr->units()));
        }

        foodTotalEq = totalItem;
        foodAvgEq = avgItem;
    }

    bool setName(const QString &_name) {
        if(name == _name) return false;
        name = _name;
        foodID->setLongDesc(name);
        foodID->setShrtDesc(name);
        return true;
    }

    void addSubFoodList() {
        FoodItemList *subList = new FoodItemList(foodItemList);
        subFoodLists.append(subList);
        createFoodEqs();
    }

    QString name;
    QList<FoodItem *> foodItems;
    QList<FoodItemList *> subFoodLists;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodItemList* foodItemList{nullptr};
    FoodID *foodID{nullptr};
};

FoodItemList::FoodItemList(QObject *parent) : QObject(parent)
{
    implementation.reset(new Implementation(this));
}

FoodItemList::~FoodItemList() {}

QQmlListProperty<FoodItem> FoodItemList::foodItems()
{
    return QQmlListProperty<FoodItem>(this, implementation->foodItems);
}

void FoodItemList::appendFood(FoodItem *foodItem)
{
    implementation->foodItems.append(foodItem);
    implementation->createFoodEqs();
    emit foodItemsChanged();
}

void FoodItemList::deleteFood(int index)
{
    implementation->foodItems.removeAt(index);
    implementation->createFoodEqs();
    emit foodItemsChanged();
}

QString FoodItemList::name() const
{
    return implementation->name;
}

FoodItem* FoodItemList::foodAvgEq()
{
    return implementation->foodAvgEq;
}

FoodItem* FoodItemList::foodTotalEq()
{
    return implementation->foodTotalEq;
}

void FoodItemList::setName(const QString &name)
{
    if(implementation->setName(name)) emit nameChanged();
}

void FoodItemList::deleteSubFoodList(int index) {
    implementation->subFoodLists.removeAt(index);
    implementation->createFoodEqs();
    emit subFoodListsChanged();
}

void FoodItemList::addSubFoodList()
{
    implementation->addSubFoodList();
    emit subFoodListsChanged();
}

QQmlListProperty<FoodItemList> FoodItemList::subFoodLists()
{
    return QQmlListProperty<FoodItemList>(this, implementation->subFoodLists);
}
}}
