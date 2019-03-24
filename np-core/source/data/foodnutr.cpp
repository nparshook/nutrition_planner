#include "foodnutr.h"

namespace np {
namespace controllers {

class FoodGrp::Implementation
{
public:
    Implementation(FoodGrp* _foodGrp, const QString& _desc, const QString& _code)
        : foodGrp(_foodGrp), desc(_desc), code(_code)
    {
    }

    FoodGrp* foodGrp{nullptr};
    QString desc;
    QString code;
};

FoodGrp::FoodGrp(QObject *parent, const QString& desc, const QString& code) : QObject(parent)
{
    implementation.reset(new Implementation(this, desc, code));
}

FoodGrp::~FoodGrp()
{

}

QString FoodGrp::desc() const
{
    return implementation->desc;
}

QString FoodGrp::code() const
{
    return implementation->code;
}
}}
