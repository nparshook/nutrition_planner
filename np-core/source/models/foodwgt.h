#ifndef FOODWGT_H
#define FOODWGT_H

#include <QObject>
#include "../np-core_global.h"

namespace np {
namespace models {


class NPCORESHARED_EXPORT FoodWgt : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ui_seq READ seq)
    Q_PROPERTY(float ui_amount READ amount)
    Q_PROPERTY(QString ui_msreDesc READ msreDesc)
    Q_PROPERTY(float ui_gmWgt READ gmWgt)

public:
    explicit FoodWgt(int seq, float amount, const QString &msreDesc, float gmWgt, QObject *parent = nullptr);
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
