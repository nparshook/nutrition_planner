#include "mastercontroller.h"
#include <QDebug>
#include <QList>

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
        qDebug() << usdaDBController->isReady();
        foodSearch = new FoodSearch(masterController, usdaDBController);
    }

    void addDiet() {
        FoodItemList *newDiet = new FoodItemList(masterController);
        newDiet->setName("New Diet");
        diets.append(newDiet);
    }

    MasterController* masterController{nullptr};
    DatabaseController* usdaDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
    FoodItemList* currentDiet{nullptr};
    FoodItemList* currentDay{nullptr};
    FoodItemList* currentMeal{nullptr};
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
