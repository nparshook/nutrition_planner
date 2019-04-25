#include "mastercontroller.h"
#include <QDebug>
#include <QList>
#include <QDir>
#include <QSqlQuery>

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
        loadDiets();
        qDebug() << "System Ready";
    }

    void loadDiets() {

        QSqlQuery *getDietsQuery = nutrPlanDBController->createPreparedQuery("SELECT id FROM diets");
        getDietsQuery->exec();
        dietList.clear();
        while (getDietsQuery->next())
        {
            Diet *diet = new Diet(getDietsQuery->value("id").toInt(), nutrPlanDBController, foodSearch, masterController);
            dietList.append(diet);
        }
    }

    Diet* addDiet() {
        Diet* newDiet = new Diet(nutrPlanDBController, foodSearch, masterController);
        dietList.append(newDiet);
        return newDiet;
    }

    MasterController* masterController{nullptr};
    DatabaseManager* usdaDBController{nullptr};
    DatabaseManager* nutrPlanDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
    Diet* currentDiet{nullptr};
    Day* currentDay{nullptr};
    Meal* currentMeal{nullptr};
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
    return QQmlListProperty<Diet>(this, implementation->dietList);
}

void MasterController::addDiet()
{
    Diet* newDiet = implementation->addDiet();
    emit dietsChanged();
    setCurrentDiet(newDiet);
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

Meal *MasterController::currentMeal()
{
    return implementation->currentMeal;
}

void MasterController::setCurrentMeal(np::models::Meal *meal){
    if(meal != implementation->currentMeal) {
        implementation->currentMeal = meal;
        emit currentMealChanged();
    }
}
}}
