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
        amountSet = false;
        key = -1;
        createFoodEqs();
    }

    void createFoodEqs()
    {
        QHash<int, FoodNutr *> nutrsTable;

        gmWgt = 0;

        for(int i = 0; i < foodItems.size(); i++) {
            QList<FoodNutr *> nutrs = foodItems[i]->nutrients();
            float scaleFactor = foodItems[i]->scaleFactor();
            float amount = foodItems[i]->amount();
            gmWgt += scaleFactor * amount;
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

        for(int i = 0; i < subFoodLists.size(); i++) {
            FoodItem* foodItem = subFoodLists[i]->foodTotalEq();
            gmWgt += subFoodLists[i]->gmWgt();
            float sf = subFoodLists[i]->amount() / subFoodLists[i]->gmWgt();
            QList<FoodNutr *> nutrs = foodItem->nutrients();
            for(int j = 0; j < nutrs.size(); j++) {
                FoodNutr *nutr = nutrs[j];
                if(!nutrsTable.contains(nutr->nutrNo()))
                {
                   nutrsTable[nutr->nutrNo()] = new FoodNutr(nutr->nutrNo(), nutr->nutrVal() * sf, nutr->nutrDesc(), nutr->tagName(), nutr->units());
                } else {
                    nutrsTable[nutr->nutrNo()]->setNutrVal(nutr->nutrVal() * sf + nutrsTable[nutr->nutrNo()]->nutrVal());
                }
            }
        }
        if(!amountSet) amount = gmWgt;
        FoodItem* totalItem = new FoodItem(foodItemList);
        FoodItem* avgItem = new FoodItem(foodItemList);
        totalItem->setFoodID(foodID);
        avgItem->setFoodID(foodID);
        totalItem->setScaleFactor(amount / gmWgt);
        avgItem->setScaleFactor(amount / gmWgt);
        QHashIterator<int, FoodNutr *> h(nutrsTable);
        while (h.hasNext()) {
            h.next();
            FoodNutr *nutr = h.value();
            totalItem->appendNutrient(nutr);
            avgItem->appendNutrient(new FoodNutr(nutr->nutrNo(), nutr->nutrVal() / (foodItems.size() + subFoodLists.size()), nutr->nutrDesc(), nutr->tagName(), nutr->units()));
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

    void setAmount(float _amount)
    {
        amountSet = true;
        amount = _amount;
        foodTotalEq->setScaleFactor(amount / gmWgt);
        foodAvgEq->setScaleFactor(amount / gmWgt);
    }

    FoodItemList* addSubFoodList() {
        FoodItemList *subList = new FoodItemList(foodItemList);
        subFoodLists.append(subList);
        createFoodEqs();
        return subList;
    }

    QString name;
    QList<FoodItem *> foodItems;
    QList<FoodItemList *> subFoodLists;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodItemList* foodItemList{nullptr};
    FoodID *foodID{nullptr};
    float gmWgt;
    float amount;
    bool amountSet;
    int key;
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
    QObject::connect(foodItem, &FoodItem::amountChanged, this, &FoodItemList::recalculate);
    QObject::connect(foodItem, &FoodItem::scaleFactorChanged, this, &FoodItemList::recalculate);
    recalculate();
    emit foodItemsChanged();
}

void FoodItemList::deleteFood(int index)
{
    implementation->foodItems.removeAt(index);
    recalculate();
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
    FoodItemList *oldSubList = implementation->subFoodLists[index];
    QObject::disconnect(oldSubList, &FoodItemList::foodEqChanged, this, &FoodItemList::recalculate);
    QObject::disconnect(oldSubList, &FoodItemList::foodAvgChanged, this, &FoodItemList::recalculate);
    implementation->subFoodLists.removeAt(index);
    recalculate();
    emit subFoodListsChanged();
}

void FoodItemList::addSubFoodList()
{
    FoodItemList* newSubList = implementation->addSubFoodList();
    QObject::connect(newSubList, &FoodItemList::foodEqChanged, this, &FoodItemList::recalculate);
    QObject::connect(newSubList, &FoodItemList::foodAvgChanged, this, &FoodItemList::recalculate);
    QObject::connect(newSubList, &FoodItemList::amountChanged, this, &FoodItemList::recalculate);
    emit subFoodListsChanged();
}

FoodItemList* FoodItemList::newSubFoodList()
{
    FoodItemList* newSubList = implementation->addSubFoodList();
    QObject::connect(newSubList, &FoodItemList::foodEqChanged, this, &FoodItemList::recalculate);
    QObject::connect(newSubList, &FoodItemList::foodAvgChanged, this, &FoodItemList::recalculate);
    QObject::connect(newSubList, &FoodItemList::amountChanged, this, &FoodItemList::recalculate);
    emit subFoodListsChanged();
    return newSubList;
}

QQmlListProperty<FoodItemList> FoodItemList::subFoodLists()
{
    return QQmlListProperty<FoodItemList>(this, implementation->subFoodLists);
}

void FoodItemList::recalculate()
{
    implementation->createFoodEqs();
    emit foodEqChanged();
    emit foodAvgChanged();
    emit gmWgtChanged();
}

float FoodItemList::gmWgt() {
    return implementation->gmWgt;
}

float FoodItemList::amount() {
    return implementation->amount;
}

void FoodItemList::setAmount(float amount) {
    if (implementation->amount != amount) {
        qDebug() << implementation->name << ":" << implementation->amount << ":" << amount;
        implementation->setAmount(amount);
        emit amountChanged();
    }
}

void FoodItemList::setKey(int key) {
    if (implementation->key != key) {
        implementation->key = key;
    }
}

int FoodItemList::key() {
    return implementation->key;
}

}}
