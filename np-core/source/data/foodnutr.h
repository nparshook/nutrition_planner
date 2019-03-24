#ifndef FOODNUTR_H
#define FOODNUTR_H

#include <QObject>

class FoodNutr : public QObject
{
    Q_OBJECT
public:
    explicit FoodNutr(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FOODNUTR_H