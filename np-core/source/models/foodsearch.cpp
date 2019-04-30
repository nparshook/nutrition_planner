#include "foodsearch.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QVariant>
#include "../database/databasemanager.h"

using namespace np::database;

namespace np {
namespace models {


class FoodSearch::Implementation
{
public:
    Implementation(FoodSearch* _foodSearch, DatabaseManager* _usdaDB)
        : foodSearch(_foodSearch), usdaDB(_usdaDB)
    {
        foodSearchQuery = usdaDB->createPreparedQuery("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE fdgrp_cd=:c AND shrt_desc LIKE :s;");
        foodDesQuery = usdaDB->createPreparedQuery("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE ndb_no=:ndb");
        foodSearchQueryAll = usdaDB->createPreparedQuery("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE shrt_desc LIKE :s;");
        foodWeightQuery = usdaDB->createPreparedQuery("SELECT seq, amount, msre_desc, gm_wgt FROM weight WHERE ndb_no=:n;");
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

    QList<QObject *>  searchFoods(int foodGrpIdx, const QString &text)
    {
        QList<QObject *> searchResults;
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
            searchResults.append(new FoodID(searchQuery->value("ndb_no").toString(),
                                            searchQuery->value("shrt_desc").toString(),
                                            searchQuery->value("long_desc").toString()));
        }
        qDebug() << searchResults.size();
        return searchResults;
    }

    FoodItem *getFood(FoodID *foodID)
    {
        FoodItem *foodItem = new FoodItem(foodSearch);
        foodItem->setFoodID(foodID);
        foodWeightQuery->bindValue(":n", foodID->ndbNo());
        foodWeightQuery->exec();
        while (foodWeightQuery->next())
        {
            foodItem->appendWeight(new FoodWgt(foodItem,
                                               foodWeightQuery->value("seq").toInt(),
                                               foodWeightQuery->value("amount").toFloat(),
                                               foodWeightQuery->value("msre_desc").toString(),
                                               foodWeightQuery->value("gm_wgt").toFloat()));
        }

        foodNutrientsQuery->bindValue(":n", foodID->ndbNo());
        foodNutrientsQuery->exec();
        while (foodNutrientsQuery->next())
        {
            foodItem->appendNutrient(new FoodNutr(foodNutrientsQuery->value("nutr_no").toInt(),
                                                  foodNutrientsQuery->value("nutr_val").toFloat(),
                                                  foodNutrientsQuery->value("nutrdesc").toString(),
                                                  foodNutrientsQuery->value("tagname").toString(),
                                                  foodNutrientsQuery->value("units").toString(),
                                                  foodItem));
        }
        qDebug() << foodNutrientsQuery->lastError();
        qDebug() << foodItem->foodID()->longDesc();
        return foodItem;
    }

    FoodItem *getFoodByNdb(const QString &ndb_no) {
        foodDesQuery->bindValue(":ndb", ndb_no);
        foodDesQuery->exec();
        foodDesQuery->next();
        FoodID* id = new FoodID(foodDesQuery->value("ndb_no").toString(),
                                foodDesQuery->value("shrt_desc").toString(),
                                foodDesQuery->value("long_desc").toString(),
                                foodSearch);
        return getFood(id);
    }

    FoodSearch* foodSearch{nullptr};
    DatabaseManager* usdaDB{nullptr};
    QSqlQuery* foodSearchQuery;
    QSqlQuery* foodDesQuery;
    QSqlQuery* foodSearchQueryAll;
    QSqlQuery* foodWeightQuery;
    QSqlQuery* foodNutrientsQuery;
    QSqlQuery* foodGroupsQuery;
    QList<FoodGrp *>foodGrps;
};

FoodSearch::FoodSearch(QObject *parent, DatabaseManager* usdaDB)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, usdaDB));
}

FoodSearch::~FoodSearch() {}

QQmlListProperty<FoodGrp> FoodSearch::foodGrps() {
    if(implementation->foodGrps.empty()) implementation->createFoodGrps();
    return QQmlListProperty<FoodGrp>(this, implementation->foodGrps);
}

QVariant FoodSearch::searchFoods(int foodGrpIdx, const QString &searchText) {
    qDebug() << "Searching for food with: " << searchText;
    return QVariant::fromValue(implementation->searchFoods(foodGrpIdx, searchText));
}

QVariant FoodSearch::getFood(FoodID *foodID)
{
    return QVariant::fromValue(implementation->getFood(foodID));
}

FoodItem* FoodSearch::getFoodByNdb(const QString &ndb_no)
{
    return implementation->getFoodByNdb(ndb_no);
}
}
}

