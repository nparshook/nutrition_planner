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
#include "../models/day.h"
#include "../models/meal.h"

namespace np {
namespace controllers {

class NPCORESHARED_EXPORT MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage READ welcomeMessage CONSTANT)

    Q_PROPERTY(np::models::FoodSearch *ui_foodSearch READ foodSearch CONSTANT)
    Q_PROPERTY(np::models::Diet *ui_currentDiet READ currentDiet WRITE setCurrentDiet NOTIFY currentDietChanged)
    Q_PROPERTY(np::models::Day *ui_currentDay READ currentDay WRITE setCurrentDay NOTIFY currentDayChanged)
    Q_PROPERTY(np::models::Meal *ui_currentMeal READ currentMeal WRITE setCurrentMeal NOTIFY currentMealChanged)
    Q_PROPERTY(QQmlListProperty<np::models::Diet> ui_diets READ diets NOTIFY dietsChanged)
public:
    explicit MasterController(QObject* parent = nullptr);
    ~MasterController();
    const QString& welcomeMessage() const;
    np::models::FoodSearch* foodSearch();
    QQmlListProperty<np::models::Diet> diets();
    QQmlListProperty<np::models::Day> days();
    Q_INVOKABLE void addDiet();
    np::models::Diet* currentDiet();
    void setCurrentDiet(np::models::Diet* diet);
    np::models::Day* currentDay();
    void setCurrentDay(np::models::Day* day);
    np::models::Meal* currentMeal();
    void setCurrentMeal(np::models::Meal* meal);

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
