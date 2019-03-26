#ifndef FOODITEM_H
#define FOODITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QList>

#include "../np-core_global.h"
#include "foodid.h"
#include "foodnutr.h"
#include "foodwgt.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(np::models::FoodID* ui_foodID READ foodID CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_protein READ protein CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_fat READ fat CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_carbs READ carbs CONSTANT)

public:
    explicit FoodItem(QObject *parent = nullptr);
    ~FoodItem();
    void setFoodID(FoodID *);
    void appendNutrient(FoodNutr *);
    void appendWeight(FoodWgt *);
    FoodID *foodID() const;
    FoodNutr *protein() const;
    FoodNutr *fat() const;
    FoodNutr *carbs() const;
    QList<FoodNutr*> nutrients();
    const float scaleFactor = 1.0;
    const float amount = 100.0;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODITEM_H
