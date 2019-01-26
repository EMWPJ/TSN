#ifndef CLC_H
#define CLC_H
#include <QDateTime>
#include <QVector>
#include "files/info.h"

typedef struct
{
    qint16 fcn;//频带号
    float baseband;//基频
    qint16 harmonic;//谐波次数
    float rawreal;
    float rawimag;
    float err;
    float FC;//拐角频率
    float real;//校正实部
    float imag;//校正虚部
}clcline;

class CLC
{
public:
    QString filepath;
    QDateTime time;
    qint8 week;
    qint8 century;
    qint16 SNUM;
    qint16 NCHN;
    qint16 unknow1;
    qint16 STAT;
    qint16 unknow2;
    qint16 unknow3;
    QString HW;
    QString VER;
    qint32 TCMB;
    qint32 TALS;
    qint32 LFRQ;
    qint32 V5SR;
    qint32 LPFR;
    QString HSN;
    double HATT;
    double HNOM;
    double CPHC;
    QString unknow4;
    QVector < clcline > datas;

public:
    QVector < Info > Infos;

public:
    CLC();
    CLC(QString filename);
    void setFileName(QString filename);
    bool read();
    qint16 nchn();
    QDateTime datatime();
    QVector < Info > information();
};

#endif // CLC_H
