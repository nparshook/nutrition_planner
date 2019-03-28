#ifndef FOODID_H
#define FOODID_H

#include <QObject>
#include <QString>
#include "../np-core_global.h"

namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodID : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_ndbNo READ ndbNo CONSTANT)
    Q_PROPERTY(QString ui_shrtDesc READ shrtDesc CONSTANT)
    Q_PROPERTY(QString ui_longDesc READ longDesc CONSTANT)

public:
    explicit FoodID(const QString &ndbNo, const QString &shrtDesc, const QString &longDesc, QObject *parent=nullptr);
    ~FoodID();
    QString ndbNo() const;
    QString shrtDesc() const;
    QString longDesc() const;

    void setShrtDesc(const QString &shrtDesc);
    void setLongDesc(const QString &longDesc);

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;

};
}}
#endif // FOODID_H
