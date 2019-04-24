#include "mastercontroller.h"
#include <QDebug>
#include <QList>
#include <QDir>
#include <QSqlQuery>
#include "../models/nutrplan.h"
#include "../models/diet.h"

using namespace np::models;
using namespace np::database;

namespace np {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController)
    {
        usdaDBController = new DatabaseManager(masterController, QDir::currentPath() + "/usda.db");
        nutrPlanDBController = new DatabaseManager(masterController, QDir::currentPath() + "/nps.db", "conn2");
        qDebug() << usdaDBController->isReady();
        qDebug() << nutrPlanDBController->isReady();
        foodSearch = new FoodSearch(masterController, usdaDBController);
        nutrPlan = new NutrPlan(masterController, nutrPlanDBController);
        loadDiets();
        qDebug() << "System Ready";
    }

    void loadDiets() {

        QSqlQuery *getDietsQuery = nutrPlanDBController->createPreparedQuery("SELECT id FROM diets");
        getDietsQuery->exec();
        dietList.clear();
        while (getDietsQuery->next())
        {
            Diet *diet = new Diet(getDietsQuery->value("id").toInt(), nutrPlanDBController, masterController);
            dietList.append(diet);
        }
    }

    void loadDays() {
        QSqlQuery *getDaysQuery = nutrPlanDBController->createPreparedQuery("SELECT id FROM days WHERE diet_id=(:id)");
        getDaysQuery->bindValue(":id", currentDiet->key());
        getDaysQuery->exec();
        dayList.clear();
        while (getDaysQuery->next())
        {
            Day *day = new Day(getDaysQuery->value("id").toInt(), nutrPlanDBController, false, masterController);
            dayList.append(day);
        }
    }

    Diet* addDiet() {
        Diet* newDiet = new Diet(nutrPlanDBController, masterController);
        dietList.append(newDiet);
        return newDiet;
    }

    Day* addNewDay() {
        Day* newDay = new Day(currentDiet->key(), nutrPlanDBController, true, masterController);
        dayList.append(newDay);
        return newDay;
    }

    MasterController* masterController{nullptr};
    DatabaseManager* usdaDBController{nullptr};
    DatabaseManager* nutrPlanDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
    Diet* currentDiet{nullptr};
    Day* currentDay{nullptr};
    FoodItemList* currentMeal{nullptr};
    NutrPlan* nutrPlan{nullptr};
    QList<Diet*> dietList;
    QList<Day*> dayList;
    QString welcomeMessage = "Welcome";
};

MasterController::MasterController(QObject* parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this));
}

MasterController::~MasterController()
{
}

const QString& MasterController::welcomeMessage() const
{
    return implementation->welcomeMessage;
}

FoodSearch *MasterController::foodSearch()
{
    return implementation->foodSearch;
}

QQmlListProperty<Diet> MasterController::diets()
{
    return QQmlListProperty<Diet>(this, implementation->dietList);
}

void MasterController::addDiet()
{
    Diet* newDiet = implementation->addDiet();
    emit dietsChanged();
    setCurrentDiet(newDiet);
}

void MasterController::addNewDayToCurrentDiet() {
    Day* newDay = implementation->addNewDay();
    emit daysChanged();
    setCurrentDay(newDay);
}

QQmlListProperty<Day> MasterController::days()
{
    return QQmlListProperty<Day>(this, implementation->dayList);
}

Diet *MasterController::currentDiet()
{
    return implementation->currentDiet;
}

void MasterController::setCurrentDiet(np::models::Diet *diet){
    if(diet != implementation->currentDiet) {
        implementation->currentDiet = diet;
        implementation->loadDays();
        setCurrentDay(nullptr);
        emit currentDietChanged();
        emit daysChanged();
    }
}

Day *MasterController::currentDay()
{
    return implementation->currentDay;
}

void MasterController::setCurrentDay(np::models::Day *day){
    if(day != implementation->currentDay) {
        implementation->currentDay = day;
        setCurrentMeal(nullptr);
        emit currentDayChanged();
    }
}

FoodItemList *MasterController::currentMeal()
{
    return implementation->currentMeal;
}

void MasterController::setCurrentMeal(np::models::FoodItemList *meal){
    if(meal != implementation->currentMeal) {
        implementation->currentMeal = meal;
        emit currentMealChanged();
    }
}
}}
