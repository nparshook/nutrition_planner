#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <controllers/mastercontroller.h>
#include <models/foodid.h>
#include <models/foodgrp.h>
#include <models/foodsearch.h>
#include <models/foodwgt.h>
#include <models/fooditem.h>
#include <models/fooditemlist.h>
#include <models/diet.h>
#include <models/day.h>
#include <models/meal.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<np::controllers::MasterController>("NP", 1, 0, "MasterController");

    qmlRegisterType<np::models::FoodWgt>("NP", 1, 0, "FoodWgt");
    qmlRegisterType<np::models::FoodGrp>("NP", 1, 0, "FoodGrp");
    qmlRegisterType<np::models::FoodID>("NP", 1, 0, "FoodID");
    qmlRegisterType<np::models::FoodItem>("NP", 1, 0, "FoodItem");
    qmlRegisterType<np::models::FoodItemList>("NP", 1, 0, "FoodItemList");
    qmlRegisterType<np::models::FoodSearch>("NP", 1, 0, "FoodSearch");
    qmlRegisterType<np::models::Diet>("NP", 1, 0, "Diet");
    qmlRegisterType<np::models::Day>("NP", 1, 0, "Day");
    qmlRegisterType<np::models::Meal>("NP", 1, 0, "Meal");


    np::controllers::MasterController masterController;

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("masterController", &masterController);
    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
