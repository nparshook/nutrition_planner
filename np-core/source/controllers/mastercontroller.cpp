#include "mastercontroller.h"
#include <QDebug>
#include <QList>
#include <QDir>
#include "../models/nutrplan.h"

using namespace np::models;

namespace np {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController)
    {
        usdaDBController = new DatabaseController(masterController);
        nutrPlanDBController = new DatabaseController(masterController, QDir::currentPath() + "/nps.db", "conn2");
        qDebug() << usdaDBController->isReady();
        qDebug() << nutrPlanDBController->isReady();
        foodSearch = new FoodSearch(masterController, usdaDBController);
        nutrPlan = new NutrPlan(masterController, nutrPlanDBController);
        qDebug() << "System Ready";
    }

    void addDiet() {
        diets.append(nutrPlan->createDiet());
    }

    /*QList<FoodItemList*> diets() {
        qDebug() << "Diets";
        qDebug() << nutrPlan->diets();
        return nutrPlan->diets();
    }*/

    MasterController* masterController{nullptr};
    DatabaseController* usdaDBController{nullptr};
    DatabaseController* nutrPlanDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
    FoodItemList* currentDiet{nullptr};
    FoodItemList* currentDay{nullptr};
    FoodItemList* currentMeal{nullptr};
    NutrPlan* nutrPlan{nullptr};
    QList<FoodItemList*> diets;
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

QQmlListProperty<FoodItemList> MasterController::diets()
{
        //QList<FoodItemList *> mDiets = implementation->diets();
        //qDebug() << "Got Diets";
        //qDebug() << mDiets;
        return QQmlListProperty<FoodItemList>(this, implementation->diets);
}

void MasterController::addDiet()
{
    implementation->addDiet();
    emit dietsChanged();
}

FoodItemList *MasterController::currentDiet()
{
    return implementation->currentDiet;
}

void MasterController::setCurrentDiet(np::models::FoodItemList *diet){
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
    if(meal != implementation->currentDiet) {
        implementation->currentMeal = meal;
        emit currentMealChanged();
    }
}
}}
