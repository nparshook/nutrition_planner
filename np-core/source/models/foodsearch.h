#ifndef FOODSEARCH_H
#define FOODSEARCH_H

#include <QObject>
#include <QQmlListProperty>
#include "../np-core_global.h"
#include "../controllers/databasecontroller.h"
#include "foodgrp.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodSearch : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<np::models::FoodGrp> ui_foodGrps READ foodGrps CONSTANT)

public:
    explicit FoodSearch( QObject *parent = nullptr, np::controllers::DatabaseController* usdaDB = nullptr);
    ~FoodSearch();
    QQmlListProperty<FoodGrp> foodGrps();
    Q_INVOKABLE QVariant searchFoods(int foodGrpIdx, const QString &searchText);
    //Q_INVOKABLE FoodItem *getFood(FoodID *);

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODSEARCH_H
