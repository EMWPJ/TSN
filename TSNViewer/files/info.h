#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include <math.h>
#include <QList>
#include <QFileInfo>
#include <QDebug>

typedef struct
{
    QDateTime begin;
    QDateTime end;
    double fre;
}fretime;

typedef struct
{
    double fre;
    float Amp;
    float pha;
}Info;

class tsInfo
{
public:
    bool exist;
    qint16 boxsn;
    QString tblpath;
    QString clbpath;
    QString tspath;
    QString hxsn;
    QString hysn;
    QString hzsn;
    QString hxpath;
    QString hypath;
    QString hzpath;
    tsInfo()
    {
        exist = false;
        boxsn = 0;
        tblpath.isEmpty();
        clbpath.isEmpty();
        tspath.isEmpty();
        hxsn.isEmpty();
        hysn.isEmpty();
        hzsn.isEmpty();
        hxpath.isEmpty();
        hypath.isEmpty();
        hzpath.isEmpty();
    }
};

class dataInfo
{
public:
    QVector< double > value;
    double samplerate;//hz
    QDateTime time;
    int sn;
    dataInfo()
    {
        sn = 0;
        value.clear();
        samplerate = 1;
        time = QDateTime::currentDateTime();
    }
};

class dataInfo_c
{
public:
    QVector< double > amp;
    QVector< double > pha;
    double samplerate;//hz
    QDateTime time;
    int sn;
    dataInfo_c()
    {
        sn = 0;
        amp.clear();
        pha.clear();
        samplerate = 1;
        time = QDateTime::currentDateTime();
    }
};

class complex
{
public:
    double real;
    double imag;
    complex()
    {
        real = 0;
        imag = 0;
    }
    void set(double r,double i)
    {
        real = r;
        imag = i;
    }
};


class tblInfo
{
public:
    QString name;
    QString tblpath;
    QString clbpath;
    QString hxclcpath;
    QString hyclcpath;
    QString hzclcpath;
    QString ts2path;
    QString ts3path;
    QString ts4path;
    QString ts5path;
    qint16 boxsn;
    QString hxsn;
    QString hysn;
    QString hzsn;
    double latitude;
    double longitude;
    tblInfo()
    {
        name.clear();
        tblpath.clear();
        clbpath.clear();
        hxclcpath.clear();
        hyclcpath.clear();
        hzclcpath.clear();
        ts2path.clear();
        ts3path.clear();
        ts4path.clear();
        ts5path.clear();
        boxsn = 0;
        hxsn.clear();
        hysn.clear();
        hzsn.clear();
        latitude = 0;
        longitude = 0;
    }
};

class measurePoint
{
public:
    QString name;
    double latitude;
    double longitude;
    QStringList files;
    measurePoint()
    {
        name.clear();
        latitude = 0;
        longitude = 0;
        files.clear();
    }
    QString contains(QString name)
    {
        for(int i=0;i<files.count();i++)
        {
            if(files.at(i).contains(name))
            {
                return files.at(i);
            }
        }
        return NULL;
    }
};

class scatter
{
public:
    double freq;
    double value;
    QDateTime time;
public:
    scatter()
    {
        freq = 0;
        value = 0;
    }

    scatter(double sfreq, double svalue)
    {
        freq = sfreq;
        value = svalue;
    }
};

class scatter_c
{
public:
    double freq;
    double amp;
    double pha;
    QDateTime time;
public:
    scatter_c()
    {
        freq = 0;
        amp = 0;
        pha = 0;
    }
    scatter_c(double sfreq,double samp,double spha)
    {
        freq = sfreq;
        amp = samp;
        pha = spha;
    }
};

class mtPoint
{
public:
    QString name;
    double value;
    double x;
    double y;
    QDateTime start;
    QDateTime end;
    QString province;

public:
    mtPoint()
    {
        name.clear();
        value=0;
        x=0;
        y=0;
    }
    mtPoint(double px,double py)
    {
        x = px;
        y = py;
    }
    mtPoint(QString text, double px, double py, double pvalue)
    {
        name = text;
        x = px;
        y = py;
        value = pvalue;
    }
};

#endif // INFO_H
