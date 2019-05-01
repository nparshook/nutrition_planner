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
            QSqlQuery *createDiets = manager->createPreparedQuery("CREATE TABLE diets (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, ls_cd TEXT)");
            createDiets->exec();
        }
    }

    void load() {
        QSqlQuery *getDietQuery = manager->createPreparedQuery("SELECT name, ls_cd FROM diets WHERE id=(:id)");
        getDietQuery->bindValue(":id", key);
        getDietQuery->exec();
        getDietQuery->next();

        name = getDietQuery->value("name").toString();
        ls_cd = getDietQuery->value("ls_cd").toString();
        nutrReqsTable = searcher->getNutrReqs(ls_cd);
    }

    void create() {
        init();
        QSqlQuery *createDietQuery = manager->createPreparedQuery("INSERT INTO diets (name, ls_cd) VALUES (:name, :ls_cd)");
        createDietQuery->bindValue(":name", name);
        createDietQuery->bindValue(":ls_cd", ls_cd);
        createDietQuery->exec();

        key = createDietQuery->lastInsertId().toInt();
        nutrReqsTable = searcher->getNutrReqs(ls_cd);
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
            connectDay(day);
            day->load();
        }
        daysLoaded = true;
        createFoodEqs();
    }

    Day* newDay() {
        Day* newDay = new Day(key, manager, searcher, true, diet);
        dayList.append(newDay);
        connectDay(newDay);
        createFoodEqs();
        return newDay;
    }

    void connectDay(Day* day) {
        QObject::connect(day, &Day::foodEqChanged, diet, [=](){createFoodEqs();});
        QObject::connect(day, &Day::foodAvgChanged, diet, [=](){createFoodEqs();});
    }

    void createFoodEqs()
    {
        QHash<int, FoodNutr *> nutrsTable;

        float gmWgt = 0;

        for(int i = 0; i < dayList.size(); i++) {
            FoodItem* item = dayList[i]->foodTotalEq();
            QList<FoodNutr *> nutrs = item->nutrients();
            float scaleFactor = item->scaleFactor();
            float amount = item->amount();
            gmWgt += scaleFactor * amount;
            for(int j = 0; j < nutrs.size(); j++) {
                FoodNutr *nutr = nutrs[j];
                if(!nutrsTable.contains(nutr->nutrNo()))
                {
                    if(nutrReqsTable.contains(nutr->nutrNo()) || (nutr->nutrNo() >= 203 && nutr->nutrNo() <= 205) ){
                        nutrsTable[nutr->nutrNo()] = new FoodNutr(nutr->nutrNo(),
                                                                 nutr->nutrVal() * scaleFactor * amount / 100, nutr->nutrDesc(),
                                                                 nutr->tagName(),
                                                                 nutr->units());
                    }
                } else {
                    nutrsTable[nutr->nutrNo()]->setNutrVal(nutr->nutrVal() * scaleFactor * amount / 100 + nutrsTable[nutr->nutrNo()]->nutrVal());
                }
            }
        }

        FoodItem* totalItem = new FoodItem(diet);
        FoodItem* avgItem = new FoodItem(diet);
        totalItem->setFoodID(foodID);
        avgItem->setFoodID(foodID);
        QHashIterator<int, FoodNutr *> h(nutrsTable);
        while (h.hasNext()) {
            h.next();
            FoodNutr *nutr = h.value();
            totalItem->appendNutrient(nutr);
            avgItem->appendNutrient(new FoodNutr(nutr->nutrNo(),
                                                 nutr->nutrVal() / dayList.size(),
                                                 nutr->nutrDesc(),
                                                 nutr->tagName(),
                                                 nutr->units()));
        }

        foodTotalEq = totalItem;
        foodAvgEq = avgItem;
    }

    void removeDay(Day* day) {
        dayList.removeOne(day);
        day->remove();
        createFoodEqs();
    }

    void remove() {
        for(int i = 0; i < dayList.count(); i++) {
            dayList[i]->remove();
        }
        dayList.clear();
        QSqlQuery *delDietQuery = manager->createPreparedQuery("DELETE FROM diets WHERE id=(:id)");
        delDietQuery->bindValue(":id", key);
        delDietQuery->exec();
    }

    float getNutrReq(int nutr_no) {
        if(nutrReqsTable.contains(nutr_no)) {
            return nutrReqsTable[nutr_no];
        }
        return -1.0;
    }

    Diet* diet{nullptr};
    DatabaseManager* manager{nullptr};
    FoodSearch* searcher{nullptr};
    QString name;
    QString ls_cd = "2";
    int key;
    FoodItem* foodTotalEq{nullptr};
    FoodItem* foodAvgEq{nullptr};
    FoodID *foodID{nullptr};
    QList<Day*> dayList{nullptr};
    bool daysLoaded = false;
    QHash<int, float> nutrReqsTable;
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

void Diet::removeDay(Day *day)
{
    implementation->removeDay(day);
}

void Diet::remove() {
    implementation->remove();
}

float Diet::getNutrReq(int nutr_no) {
    return implementation->getNutrReq(nutr_no);
}
}}
