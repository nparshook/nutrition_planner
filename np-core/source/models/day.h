#ifndef DAY_H
#define DAY_H

#include <QObject>
#include <QScopedPointer>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "fooditem.h"
#include "meal.h"
#include "../database/databasemanager.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT Day : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(FoodItem* foodTotalEq READ foodTotalEq NOTIFY foodEqChanged)
    Q_PROPERTY(FoodItem* foodAvgEq READ foodAvgEq NOTIFY foodAvgChanged)
    Q_PROPERTY(QQmlListProperty<np::models::Meal> ui_meals READ meals NOTIFY mealsChanged)


public:
    Day(QObject *parent = nullptr);
    Day(int dietID, np::database::DatabaseManager *manager, bool isNew = true, QObject *parent = nullptr);
    ~Day();

    QString name() const;
    void setName(const QString &name);

    void setKey(int key);
    int key();

    FoodItem* foodTotalEq();
    FoodItem* foodAvgEq();

    QQmlListProperty<np::models::Meal> meals();
    Q_INVOKABLE QVariant newMeal();

signals:
    void nameChanged();
    void mealsChanged();
    void foodEqChanged();
    void foodAvgChanged();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // DAY_H
