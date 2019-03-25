#include "foodsearch.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QVariant>
#include "../controllers/databasecontroller.h"
#include "foodgrp.h"
#include "foodid.h"

using namespace np::controllers;

namespace np {
namespace models {


class FoodSearch::Implementation
{
public:
    Implementation(FoodSearch* _foodSearch, DatabaseController* _usdaDB)
        : foodSearch(_foodSearch), usdaDB(_usdaDB)
    {
        foodSearchQuery = usdaDB->createPreparedQuery("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE fdgrp_cd=:c AND shrt_desc LIKE :s;");
        foodSearchQueryAll = usdaDB->createPreparedQuery("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE shrt_desc LIKE :s;");
        foodWeightQuery = usdaDB->createPreparedQuery("SELECT amount, msre_desc, gm_wgt FROM weight WHERE ndb_no=:n;");
        foodNutrientsQuery = usdaDB->createPreparedQuery("SELECT nut_data.nutr_no, nut_data.nutr_val, nutr_def.units, nutr_def.tagname, nutr_def.nutrdesc FROM nut_data INNER JOIN nutr_def ON nut_data.nutr_no=nutr_def.nutr_no WHERE ndb_no=:n ORDER BY nutr_def.tagname;");
        foodGroupsQuery = usdaDB->createPreparedQuery("SELECT * FROM fd_group ORDER BY fdgrp_desc");
    }

    void createFoodGrps() {
        foodGrps.clear();
        FoodGrp *all = new FoodGrp(foodSearch, "All", "0");

        foodGrps.append(all);
        foodGroupsQuery->exec();
        while (foodGroupsQuery->next())
        {
            foodGrps.append(new FoodGrp(foodSearch, foodGroupsQuery->value("fdgrp_desc").toString(), foodGroupsQuery->value("fdgrp_cd").toString()));
        }
    }

    QList<FoodID *>  searchFoods(int foodGrpIdx, const QString &text)
    {
        QList<FoodID *> searchResults;
        if (text == "") {
            return searchResults;
        }

        FoodGrp* foodGrp = foodGrps.at(foodGrpIdx);

        QSqlQuery *searchQuery;

        if(foodGrp->code() == "0") {
            searchQuery = foodSearchQueryAll;
        } else {
            searchQuery = foodSearchQuery;
            foodSearchQuery->bindValue(":c", foodGrp->code());
        }

        searchQuery->bindValue(":s", "%" + text + "%");
        searchQuery->exec();
        while (searchQuery->next())
        {
            searchResults.append(new FoodID(searchQuery->value("ndb_no").toString(), searchQuery->value("shrt_desc").toString(), searchQuery->value("long_desc").toString()));
        }

        return searchResults;
    }

    FoodSearch* foodSearch{nullptr};
    DatabaseController* usdaDB{nullptr};
    QSqlQuery* foodSearchQuery;
    QSqlQuery* foodSearchQueryAll;
    QSqlQuery* foodWeightQuery;
    QSqlQuery* foodNutrientsQuery;
    QSqlQuery* foodGroupsQuery;
    QList<FoodGrp *>foodGrps;
};

FoodSearch::FoodSearch(QObject *parent, DatabaseController* usdaDB)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, usdaDB));
}

FoodSearch::~FoodSearch() {}

QQmlListProperty<FoodGrp> FoodSearch::foodGrps() {
    if(implementation->foodGrps.empty()) implementation->createFoodGrps();
    qDebug() << implementation->foodGrps.size();
    return QQmlListProperty<FoodGrp>(this, implementation->foodGrps);
}

QVariant FoodSearch::searchFoods(int foodGrpIdx, const QString &searchText) {
    return QVariant::fromValue(implementation->searchFoods(foodGrpIdx, searchText));
}
}
}

