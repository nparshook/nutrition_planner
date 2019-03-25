#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include "../np-core_global.h"
#include "databasecontroller.h"
#include "../models/foodsearch.h"

namespace np {
namespace controllers {

class NPCORESHARED_EXPORT MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage READ welcomeMessage CONSTANT)

    Q_PROPERTY(np::models::FoodSearch *ui_foodSearch READ foodSearch CONSTANT)

public:
    explicit MasterController(QObject* parent = nullptr);
    ~MasterController();
    const QString& welcomeMessage() const;
    np::models::FoodSearch* foodSearch();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // MASTERCONTROLLER_H
