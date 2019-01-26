#include "clc.h"
#include <QFile>
#include <QDataStream>

#include "math.h"

CLC::CLC()
{
    filepath.clear();
}

CLC::CLC(QString filename)
{
    filepath = filename;
    read();
}

void CLC::setFileName(QString filename)
{
    filepath = filename;
    read();
}

bool CLC::read()
{
    if(filepath.isEmpty())
    {
        return false;
    }
    QFile clcFile(filepath);
    if(!clcFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream clcRead(&clcFile);
    qint8 sec,min,hour,day,mon,year;
    clcRead.setByteOrder(QDataStream::BigEndian);
    clcRead>>sec>>min>>hour>>day>>mon>>year;
    time = QDateTime(QDate(year+2000,mon,day),QTime(hour,min,sec));
    clcRead>>week>>century;
    clcRead.setByteOrder(QDataStream::LittleEndian);
    clcRead>>SNUM>>NCHN>>unknow1>>STAT>>unknow2>>unknow3;
    char temp12[12];
    clcRead.readRawData(temp12,12);
    HW=QString(temp12);
    clcRead.readRawData(temp12,12);
    VER=QString(temp12);
    clcRead>>TCMB>>TALS>>LFRQ>>V5SR>>LPFR;
    clcRead.readRawData(temp12,12);
    HSN=QString(temp12);
    clcRead>>HATT>>HNOM>>CPHC;
    char temp32[32];
    clcRead.readRawData(temp32,32);
    clcline current;
    clcRead.setFloatingPointPrecision(QDataStream::SinglePrecision);
    while(!clcRead.atEnd())
    {
        clcRead>>current.fcn>>current.baseband>>current.harmonic;
        for(int i=0;i<NCHN;i++)
        {
            clcRead>>current.rawreal>>current.rawimag
                    >>current.err>>current.FC>>current.real>>current.imag;
            datas<<current;
        }
    }
    clcFile.close();
    return true;
}

qint16 CLC::nchn()
{
    return NCHN;
}

QDateTime CLC::datatime()
{
    return time;
}

QVector < Info > CLC::information()
{
    QVector < Info > infor;
    int count = datas.count();
    for(int i=0;i<count;i++)
    {
        Info current;
        if(datas.at(i).baseband>400)
        {
           break;
        }
        current.fre = datas.at(i).baseband ;//* double(datas.at(i).harmonic);
        current.Amp = sqrt(datas.at(i).rawreal * datas.at(i).rawreal + datas.at(i).rawimag * datas.at(i).rawimag);
        current.pha = atan(datas.at(i).rawimag / datas.at(i).rawreal) * 180 / 3.1415926535897932384626;
        infor<<current;
    }
//    Info current;
//    current.fre = infor.begin()->fre / 1.414;
//    current.Amp = infor.begin()->Amp;
//    current.pha = infor.begin()->pha;
//    infor<<current;
//    for(int i=count;i<count+10;i++)
//    {
//        current.fre = infor.at(infor.count()-1).fre / 1.414;
//        current.Amp = infor.at(infor.count()-1).Amp;
//        current.pha = infor.at(infor.count()-1).pha;
//        infor<<current;
//    }
    return infor;
}

