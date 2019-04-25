#include "meal.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>

using namespace np::database;

namespace np {
namespace models {

class Meal::Implementation {
public:

    Implementation(Meal* _meal, DatabaseManager* _manager, FoodSearch* _searcher, int ID, bool isNew)
        : meal(_meal), manager(_manager), searcher(_searcher)
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
        if(!manager->hasTable("meal_foods")) {
            QSqlQuery *createMealFoods = manager->createPreparedQuery("CREATE TABLE meal_foods (id INTEGER PRIMARY KEY AUTOINCREMENT, ndb_no TEXT, meal_id INTEGER)");
            createMealFoods->exec();
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

    void appendFood(FoodItem *item) {
        QSqlQuery *saveMealFoodQuery = manager->createPreparedQuery("INSERT INTO meal_foods (ndb_no, meal_id) VALUES (:ndb_no, :id)");
        saveMealFoodQuery->bindValue(":ndb_no", item->foodID()->ndbNo());
        saveMealFoodQuery->bindValue(":id", key);
        saveMealFoodQuery->exec();
        qDebug() << "IM SURE";
        qDebug() << saveMealFoodQuery->lastError();
        qDebug() << saveMealFoodQuery->lastQuery();
        qDebug() << saveMealFoodQuery->boundValues();
        foodList.append(item);
    }

    void loadFood() {
        QSqlQuery *getMealFoodsQuery = manager->createPreparedQuery("SELECT ndb_no FROM meal_foods WHERE meal_id=(:id)");
        getMealFoodsQuery->bindValue(":id", key);
        getMealFoodsQuery->exec();
        foodList.clear();
        qDebug() << "Getting Foods";
        qDebug() << key;
        qDebug() << getMealFoodsQuery->lastError();
        while (getMealFoodsQuery->next())
        {
            FoodItem *item = searcher->getFoodByNdb(getMealFoodsQuery->value("ndb_no").toString());
            foodList.append(item);
            qDebug() << "ID";
            qDebug() << item->foodID()->longDesc();
        }
        foodLoaded = true;
    }

    Meal* meal{nullptr};
    DatabaseManager* manager{nullptr};
    FoodSearch* searcher{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    QList<FoodItem*> foodList{nullptr};
    bool foodLoaded = false;

};

Meal::Meal(QObject *parent)
    : QObject(parent)
{}

Meal::Meal(int dbID, DatabaseManager *manager, FoodSearch* searcher, bool isNew, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, searcher, dbID, isNew));
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

void Meal::appendFood(FoodItem *item)
{
    implementation->appendFood(item);
    emit foodsChanged();
}

QQmlListProperty<FoodItem> Meal::foods() {
    if (!implementation->foodLoaded) {
        implementation->loadFood();
        emit foodsChanged();
    }
    return QQmlListProperty<FoodItem>(this, implementation->foodList);
}

}}
