#ifndef FOODNUTR_H
#define FOODNUTR_H

#include <QObject>
#include "../np-core_global.h"
namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodNutr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ui_nutrNo READ nutrNo CONSTANT)
    Q_PROPERTY(float ui_nutrVal READ nutrVal CONSTANT)
    Q_PROPERTY(QString ui_nutrDesc READ nutrDesc CONSTANT)
    Q_PROPERTY(QString ui_tagName READ tagName CONSTANT)
    Q_PROPERTY(QString ui_units READ units CONSTANT)

public:
    explicit FoodNutr(int nutrNo, float nutrVal, const QString &nutrDesc, const QString &tagName, const QString &units, QObject *parent=nullptr);
    explicit FoodNutr(QObject *parent = nullptr);
    ~FoodNutr();
    void setNutrVal(float nutrVal);
    int nutrNo() const;
    float nutrVal() const;
    QString nutrDesc() const;
    QString tagName() const;
    QString units() const;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODNUTR_H
