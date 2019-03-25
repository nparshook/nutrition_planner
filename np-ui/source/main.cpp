#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <controllers/mastercontroller.h>
#include <models/foodgrp.h>
#include <models/foodsearch.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    qmlRegisterType<np::controllers::MasterController>("NP", 1, 0, "MasterController");
    qmlRegisterType<np::models::FoodGrp>("NP", 1, 0, "FoodGrp");
    qmlRegisterType<np::models::FoodSearch>("NP", 1, 0, "FoodSearch");

    np::controllers::MasterController masterController;
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.rootContext()->setContextProperty("masterController", &masterController);
    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
