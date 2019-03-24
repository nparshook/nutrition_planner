#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include "fooddatabasecontroller.h"
namespace np {
namespace controllers {

class MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage READ welcomeMessage CONSTANT)

public:
    explicit MasterController(QObject* parent = nullptr);
    ~MasterController();
    const QString& welcomeMessage() const;
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // MASTERCONTROLLER_H
