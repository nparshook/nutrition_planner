#include "fooditem.h"
#include <QList>
#include <QDebug>

namespace np {
namespace models {

class FoodItem::Implementation
{
public:
    Implementation(FoodItem* _foodItem)
        : foodItem(_foodItem)
    {
        weightIdx = 0;
        amount = 100.0;
        scaleFactor = 1.0;
        weights.append(new FoodWgt(foodItem, -1, 1, "Grams", 1));
    }

    FoodItem* foodItem{nullptr};
    FoodID* foodID{nullptr};
    FoodNutr* protein{nullptr};
    FoodNutr* fat{nullptr};
    FoodNutr* carbs{nullptr};
    QList<FoodWgt *>weights;
    QList<FoodNutr *> nutrients;
    int weightIdx;
    float amount;
    float scaleFactor;
};

FoodItem::FoodItem(QObject *parent) : QObject(parent)
{
    implementation.reset(new Implementation(this));
}

FoodItem::~FoodItem() {}

FoodID* FoodItem::foodID() const
{
    return implementation->foodID;
}

FoodNutr* FoodItem::protein() const
{
    return implementation->protein;
}

FoodNutr* FoodItem::fat() const
{
    return implementation->fat;
}

FoodNutr* FoodItem::carbs() const
{
    return implementation->carbs;
}

QList<FoodNutr*> FoodItem::nutrients() {
    return implementation->nutrients;
}

void FoodItem::setFoodID(FoodID *foodID)
{
    if(foodID != implementation->foodID) implementation->foodID = foodID;
}

void FoodItem::appendWeight(FoodWgt *weight)
{
    implementation->weights.append(weight);
}

void FoodItem::appendNutrient(FoodNutr *nutrient)
{
    implementation->nutrients.append(nutrient);
    int nutrNo = nutrient->nutrNo();

    switch(nutrNo)
    {
    case 203:
        implementation->protein = nutrient;
        break;
    case 204:
        implementation->fat = nutrient;
        break;
    case 205:
        implementation->carbs = nutrient;
        break;
    /*case 208: m_cals = nutrient;
        break;
    case 601: m_nutrSummary.append(nutrient);
        emit nutrSummaryChanged();
        break;*/
    default:
        break;
        /*if(nutrNo <= 291) {
            m_nutrSummary.append(nutrient);
            emit nutrSummaryChanged();
        } else if (nutrNo <= 317) {
            m_minerals.append(nutrient);
            emit mineralsChanged();
        } else if (nutrNo <= 578) {
            m_vitamins.append(nutrient);
            emit vitaminsChanged();
        } else {
            m_fattyAcids.append(nutrient);
            emit fattyAcidsChanged();
        }*/
    }
}

int FoodItem::weightIdx() const
{
    return implementation->weightIdx;
}

void FoodItem::setWeightIdx(int idx) {
    if (implementation->weightIdx != idx) {
        implementation->weightIdx = idx;
        implementation->scaleFactor = implementation->weights[implementation->weightIdx]->gmWgt();
        emit weightIdxChanged();
        emit scaleFactorChanged();
    }
}

float FoodItem::amount() const
{
    return implementation->amount;
}

void FoodItem::setAmount(float amt) {
    if (implementation->amount != amt) {
        implementation->amount = amt;
        emit amountChanged();
    }
}

float FoodItem::scaleFactor()
{
    return implementation->scaleFactor;
}

QQmlListProperty<FoodWgt> FoodItem::weights()
{
    return QQmlListProperty<FoodWgt>(this, implementation->weights);
}

void FoodItem::setScaleFactor(float scaleFactor)
{
    if (implementation->scaleFactor != scaleFactor) {
        implementation->scaleFactor = scaleFactor;
        emit scaleFactorChanged();
    }
}
}}
