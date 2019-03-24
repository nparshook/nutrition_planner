#ifndef FOODWGT_H
#define FOODWGT_H

#include <QObject>

class FoodWgt : public QObject
{
    Q_OBJECT
public:
    explicit FoodWgt(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FOODWGT_H