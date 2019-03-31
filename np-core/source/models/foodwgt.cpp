#include "foodwgt.h"

namespace np {
namespace models {

class FoodWgt::Implementation
{
public:
    Implementation(FoodWgt* _foodWgt, int _seq, float _amount, const QString &_msreDesc, float _gmWgt)
        : foodWgt(_foodWgt), seq(_seq), amount(_amount), msreDesc(_msreDesc), gmWgt(_gmWgt)
    {
    }

    FoodWgt* foodWgt{nullptr};
    int seq;
    float amount;
    QString msreDesc;
    float gmWgt;
};

FoodWgt::FoodWgt(QObject *parent, int seq, float amount, const QString &msreDesc, float gmWgt)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, seq, amount, msreDesc, gmWgt));
}

FoodWgt::~FoodWgt() {}

int FoodWgt::seq() const
{
    return implementation->seq;
}

float FoodWgt::amount() const
{
    return implementation->amount;
}

QString FoodWgt::msreDesc() const
{
    return implementation->msreDesc;
}

float FoodWgt::gmWgt() const
{
    return implementation->gmWgt;
}

}}
