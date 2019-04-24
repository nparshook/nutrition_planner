#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "../database/databasemanager.h"
#include "../models/foodsearch.h"
#include "../models/fooditemlist.h"
#include "../models/diet.h"

namespace np {
namespace controllers {

class NPCORESHARED_EXPORT MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage READ welcomeMessage CONSTANT)

    Q_PROPERTY(np::models::FoodSearch *ui_foodSearch READ foodSearch CONSTANT)
    Q_PROPERTY(np::models::Diet *ui_currentDiet READ currentDiet WRITE setCurrentDiet NOTIFY currentDietChanged)
    Q_PROPERTY(np::models::FoodItemList *ui_currentDay READ currentDay WRITE setCurrentDay NOTIFY currentDayChanged)
    Q_PROPERTY(np::models::FoodItemList *ui_currentMeal READ currentMeal WRITE setCurrentMeal NOTIFY currentMealChanged)
    Q_PROPERTY(QQmlListProperty<np::models::Diet> ui_diets READ diets NOTIFY dietsChanged)

public:
    explicit MasterController(QObject* parent = nullptr);
    ~MasterController();
    const QString& welcomeMessage() const;
    np::models::FoodSearch* foodSearch();
    QQmlListProperty<np::models::Diet> diets();
    Q_INVOKABLE void addDiet();
    Q_INVOKABLE void addNewDayToCurrentDiet();
    np::models::Diet* currentDiet();
    void setCurrentDiet(np::models::Diet* diet);
    np::models::FoodItemList* currentDay();
    void setCurrentDay(np::models::FoodItemList* day);
    np::models::FoodItemList* currentMeal();
    void setCurrentMeal(np::models::FoodItemList* meal);

signals:
    void dietsChanged();
    void currentDietChanged();
    void currentDayChanged();
    void currentMealChanged();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // MASTERCONTROLLER_H
