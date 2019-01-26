#include "clb.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "math.h"

CLB::CLB()
{
    filepath.clear();
}

CLB::CLB(QString filename)
{
    filepath = filename;
    read();
}

void CLB::setFileName(QString filename)
{
    filepath = filename;
    read();
}

bool CLB::read()
{
    if(filepath.isEmpty())
    {
        return false;
    }
    QFile clbFile(filepath);
    if(!clbFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream clbRead(&clbFile);
    qint8 sec,min,hour,day,mon,year;
    clbRead.setByteOrder(QDataStream::BigEndian);
    clbRead>>sec>>min>>hour>>day>>mon>>year;
    time = QDateTime(QDate(year+2000,mon,day),QTime(hour,min,sec));
    clbRead>>week>>century;
    clbRead.setByteOrder(QDataStream::LittleEndian);
    clbRead>>SNUM>>NCHN>>unknow1>>STAT>>unknow2>>unknow3;
    char temp12[12];
    clbRead.readRawData(temp12,12);
    HW=QString(temp12);
    clbRead.readRawData(temp12,12);
    VER=QString(temp12);
    clbRead>>TCMB>>TALS>>LFRQ>>V5SR>>LPFR;
    char temp68[68];
    clbRead.readRawData(temp68,68);
    clbline current;
    clbRead.setFloatingPointPrecision(QDataStream::SinglePrecision);
    while(!clbRead.atEnd())
    {
        clbRead>>current.fcn>>current.baseband>>current.harmonic;
        for(int i=0;i<NCHN;i++)
        {
            clbRead>>current.rawreal>>current.rawimag
                    >>current.err>>current.FC>>current.real>>current.imag;
            datas<<current;
        }
        if(current.baseband>1200)
        {
            break;
        }
    }
    clbFile.close();
    return true;
}
qint16 CLB::boxsn()
{
    return SNUM;
}

qint16 CLB::nchn()
{
    return NCHN;
}

QDateTime CLB::datatime()
{
    return time;
}

QVector < Info > CLB::information()
{
    QVector < Info > infor;
    int count = datas.count();
    for(int i=0;i<count;i++)
    {
        if(datas.at(i).baseband>400)
        {
           break;
        }
        Info current;
        current.fre = datas.at(i).baseband;//* datas.at(i).harmonic;
        current.Amp = sqrt(datas.at(i).real * datas.at(i).real + datas.at(i).imag * datas.at(i).imag);
        current.pha = atan(datas.at(i).imag / datas.at(i).real) * 180 / 3.1415926535897932384626;
        infor<<current;
    }
//    Info current;
//    current.fre = infor.at(0).fre / 1.414;
//    current.Amp = infor.at(0).Amp;
//    current.pha = infor.at(0).pha;
//    infor<<current;
//    current.fre = infor.at(1).fre / 1.414;
//    current.Amp = infor.at(1).Amp;
//    current.pha = infor.at(1).pha;
//    infor<<current;
//    current.fre = infor.at(2).fre / 1.414;
//    current.Amp = infor.at(2).Amp;
//    current.pha = infor.at(2).pha;
//    infor<<current;
//    current.fre = infor.at(3).fre / 1.414;
//    current.Amp = infor.at(3).Amp;
//    current.pha = infor.at(3).pha;
//    infor<<current;
//    current.fre = infor.at(4).fre / 1.414;
//    current.Amp = infor.at(4).Amp;
//    current.pha = infor.at(4).pha;
//    infor<<current;
//    for(int i=0;i<10;i++)
//    {
//        current.fre = infor.at(infor.count()-5).fre / 1.414;
//        current.Amp = infor.at(infor.count()-5).Amp;
//        current.pha = infor.at(infor.count()-5).pha;
//        infor<<current;
//        current.fre = infor.at(infor.count()-5).fre / 1.414;
//        current.Amp = infor.at(infor.count()-5).Amp;
//        current.pha = infor.at(infor.count()-5).pha;
//        infor<<current;
//        current.fre = infor.at(infor.count()-5).fre / 1.414;
//        current.Amp = infor.at(infor.count()-5).Amp;
//        current.pha = infor.at(infor.count()-5).pha;
//        infor<<current;
//        current.fre = infor.at(infor.count()-5).fre / 1.414;
//        current.Amp = infor.at(infor.count()-5).Amp;
//        current.pha = infor.at(infor.count()-5).pha;
//        infor<<current;
//        current.fre = infor.at(infor.count()-5).fre / 1.414;
//        current.Amp = infor.at(infor.count()-5).Amp;
//        current.pha = infor.at(infor.count()-5).pha;
//        infor<<current;
//    }
    return infor;
}


