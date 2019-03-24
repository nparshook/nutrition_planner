#include "fooddatabasecontroller.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include <QQmlListProperty>
#include <QDebug>

namespace np {
namespace controllers {

class FoodDatabaseController::Implementation
{
public:
    Implementation(FoodDatabaseController* _foodDBController, const QString& dbName)
        : foodDBController(_foodDBController)
    {
        initialize(dbName);
    }

    ~Implementation() {
        db.close();
    }

    void initialize(const QString& dbName) {
        qDebug() << dbName;
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
        if (!db.open()) {
            ready = false;
            return;
        }

        foodSearchQuery = QSqlQuery(db);
        foodSearchQuery.prepare("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE fdgrp_cd=:c AND shrt_desc LIKE :s;");

        foodSearchQueryAll = QSqlQuery(db);
        foodSearchQueryAll.prepare("SELECT ndb_no, shrt_desc, long_desc FROM food_des WHERE shrt_desc LIKE :s;");

        foodWeightQuery = QSqlQuery(db);
        foodWeightQuery.prepare("SELECT amount, msre_desc, gm_wgt FROM weight WHERE ndb_no=:n;");

        foodNutrientsQuery = QSqlQuery(db);
        foodNutrientsQuery.prepare("SELECT nut_data.nutr_no, nut_data.nutr_val, nutr_def.units, nutr_def.tagname, nutr_def.nutrdesc FROM nut_data INNER JOIN nutr_def ON nut_data.nutr_no=nutr_def.nutr_no WHERE ndb_no=:n ORDER BY nutr_def.tagname;");

        foodGroupsQuery = QSqlQuery(db);
        foodGroupsQuery.prepare("SELECT * FROM fd_group ORDER BY fdgrp_desc");

        ready = true;
    }

    void createFoodGrps() {
        FoodGrp *all = new FoodGrp(foodDBController, "All", "0");

        foodGroups.append(all);

        foodGroupsQuery.exec();
        while (foodGroupsQuery.next())
        {
            foodGroups.append(new FoodGrp(foodDBController, foodGroupsQuery.value("fdgrp_desc").toString(), foodGroupsQuery.value("fdgrp_cd").toString()));
        }
    }

    FoodDatabaseController* foodDBController{nullptr};
    QSqlDatabase db;
    QSqlQuery foodSearchQuery;
    QSqlQuery foodSearchQueryAll;
    QSqlQuery foodWeightQuery;
    QSqlQuery foodNutrientsQuery;
    QSqlQuery foodGroupsQuery;
    QList<FoodGrp *> foodGroups;
    bool ready = false;
};

FoodDatabaseController::FoodDatabaseController(QObject* parent, const QString& databasename)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, databasename));
}

FoodDatabaseController::~FoodDatabaseController()
{
}

bool FoodDatabaseController::isReady() {
    return implementation->ready;
}

QQmlListProperty<FoodGrp> FoodDatabaseController::foodGrps()
{
    if(implementation->foodGroups.size() == 0) implementation->createFoodGrps();
    return QQmlListProperty<FoodGrp>(this, implementation->foodGroups);
}
}}
