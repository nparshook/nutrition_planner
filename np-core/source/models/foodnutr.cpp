#include "foodnutr.h"

namespace np {
namespace models {

class FoodNutr::Implementation
{
public:
    Implementation(FoodNutr* _foodNutr, int _nutrNo, float _nutrVal, const QString& _nutrDesc, const QString& _tagName, const QString& _units)
        : foodNutr(_foodNutr), nutrNo(_nutrNo), nutrVal(_nutrVal), nutrDesc(_nutrDesc), tagName(_tagName), units(_units)
    {
    }

    FoodNutr* foodNutr{nullptr};
    int nutrNo;
    float nutrVal;
    QString nutrDesc;
    QString tagName;
    QString units;
};

FoodNutr::FoodNutr(int nutrNo, float nutrVal, const QString &nutrDesc, const QString &tagName, const QString &units, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, nutrNo, nutrVal, nutrDesc, tagName, units));
}

FoodNutr::~FoodNutr(){}

void FoodNutr::setNutrVal(float nutrVal)
{
    implementation->nutrVal = nutrVal;
}

int FoodNutr::nutrNo() const
{
    return implementation->nutrNo;
}

float FoodNutr::nutrVal() const
{
    return implementation->nutrVal;
}

QString FoodNutr::nutrDesc() const
{
    return implementation->nutrDesc;
}

QString FoodNutr::tagName() const
{
    return implementation->tagName;
}

QString FoodNutr::units() const
{
    return implementation->units;
}
}}
