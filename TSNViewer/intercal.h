#ifndef INTERCAL_H
#define INTERCAL_H

#include <QObject>
#include "files/info.h"

class interCal : public QObject
{
    Q_OBJECT
public:
    explicit interCal(QObject *parent = 0);

public:
    QVector< double > ts;

signals:

public slots:

private:
    QVector<scatter> secDerv(QVector<scatter> info);//二阶求导
    double splint(QVector<scatter> org, QVector<scatter> sp, double freq);
    QVector <scatter> sortPoint(QVector <scatter> data);
    QVector <double> sortPoint(QVector <double> data);
public:
    QVector<scatter> spline(QVector<scatter> info, QVector<double> freqs);
    QVector<scatter_c> spline(QVector<scatter_c> info, QVector<double> freqs);
};

#endif // INTERCAL_H
