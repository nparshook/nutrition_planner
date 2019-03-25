#ifndef FOODNUTR_H
#define FOODNUTR_H

#include <QObject>
#include "../np-core_global.h"
namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodNutr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ui_nutrNo READ nutrNo)
    Q_PROPERTY(float ui_nutrVal READ nutrVal)
    Q_PROPERTY(QString ui_nutrDesc READ nutrDesc)
    Q_PROPERTY(QString ui_tagName READ tagName)
    Q_PROPERTY(QString ui_units READ units)

public:
    explicit FoodNutr(int nutrNo, float nutrVal, const QString &nutrDesc, const QString &tagName, const QString &units, QObject *parent=nullptr);
    ~FoodNutr();
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
