#include "nutrplan.h"
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
using namespace np::controllers;

namespace np {
namespace models {

class NutrPlan::Implementation
{
public:
    Implementation(NutrPlan* _nutrPlan, DatabaseController* _nutrPlanDB)
        : nutrPlan(_nutrPlan), nutrPlanDB(_nutrPlanDB)
    {
        init();
    }

    void init() {
        if(!nutrPlanDB->hasTable("diets")) {
            QSqlQuery *createNP = nutrPlanDB->createPreparedQuery("CREATE TABLE diets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
            createNP->exec();
        }
    }

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
        qDebug() << "Saving";
        qDebug() << diet->name();
        qDebug() << diet->key();
        QSqlQuery *saveDietQuery = nutrPlanDB->createPreparedQuery("UPDATE diets SET name = (:name) WHERE id = (:id)");
        saveDietQuery->bindValue(":name", diet->name());
        saveDietQuery->bindValue(":id", diet->key());
        saveDietQuery->exec();
        qDebug() << saveDietQuery->lastError();
    }

    NutrPlan* nutrPlan{nullptr};
    DatabaseController* nutrPlanDB{nullptr};
    QList<FoodItemList *> diets;
};

NutrPlan::NutrPlan(QObject *parent, DatabaseController* nutrPlanDB) : QObject(parent)
{
    implementation.reset(new Implementation(this, nutrPlanDB));
}

NutrPlan::~NutrPlan() {}
QList<FoodItemList*> NutrPlan::diets() {
    return implementation->diets;
}

FoodItemList* NutrPlan::createDiet(const QString& name) {
    FoodItemList* diet = implementation->createDiet(name);
    QObject::connect(diet, &FoodItemList::nameChanged, this, [=](){this->saveDiet(diet);});
    return diet;
}

void NutrPlan::saveDiet(FoodItemList* diet) {
    implementation->saveDiet(diet);
}
}}
