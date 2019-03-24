#ifndef FOODDATABASECONTROLLER_H
#define FOODDATABASECONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QDir>
#include <QQmlListProperty>

#include "../data/foodgrp.h"

namespace np {
namespace controllers {

class FoodDatabaseController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<np::controllers::FoodGrp> ui_foodGrps READ foodGrps)

public:
    FoodDatabaseController(QObject* parent = nullptr, const QString& databaseName = QDir::currentPath() + "/usda.db");
    ~FoodDatabaseController();
    bool isReady();
    QQmlListProperty<FoodGrp> foodGrps();

private:
    class Implementation;
    QScopedPointer<Implementation> implementation;
};
}}
#endif // FOODDATABASECONTROLLER_H
