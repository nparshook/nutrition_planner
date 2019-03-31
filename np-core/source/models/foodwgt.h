#ifndef FOODWGT_H
#define FOODWGT_H

#include <QObject>
#include <QScopedPointer>
#include "../np-core_global.h"

namespace np {
namespace models {


class NPCORESHARED_EXPORT FoodWgt : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ui_seq READ seq CONSTANT)
    Q_PROPERTY(float ui_amount READ amount CONSTANT)
    Q_PROPERTY(QString ui_msreDesc READ msreDesc CONSTANT)
    Q_PROPERTY(float ui_gmWgt READ gmWgt CONSTANT)

public:
    explicit FoodWgt(QObject *parent = nullptr, int seq=-1, float amount=0.0, const QString &msreDesc="MSREDESC", float gmWgt=1.0);
    ~FoodWgt();

    int seq() const;
    float amount() const;
    QString msreDesc() const;
    float gmWgt() const;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODWGT_H
