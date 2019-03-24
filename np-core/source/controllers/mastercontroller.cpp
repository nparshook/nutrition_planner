#include "mastercontroller.h"
#include <QDebug>

namespace np {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController)
    {
        foodDBController = new FoodDatabaseController(masterController);
        qDebug() << foodDBController->isReady();
    }

    MasterController* masterController{nullptr};
    FoodDatabaseController* foodDBController{nullptr};
    QString welcomeMessage = "Welcome";
};

MasterController::MasterController(QObject* parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this));
}

MasterController::~MasterController()
{
}

const QString& MasterController::welcomeMessage() const
{
    return implementation->welcomeMessage;
}
}}
