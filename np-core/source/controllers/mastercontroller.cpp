#include "mastercontroller.h"
#include <QDebug>

using namespace np::models;

namespace np {
namespace controllers {

class MasterController::Implementation
{
public:
    Implementation(MasterController* _masterController)
        : masterController(_masterController)
    {
        usdaDBController = new DatabaseController(masterController);
        qDebug() << usdaDBController->isReady();
        foodSearch = new FoodSearch(masterController, usdaDBController);
    }

    MasterController* masterController{nullptr};
    DatabaseController* usdaDBController{nullptr};
    FoodSearch* foodSearch{nullptr};
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

FoodSearch *MasterController::foodSearch()
{
    return implementation->foodSearch;
}
}}
