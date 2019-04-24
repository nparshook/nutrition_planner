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
        while (getDietsQuery->next())
        {
            Diet *diet = new Diet(getDietsQuery->value("id").toInt(), nutrPlanDBController, masterController);
            dietList.append(diet);
        }
    }

    void addDiet() {
        dietList.append(new Diet(nutrPlanDBController, masterController));
    }

    void addNewDay(FoodItemList *diet) {
        nutrPlan->createDay(diet);
    }

    QQmlListProperty<FoodItemList> diets() {
        qDebug() << "Diets";
        //qDebug() << nutrPlan->diets();
        return nutrPlan->diets();
    }

    MasterController* masterController{nullptr};
    DatabaseManager* usdaDBController{nullptr};
    DatabaseManager* nutrPlanDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
    Diet* currentDiet{nullptr};
    FoodItemList* currentDay{nullptr};
    FoodItemList* currentMeal{nullptr};
    NutrPlan* nutrPlan{nullptr};
    QList<Diet*> dietList;
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
        //return implementation->diets();
        //qDebug() << "Got Diets";
        //qDebug() << mDiets;
        return QQmlListProperty<Diet>(this, implementation->dietList);
        //return QQmlListProperty<FoodItemList>(this, mDiets);
}

void MasterController::addDiet()
{
    implementation->addDiet();
    emit dietsChanged();
}

void MasterController::addNewDayToCurrentDiet() {
    //implementation->addNewDay(implementation->currentDiet);
}

Diet *MasterController::currentDiet()
{
    return implementation->currentDiet;
}

void MasterController::setCurrentDiet(np::models::Diet *diet){
    if(diet != implementation->currentDiet) {
        implementation->currentDiet = diet;
        setCurrentDay(nullptr);
        emit currentDietChanged();
    }
}

FoodItemList *MasterController::currentDay()
{
    return implementation->currentDay;
}

void MasterController::setCurrentDay(np::models::FoodItemList *day){
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
