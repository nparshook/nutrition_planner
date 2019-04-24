#include "nutrplan.h"
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
using namespace np::database;

namespace np {
namespace models {

class NutrPlan::Implementation
{
public:
    Implementation(NutrPlan* _nutrPlan, DatabaseManager* _nutrPlanDB)
        : nutrPlan(_nutrPlan), nutrPlanDB(_nutrPlanDB)
    {
        init();
    }

    void init() {
        if(!nutrPlanDB->hasTable("diets")) {
            QSqlQuery *createDiets = nutrPlanDB->createPreparedQuery("CREATE TABLE diets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
            createDiets->exec();
        }
        if(!nutrPlanDB->hasTable("days")) {
            QSqlQuery *createDays = nutrPlanDB->createPreparedQuery("CREATE TABLE days (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, diet_id INTEGER)");
            createDays->exec();
        }
        if(!nutrPlanDB->hasTable("meals")) {
            QSqlQuery *createMeals = nutrPlanDB->createPreparedQuery("CREATE TABLE meals (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, day_id INTEGER)");
            createMeals->exec();
        }
        QSqlQuery *getDietsQuery = nutrPlanDB->createPreparedQuery("SELECT * FROM diets");
        getDietsQuery->exec();
        while (getDietsQuery->next())
        {
            FoodItemList *diet = new FoodItemList(nutrPlan);
            diet->setName(getDietsQuery->value("name").toString());
            diet->setKey(getDietsQuery->value("id").toInt());
            diets.append(diet);
        }
    }

    void setDays(FoodItemList *diet) {}

    FoodItemList* createDiet(const QString& name) {
        FoodItemList *newDiet = new FoodItemList(nutrPlan);
        newDiet->setName(name);
        QSqlQuery *createDietQuery = nutrPlanDB->createPreparedQuery("INSERT INTO diets (name) VALUES (:name)");
        createDietQuery->bindValue(":name", newDiet->name());
        createDietQuery->exec();
        newDiet->setKey(createDietQuery->lastInsertId().toInt());
        diets.append(newDiet);
        return newDiet;
    }

    void saveDiet(FoodItemList* diet) {
        QSqlQuery *saveDietQuery = nutrPlanDB->createPreparedQuery("UPDATE diets SET name = (:name) WHERE id = (:id)");
        saveDietQuery->bindValue(":name", diet->name());
        saveDietQuery->bindValue(":id", diet->key());
        saveDietQuery->exec();
        qDebug() << saveDietQuery->lastError();
    }

    FoodItemList* createDay(FoodItemList *diet, const QString &name) {
        FoodItemList *newDay = diet->newSubFoodList();
        newDay->setName(name);
        QSqlQuery *createDayQuery = nutrPlanDB->createPreparedQuery("INSERT INTO days (name, diet_id) VALUES (:name, :diet_id)");
        createDayQuery->bindValue(":name", newDay->name());
        createDayQuery->bindValue(":diet_id", diet->key());
        createDayQuery->exec();
        newDay->setKey(createDayQuery->lastInsertId().toInt());
        return newDay;
    }

    void saveDay(FoodItemList* day) {
        QSqlQuery *saveDayQuery = nutrPlanDB->createPreparedQuery("UPDATE days SET name = (:name) WHERE id = (:id)");
        saveDayQuery->bindValue(":name", day->name());
        saveDayQuery->bindValue(":id", day->key());
        saveDayQuery->exec();
    }

    NutrPlan* nutrPlan{nullptr};
    DatabaseManager* nutrPlanDB{nullptr};
    QList<FoodItemList *> diets;
};

NutrPlan::NutrPlan(QObject *parent, DatabaseManager* nutrPlanDB) : QObject(parent)
{
    implementation.reset(new Implementation(this, nutrPlanDB));
}

NutrPlan::~NutrPlan() {}
QQmlListProperty<FoodItemList> NutrPlan::diets() {
    return QQmlListProperty<FoodItemList>(this, implementation->diets);
}

FoodItemList* NutrPlan::createDiet(const QString& name) {
    FoodItemList* diet = implementation->createDiet(name);
    QObject::connect(diet, &FoodItemList::nameChanged, this, [=](){this->saveDiet(diet);});
    return diet;
}

void NutrPlan::saveDiet(FoodItemList* diet) {
    implementation->saveDiet(diet);
}

FoodItemList* NutrPlan::createDay(FoodItemList *diet, const QString& name)
{
    FoodItemList* day = implementation->createDay(diet, name);
    QObject::connect(day, &FoodItemList::nameChanged, this, [=](){this->saveDay(day);});
    return day;
}

void NutrPlan::saveDay(FoodItemList* day) {
    implementation->saveDay(day);
}
}}
