#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

namespace np {
namespace controllers {

class MasterController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage READ welcomeMessage CONSTANT)

public:
    explicit MasterController(QObject* parent = nullptr);
    ~MasterController();
    //NavigationController* navigationController();
    const QString& welcomeMessage() const;
    //CommandController* commandController();
private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}

#endif // MASTERCONTROLLER_H
