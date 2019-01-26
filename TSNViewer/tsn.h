#ifndef TSN_H
#define TSN_H
#include <QDateTime>
#include <QDataStream>
#include <QVector>
#include <QDebug>

#include "files/info.h"

typedef struct
{
    qint8 second;
    qint8 minute;
    qint8 hour;
    qint8 day;
    qint8 mouth;
    qint8 year;
    qint8 week;
    qint8 century;
    qint16 BoxSN;
    qint16 NumberofScans;
    qint8 NumberofChannels;
    qint8 LengthofTS;
    qint8 StatusCodes;
    qint8 SaturationFlag;
    qint8 Reservedbit;
    qint8 Samplelength;
    qint16 SampleRate;
    qint8 SampleRateUnits;
    qint8 ClockStatus;
    qint32 ClockError;
    qint8 keep0;
    qint8 keep1;
    qint8 keep2;
    qint8 keep3;
    qint8 keep4;
    qint8 keep5;
    QVector < QVector < qint32 > > datas;
    QDateTime time;
}Ts;

inline qint32 bigbytesToInt32(QByteArray bytes)
{
    qint32 addr = bytes[0] & 0x000000FF;
    addr |= ((bytes[1] << 8) & 0x0000FF00);
    addr |= ((bytes[2] << 16) & 0x00FF0000);
    if( (unsigned char)bytes[2] > 0x7f)
        addr |= ((0xff << 24) & 0xFF000000);
    return addr;
}


class TSN
{
public:
    QString filepath;
    qint32 BoxSN;
    qint32 NumberofChannels;
    qint32 NumberofScans;
    qint32 SamplingRate;
    qint32 length_pre;
    qint32 tscount;

public:
    TSN();
    TSN(QString filename);
    void setFileName(QString filename);
    qint32 sampleRate();
    Ts getdata(qint32 index);
    qint32 segment();
    dataInfo getData(int index, int ch);
    QVector < dataInfo > getData(int index);
    qint32 scans();
    qint32 channels();
    QDateTime begintime();
    QDateTime endtime();
};

#endif // TSN_H
