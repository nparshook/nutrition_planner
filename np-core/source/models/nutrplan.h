#ifndef NUTRPLAN_H
#define NUTRPLAN_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include "../np-core_global.h"
#include "../controllers/databasecontroller.h"
#include "fooditemlist.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT NutrPlan : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QQmlListProperty<np::models::FoodItemList> ui_diets READ diets CONSTANT)

public:
    explicit NutrPlan(QObject *parent = nullptr, np::controllers::DatabaseController* nutrPlanDB = nullptr);
    ~NutrPlan();
    QList<FoodItemList*> diets();
    FoodItemList *createDiet(const QString& name="New Diet");

public slots:
    void saveDiet(FoodItemList* diet);

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // NUTRPLAN_H
