#include "day.h"
#include <QDebug>
#include <QList>
#include <QSqlQuery>

using namespace np::database;

namespace np {
namespace models {

class Day::Implementation {
public:

    Implementation(Day* _day, DatabaseManager* _manager, FoodSearch* _searcher, int ID, bool isNew)
        : day(_day), manager(_manager), searcher(_searcher)
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
        qDebug() << "New Day";
        init();
        QSqlQuery *createDayQuery = manager->createPreparedQuery("INSERT INTO days (name, diet_id) VALUES (:name, :id)");
        createDayQuery->bindValue(":name", name);
        createDayQuery->bindValue(":id", dietID);
        createDayQuery->exec();
        key = createDayQuery->lastInsertId().toInt();
        createFoodEqs();
        qDebug() << "Created";
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
            Meal *meal = new Meal(getDaysQuery->value("id").toInt(), manager, searcher, false, day);
            mealList.append(meal);
            connectMeal(meal);
            meal->load();
        }
        mealsLoaded = true;
        createFoodEqs();
    }

    Meal* newMeal() {
        Meal* newMeal = new Meal(key, manager, searcher, true, day);
        qDebug() << "Meal Made";
        mealList.append(newMeal);
        connectMeal(newMeal);
        createFoodEqs();
        qDebug() << "Meal Added";
        return newMeal;
    }

    void connectMeal(Meal* meal) {
        QObject::connect(meal, &Meal::foodEqChanged, day, [=](){createFoodEqs();});
        QObject::connect(meal, &Meal::foodAvgChanged, day, [=](){createFoodEqs();});
    }

    void createFoodEqs()
    {
        QHash<int, FoodNutr *> nutrsTable;

        float gmWgt = 0;
        qDebug() << mealList.size();
        for(int i = 0; i < mealList.size(); i++) {
            FoodItem* item = mealList[i]->foodTotalEq();
            QList<FoodNutr *> nutrs = item->nutrients();
            float scaleFactor = item->scaleFactor();
            float amount = item->amount();
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
        qDebug() << "Meals Summed";
        FoodItem* totalItem = new FoodItem(day);
        FoodItem* avgItem = new FoodItem(day);
        totalItem->setFoodID(foodID);
        avgItem->setFoodID(foodID);
        QHashIterator<int, FoodNutr *> h(nutrsTable);
        while (h.hasNext()) {
            h.next();
            FoodNutr *nutr = h.value();
            totalItem->appendNutrient(nutr);
            avgItem->appendNutrient(new FoodNutr(nutr->nutrNo(),
                                                 nutr->nutrVal() / mealList.size(),
                                                 nutr->nutrDesc(),
                                                 nutr->tagName(),
                                                 nutr->units()));
        }

        foodTotalEq = totalItem;
        foodAvgEq = avgItem;
        day->foodEqChanged();
        day->foodAvgChanged();
    }

    void removeMeal(Meal* meal) {
        mealList.removeOne(meal);
        meal->remove();
        createFoodEqs();
    }

    Day* day{nullptr};
    DatabaseManager* manager{nullptr};
    FoodSearch* searcher{nullptr};
    QString name;
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    QList<Meal*> mealList;
    bool mealsLoaded = false;
};

Day::Day(QObject *parent)
    : QObject(parent)
{}

Day::Day(int dbID, DatabaseManager *manager, FoodSearch* searcher, bool isNew, QObject *parent)
    : QObject(parent)
{
    implementation.reset(new Implementation(this, manager, searcher, dbID, isNew));
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

void Day::load()
{
    implementation->loadMeals();
}

void Day::removeMeal(Meal *meal) {
    implementation->removeMeal(meal);
}
}}
