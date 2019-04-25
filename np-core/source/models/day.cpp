#include "day.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>

using namespace np::database;

namespace np {
namespace models {

class Day::Implementation {
public:

    Implementation(Day* _day, DatabaseManager* _manager, int ID, bool isNew)
        : day(_day), manager(_manager)
    {
        name = "New day...";
        foodID = new FoodID("0", name, name, day);
        if(isNew) {
            create(ID);
        } else {
            load(ID);
        }
    }

    void init() {
        if(!manager->hasTable("days")) {
            QSqlQuery *createDays = manager->createPreparedQuery("CREATE TABLE days (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, diet_id INTEGER)");
            createDays->exec();
        }
    }

    void load(int dayID) {
        QSqlQuery *getDayQuery = manager->createPreparedQuery("SELECT name FROM days WHERE id=(:id)");
        getDayQuery->bindValue(":id", dayID);
        getDayQuery->exec();
        getDayQuery->next();

        name = getDayQuery->value("name").toString();
        key = dayID;
    }

    void create(int dietID) {
        init();
        QSqlQuery *createDayQuery = manager->createPreparedQuery("INSERT INTO days (name, diet_id) VALUES (:name, :id)");
        createDayQuery->bindValue(":name", name);
        createDayQuery->bindValue(":id", dietID);
        createDayQuery->exec();
        key = createDayQuery->lastInsertId().toInt();
    }

    void save() {
        QSqlQuery *savedayQuery = manager->createPreparedQuery("UPDATE days SET name = (:name) WHERE id = (:id)");
        savedayQuery->bindValue(":name", name);
        savedayQuery->bindValue(":id", key);
        savedayQuery->exec();
    }

    bool setName(const QString &_name) {
        if(name == _name) return false;
        name = _name;
        foodID->setLongDesc(name);
        foodID->setShrtDesc(name);
        save();
        return true;
    }

    void loadMeals() {
        QSqlQuery *getDaysQuery = manager->createPreparedQuery("SELECT id FROM meals WHERE day_id=(:id)");
        getDaysQuery->bindValue(":id", key);
        getDaysQuery->exec();
        mealList.clear();
        while (getDaysQuery->next())
        {
            Meal *meal = new Meal(getDaysQuery->value("id").toInt(), manager, false, day);
            mealList.append(meal);
        }
        mealsLoaded = true;
    }

    Meal* newMeal() {
        Meal* newMeal = new Meal(key, manager, true, day);
        mealList.append(newMeal);
        return newMeal;
    }

    Day* day{nullptr};
    DatabaseManager* manager{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    QList<Meal*> mealList{nullptr};
    bool mealsLoaded = false;
};

Day::Day(QObject *parent)
    : QObject(parent)
{}

Day::Day(int dbID, DatabaseManager *manager, bool isNew, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, dbID, isNew));
}

Day::~Day() {}

QString Day::name() const
{
    return implementation->name;
}

FoodItem* Day::foodAvgEq()
{
    return implementation->foodAvgEq;
}

FoodItem* Day::foodTotalEq()
{
    return implementation->foodTotalEq;
}

void Day::setName(const QString &name)
{
    if(implementation->setName(name)) emit nameChanged();
}

/*void Day::recalculate()
{
    implementation->createFoodEqs();
    emit foodEqChanged();
    emit foodAvgChanged();
    emit gmWgtChanged();
}*/

void Day::setKey(int key) {
    if (implementation->key != key) {
        implementation->key = key;
    }
}

int Day::key() {
    return implementation->key;
}

QQmlListProperty<Meal> Day::meals() {
    if (!implementation->mealsLoaded) {
        implementation->loadMeals();
        emit mealsChanged();
    }
    return QQmlListProperty<Meal>(this, implementation->mealList);
}

QVariant Day::newMeal() {
    Meal* meal = implementation->newMeal();
    emit mealsChanged();
    return QVariant::fromValue(meal);
}

}}
