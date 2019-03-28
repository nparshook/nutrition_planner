#include "foodid.h"

namespace np {
namespace models {

class FoodID::Implementation
{
public:
    Implementation(FoodID* _foodID, const QString& _ndbNo, const QString& _shrtDesc, const QString& _longDesc)
        : foodID(_foodID), ndbNo(_ndbNo), shrtDesc(_shrtDesc), longDesc(_longDesc)
    {
    }

    FoodID* foodID{nullptr};
    QString ndbNo;
    QString shrtDesc;
    QString longDesc;
};

FoodID::FoodID(const QString& ndbNo, const QString& shrtDesc, const QString& longDesc, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, ndbNo, shrtDesc, longDesc));
}

FoodID::~FoodID()
{
}

QString FoodID::ndbNo() const
{
    return implementation->ndbNo;
}


QString FoodID::shrtDesc() const
{
    return implementation->shrtDesc;
}

QString FoodID::longDesc() const
{
    return implementation->longDesc;
}

void FoodID::setLongDesc(const QString &longDesc)
{
    if(longDesc != implementation->longDesc) implementation->longDesc = longDesc;
}

void FoodID::setShrtDesc(const QString &shrtDesc) {
    if(shrtDesc != implementation->shrtDesc) implementation->shrtDesc = shrtDesc;
}
}}
