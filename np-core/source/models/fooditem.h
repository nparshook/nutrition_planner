#ifndef FOODITEM_H
#define FOODITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QQmlListProperty>

#include "../np-core_global.h"
#include "foodid.h"
#include "foodnutr.h"
#include "foodwgt.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(np::models::FoodID* ui_foodID READ foodID CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_protein READ protein CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_fat READ fat CONSTANT)
    Q_PROPERTY(np::models::FoodNutr* ui_carbs READ carbs CONSTANT)
    Q_PROPERTY(QQmlListProperty<np::models::FoodWgt> ui_weights READ weights CONSTANT)
    Q_PROPERTY(QQmlListProperty<np::models::FoodNutr> ui_summaryNutrs READ summaryNutrs CONSTANT)
    Q_PROPERTY(QQmlListProperty<np::models::FoodNutr> ui_minerals READ minerals CONSTANT)
    Q_PROPERTY(QQmlListProperty<np::models::FoodNutr> ui_vitamins READ vitamins CONSTANT)
    Q_PROPERTY(QQmlListProperty<np::models::FoodNutr> ui_fattyAcids READ fattyAcids CONSTANT)
    Q_PROPERTY(float ui_scaleFactor READ scaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(int ui_weightIdx READ weightIdx WRITE setWeightIdx NOTIFY weightIdxChanged)
    Q_PROPERTY(float ui_amount READ amount WRITE setAmount NOTIFY amountChanged)

public:
    explicit FoodItem(QObject *parent = nullptr);
    ~FoodItem();
    void setFoodID(FoodID *);
    void appendNutrient(FoodNutr *);
    void appendWeight(FoodWgt *);
    FoodID *foodID() const;
    FoodNutr *protein() const;
    FoodNutr *fat() const;
    FoodNutr *carbs() const;
    QList<FoodNutr*> nutrients();
    QQmlListProperty<FoodWgt> weights();
    QQmlListProperty<FoodNutr> summaryNutrs();
    QQmlListProperty<FoodNutr> minerals();
    QQmlListProperty<FoodNutr> vitamins();
    QQmlListProperty<FoodNutr> fattyAcids();
    int weightIdx() const;
    void setWeightIdx(int idx);
    float amount() const;
    void setAmount(float amt);
    float scaleFactor();
    void setScaleFactor(float scaleFactor);

signals:
    void weightIdxChanged();
    void amountChanged();
    void scaleFactorChanged();
    /*void summaryNutrsChanged();
    void mineralsChanged();
    void vitaminsChanged();
    void fattyAcidsChanged();*/
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODITEM_H
