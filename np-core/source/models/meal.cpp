#include "meal.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QHash>

using namespace np::database;

namespace np {
namespace models {

class Meal::Implementation{
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
            QSqlQuery *createMealFoods = manager->createPreparedQuery("CREATE TABLE meal_foods (id INTEGER PRIMARY KEY AUTOINCREMENT, ndb_no TEXT, meal_id INTEGER, wgt_idx INTEGER, amt FLOAT)");
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
        createFoodEqs();
    }

    void save() {
        QSqlQuery *saveMealQuery = manager->createPreparedQuery("UPDATE meals SET name = (:name) WHERE id = (:id)");
        saveMealQuery->bindValue(":name", name);
        saveMealQuery->bindValue(":id", key);
        saveMealQuery->exec();
    }

    void saveFood(FoodItem* item) {
        QSqlQuery *saveFoodQuery = manager->createPreparedQuery("UPDATE meal_foods SET wgt_idx = (:idx), amt = (:amt) WHERE id = (:id)");
        saveFoodQuery->bindValue(":idx", item->weightIdx());
        saveFoodQuery->bindValue(":amt", item->amount());
        saveFoodQuery->bindValue(":id", foodTable[item]);
        saveFoodQuery->exec();
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
        QSqlQuery *saveMealFoodQuery = manager->createPreparedQuery("INSERT INTO meal_foods (ndb_no, meal_id, wgt_idx, amt) VALUES (:ndb_no, :id, :idx, :amt)");
        saveMealFoodQuery->bindValue(":ndb_no", item->foodID()->ndbNo());
        saveMealFoodQuery->bindValue(":id", key);
        saveMealFoodQuery->bindValue(":idx", item->weightIdx());
        saveMealFoodQuery->bindValue(":amt", item->amount());
        saveMealFoodQuery->exec();
        foodTable[item] = saveMealFoodQuery->lastInsertId().toInt();
        foodList.append(item);
        connectFood(item);
        createFoodEqs();
    }

    void loadFood() {
        QSqlQuery *getMealFoodsQuery = manager->createPreparedQuery("SELECT id, ndb_no, wgt_idx, amt FROM meal_foods WHERE meal_id=(:id)");
        getMealFoodsQuery->bindValue(":id", key);
        getMealFoodsQuery->exec();
        foodTable.clear();
        foodList.clear();
        while (getMealFoodsQuery->next())
        {
            FoodItem *item = searcher->getFoodByNdb(getMealFoodsQuery->value("ndb_no").toString());
            item->setAmount(getMealFoodsQuery->value("amt").toFloat());
            item->setWeightIdx(getMealFoodsQuery->value("wgt_idx").toInt());
            foodTable[item] = getMealFoodsQuery->value("id").toInt();
            foodList.append(item);
            connectFood(item);
        }
        foodLoaded = true;
        createFoodEqs();
    }

    void connectFood(FoodItem* item) {
        QObject::connect(item, &FoodItem::amountChanged, meal, [=](){saveFood(item); createFoodEqs();});
        QObject::connect(item, &FoodItem::weightIdxChanged, meal, [=](){saveFood(item); createFoodEqs();});
    }

    void createFoodEqs()
    {
        QHash<int, FoodNutr *> nutrsTable;

        float gmWgt = 0;

        for(int i = 0; i < foodList.size(); i++) {
            QList<FoodNutr *> nutrs = foodList[i]->nutrients();
            float scaleFactor = foodList[i]->scaleFactor();
            float amount = foodList[i]->amount();
            gmWgt += scaleFactor * amount;
            for(int j = 0; j < nutrs.size(); j++) {
                FoodNutr *nutr = nutrs[j];
                if(!nutrsTable.contains(nutr->nutrNo()))
                {
                   nutrsTable[nutr->nutrNo()] = new FoodNutr(nutr->nutrNo(),
                                                             nutr->nutrVal() * scaleFactor * amount / 100, nutr->nutrDesc(),
                                                             nutr->tagName(),
                                                             nutr->units());
                } else {
                    nutrsTable[nutr->nutrNo()]->setNutrVal(nutr->nutrVal() * scaleFactor * amount / 100 + nutrsTable[nutr->nutrNo()]->nutrVal());
                }
            }
        }

        //if(!amountSet) amount = gmWgt;
        FoodItem* totalItem = new FoodItem(meal);
        FoodItem* avgItem = new FoodItem(meal);
        totalItem->setFoodID(foodID);
        avgItem->setFoodID(foodID);
        //totalItem->setScaleFactor(amount / gmWgt);
        //avgItem->setScaleFactor(amount / gmWgt);
        QHashIterator<int, FoodNutr *> h(nutrsTable);
        while (h.hasNext()) {
            h.next();
            FoodNutr *nutr = h.value();
            totalItem->appendNutrient(nutr);
            avgItem->appendNutrient(new FoodNutr(nutr->nutrNo(),
                                                 nutr->nutrVal() / foodList.size(),
                                                 nutr->nutrDesc(),
                                                 nutr->tagName(),
                                                 nutr->units()));
        }

        foodTotalEq = totalItem;
        foodAvgEq = avgItem;
        meal->foodEqChanged();
    }

    void removeFood(FoodItem* item) {
        int id = foodTable.value(item);
        foodList.removeOne(item);
        foodTable.remove(item);
        deleteFood(id);
        createFoodEqs();
    }

    void deleteFood(int id) {
        QSqlQuery *delMealFoodQuery = manager->createPreparedQuery("DELETE FROM meal_foods WHERE id=(:id)");
        delMealFoodQuery->bindValue(":id", id);
        delMealFoodQuery->exec();
    }

    void remove() {
        QHashIterator<FoodItem*, int> h(foodTable);
        while (h.hasNext()) {
            h.next();
            deleteFood(h.value());
        }
        foodList.clear();
        foodTable.clear();
        QSqlQuery *delMealQuery = manager->createPreparedQuery("DELETE FROM meals WHERE id=(:id)");
        delMealQuery->bindValue(":id", key);
        delMealQuery->exec();
    }

    Meal* meal{nullptr};
    DatabaseManager* manager{nullptr};
    FoodSearch* searcher{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    bool foodLoaded = false;
    QList<FoodItem*> foodList;
    QHash<FoodItem*, int> foodTable;
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

void Meal::load()
{
    implementation->loadFood();
}

void Meal::removeFood(FoodItem *item)
{
    implementation->removeFood(item);
    emit foodsChanged();
}

void Meal::remove() {
    implementation->remove();
}
}}
