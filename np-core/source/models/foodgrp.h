#ifndef FOODGRP_H
#define FOODGRP_H

#include <QObject>
#include <QScopedPointer>

#include "../np-core_global.h"
namespace np {
namespace models {

class NPCORESHARED_EXPORT FoodGrp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_desc READ desc CONSTANT)
    Q_PROPERTY(QString ui_code READ code CONSTANT)

public:
    explicit FoodGrp(QObject *parent = nullptr, const QString& desc = "DESC", const QString& code = "CODE");
    ~FoodGrp();

    QString desc() const;
    QString code() const;

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODGRP_H
