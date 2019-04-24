#ifndef NUTRPLAN_H
#define NUTRPLAN_H

#include <QObject>
#include <QQmlListProperty>
#include <QList>
#include "../np-core_global.h"
#include "../database/databasemanager.h"
#include "fooditemlist.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT NutrPlan : public QObject
{
    Q_OBJECT

public:
    explicit NutrPlan(QObject *parent = nullptr, np::database::DatabaseManager* nutrPlanDB = nullptr);
    ~NutrPlan();
    QQmlListProperty<FoodItemList> diets();
    FoodItemList *createDiet(const QString& name="New Diet");
    FoodItemList *createDay(FoodItemList *diet, const QString& name="New Day");

public slots:
    void saveDiet(FoodItemList* diet);
    void saveDay(FoodItemList* day);

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // NUTRPLAN_H
