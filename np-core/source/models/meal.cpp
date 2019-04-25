#include "meal.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>

using namespace np::database;

namespace np {
namespace models {

class Meal::Implementation {
public:

    Implementation(Meal* _meal, DatabaseManager* _manager, int ID, bool isNew)
        : meal(_meal), manager(_manager)
    {
        name = "New meal...";
        foodID = new FoodID("0", name, name, meal);
        if(isNew) {
            create(ID);
        } else {
            load(ID);
        }
    }

    void init() {
        if(!manager->hasTable("meals")) {
            QSqlQuery *createMeals = manager->createPreparedQuery("CREATE TABLE meals (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, day_id INTEGER)");
            createMeals->exec();
        }
    }

    void load(int mealID) {
        QSqlQuery *getMealQuery = manager->createPreparedQuery("SELECT name FROM meals WHERE id=(:id)");
        getMealQuery->bindValue(":id", mealID);
        getMealQuery->exec();
        getMealQuery->next();

        name = getMealQuery->value("name").toString();
        key = mealID;
    }

    void create(int dayID) {
        init();
        QSqlQuery *createMealQuery = manager->createPreparedQuery("INSERT INTO meals (name, day_id) VALUES (:name, :id)");
        createMealQuery->bindValue(":name", name);
        createMealQuery->bindValue(":id", dayID);
        createMealQuery->exec();
        key = createMealQuery->lastInsertId().toInt();
    }

    void save() {
        QSqlQuery *saveMealQuery = manager->createPreparedQuery("UPDATE meals SET name = (:name) WHERE id = (:id)");
        saveMealQuery->bindValue(":name", name);
        saveMealQuery->bindValue(":id", key);
        saveMealQuery->exec();
    }

    bool setName(const QString &_name) {
        if(name == _name) return false;
        name = _name;
        foodID->setLongDesc(name);
        foodID->setShrtDesc(name);
        save();
        return true;
    }

    Meal* meal{nullptr};
    DatabaseManager* manager{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
};

Meal::Meal(QObject *parent)
    : QObject(parent)
{}

Meal::Meal(int dbID, DatabaseManager *manager, bool isNew, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, dbID, isNew));
}

Meal::~Meal() {}

QString Meal::name() const
{
    return implementation->name;
}

FoodItem* Meal::foodAvgEq()
{
    return implementation->foodAvgEq;
}

FoodItem* Meal::foodTotalEq()
{
    return implementation->foodTotalEq;
}

void Meal::setName(const QString &name)
{
    if(implementation->setName(name)) emit nameChanged();
}

/*void Meal::recalculate()
{
    implementation->createFoodEqs();
    emit foodEqChanged();
    emit foodAvgChanged();
    emit gmWgtChanged();
}*/

void Meal::setKey(int key) {
    if (implementation->key != key) {
        implementation->key = key;
    }
}

int Meal::key() {
    return implementation->key;
}

}}
