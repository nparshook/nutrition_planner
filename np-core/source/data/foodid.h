#ifndef FOODID_H
#define FOODID_H

#include <QObject>

class FoodID : public QObject
{
    Q_OBJECT
public:
    explicit FoodID(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FOODID_H