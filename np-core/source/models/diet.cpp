#include "diet.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>

using namespace np::database;

namespace np {
namespace models {

class Diet::Implementation {
public:

    Implementation(Diet* _diet, DatabaseManager* _manager, FoodSearch* _searcher)
        : diet(_diet), manager(_manager), searcher(_searcher)
    {
        name = "New Diet...";
        foodID = new FoodID("0", name, name, diet);
        create();
    }


    Implementation(Diet* _diet, DatabaseManager* _manager, FoodSearch* _searcher, int dbID)
        : diet(_diet), manager(_manager), searcher(_searcher)
    {

        name = "New Diet...";
        foodID = new FoodID("0", name, name, diet);
        key = dbID;
        load();
    }

    void init() {
        if(!manager->hasTable("diets")) {
            QSqlQuery *createDiets = manager->createPreparedQuery("CREATE TABLE diets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
            createDiets->exec();
        }
    }

    void load() {
        QSqlQuery *getDietQuery = manager->createPreparedQuery("SELECT name FROM diets WHERE id=(:id)");
        getDietQuery->bindValue(":id", key);
        getDietQuery->exec();
        getDietQuery->next();

        name = getDietQuery->value("name").toString();
    }

    void create() {
        init();
        QSqlQuery *createDietQuery = manager->createPreparedQuery("INSERT INTO diets (name) VALUES (:name)");
        createDietQuery->bindValue(":name", name);
        createDietQuery->exec();
        key = createDietQuery->lastInsertId().toInt();
    }

    void save() {
        QSqlQuery *saveDietQuery = manager->createPreparedQuery("UPDATE diets SET name = (:name) WHERE id = (:id)");
        saveDietQuery->bindValue(":name", name);
        saveDietQuery->bindValue(":id", key);
        saveDietQuery->exec();
    }

    bool setName(const QString &_name) {
        if(name == _name) return false;
        name = _name;
        foodID->setLongDesc(name);
        foodID->setShrtDesc(name);
        save();
        return true;
    }

    void loadDays() {
        QSqlQuery *getDaysQuery = manager->createPreparedQuery("SELECT id FROM days WHERE diet_id=(:id)");
        getDaysQuery->bindValue(":id", key);
        getDaysQuery->exec();
        dayList.clear();
        while (getDaysQuery->next())
        {
            Day *day = new Day(getDaysQuery->value("id").toInt(), manager, searcher, false, diet);
            dayList.append(day);
        }
        daysLoaded = true;
    }

    Day* newDay() {
        Day* newDay = new Day(key, manager, searcher, true, diet);
        dayList.append(newDay);
        return newDay;
    }

    Diet* diet{nullptr};
    DatabaseManager* manager{nullptr};
    FoodSearch* searcher{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    QList<Day*> dayList{nullptr};
    bool daysLoaded = false;
};

Diet::Diet(QObject *parent)
    : QObject(parent)
{}

Diet::Diet(DatabaseManager *manager, FoodSearch* searcher, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, searcher));
}

Diet::Diet(int dbID, DatabaseManager *manager, FoodSearch* searcher, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, searcher, dbID));
}

Diet::~Diet() {}

QString Diet::name() const
{
    return implementation->name;
}

FoodItem* Diet::foodAvgEq()
{
    return implementation->foodAvgEq;
}

FoodItem* Diet::foodTotalEq()
{
    return implementation->foodTotalEq;
}

void Diet::setName(const QString &name)
{
    if(implementation->setName(name)) emit nameChanged();
}

/*void Diet::recalculate()
{
    implementation->createFoodEqs();
    emit foodEqChanged();
    emit foodAvgChanged();
    emit gmWgtChanged();
}*/

void Diet::setKey(int key) {
    if (implementation->key != key) {
        implementation->key = key;
    }
}

int Diet::key() {
    return implementation->key;
}

QQmlListProperty<Day> Diet::days() {
    if (!implementation->daysLoaded) {
        implementation->loadDays();
        emit daysChanged();
    }
    return QQmlListProperty<Day>(this, implementation->dayList);
}

QVariant Diet::newDay() {
    Day* day = implementation->newDay();
    emit daysChanged();
    return QVariant::fromValue(day);
}
}}
