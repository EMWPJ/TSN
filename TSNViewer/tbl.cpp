#include "tbl.h"
#include <QDebug>

TBL::TBL()
{
    filePath.clear();
}

TBL::TBL(QString filename)
{
    filePath = filename;
    read();
}

void TBL::setFileName(QString filename)
{
    filePath = filename;
    read();
}

bool TBL::write()
{
    if(filePath.isEmpty())
    {
        return false;
    }
    QFile tblFile(filePath);
    if(!tblFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    QDataStream tblWrite(&tblFile);
    for(int i=0;i<tblLines.count();i++)
    {
        QString attribute = QString(tblLines.at(i).name);
        Tbl_Line current = tblLines.at(i);
        if(attribute=="INIT")
        {
            current.setUnknow(INIT);
        }
        else if(attribute=="RQST")
        {
            current.setUnknow(RQST);
        }
        else if(attribute=="MODE")
        {
            current.setUnknow(MODE);
        }
        else if(attribute=="AQST")
        {
            current.setUnknow(AQST);
        }
        else if(attribute=="SNUM")
        {
            current.setUnknow(SNUM);
        }
        else if(attribute=="VER")
        {
            current.setUnknow(VER);
        }
        else if(attribute=="HW")
        {
            current.setUnknow(HW);
        }
        else if(attribute=="SITE")
        {
            current.setUnknow(SITE);
        }
        else if(attribute=="CMPY")
        {
            current.setUnknow(CMPY);
        }
        else if(attribute=="SRVY")
        {
            current.setUnknow(SRVY);
        }
        else if(attribute=="ATYP")
        {
            current.setUnknow(ATYP);
        }
        else if(attribute=="NREF")
        {
            current.setUnknow(NREF);
        }
        else if(attribute=="FILE")
        {
            current.setUnknow(FILE);
        }
        else if(attribute=="NUTC")
        {
            current.setUnknow(NUTC);
        }
        else if(attribute=="STIM")
        {
            current.setUnknow(STIM);
        }
        else if(attribute=="ETIM")
        {
            current.setUnknow(ETIM);
        }
        else if(attribute=="HTIM")
        {
            current.setUnknow(HTIM);
        }
        else if(attribute=="ETMH")
        {
            current.setUnknow(ETMH);
        }
        else if(attribute=="HSMP")
        {
            current.setUnknow(HSMP);
        }
        else if(attribute=="SGIN")
        {
            current.setUnknow(SGIN);
        }
        else if(attribute=="EGN")
        {
            current.setUnknow(EGN);
        }
        else if(attribute=="HGN")
        {
            current.setUnknow(HGN);
        }
        else if(attribute=="ACDC")
        {
            current.setUnknow(ACDC);
        }
        else if(attribute=="ACDH")
        {
            current.setUnknow(ACDH);
        }
        else if(attribute=="LPFR")
        {
            current.setUnknow(LPFR);
        }
        else if(attribute=="LFRQ")
        {
            current.setUnknow(LFRQ);
        }
        else if(attribute=="V5SR")
        {
            current.setUnknow(V5SR);
        }
        else if(attribute=="L3NS")
        {
            current.setUnknow(L3NS);
        }
        else if(attribute=="L4NS")
        {
            current.setUnknow(L4NS);
        }
        else if(attribute=="SRL3")
        {
            current.setUnknow(SRL3);
        }
        else if(attribute=="SRL4")
        {
            current.setUnknow(SRL4);
        }
        else if(attribute=="SRL5")
        {
            current.setUnknow(SRL5);
        }
        else if(attribute=="CHEX")
        {
            current.setUnknow(CHEX);
        }
        else if(attribute=="CHEY")
        {
            current.setUnknow(CHEY);
        }
        else if(attribute=="CHEZ")
        {
            current.setUnknow(CHEZ);
        }
        else if(attribute=="CHHX")
        {
            current.setUnknow(CHHX);
        }
        else if(attribute=="CHHY")
        {
            current.setUnknow(CHHY);
        }
        else if(attribute=="CHHZ")
        {
            current.setUnknow(CHHZ);
        }
        else if(attribute=="EXAC")
        {
            current.setUnknow(EXAC);
        }
        else if(attribute=="EXDC")
        {
            current.setUnknow(EXDC);
        }
        else if(attribute=="EYAC")
        {
            current.setUnknow(EYAC);
        }
        else if(attribute=="EYDC")
        {
            current.setUnknow(EYDC);
        }
        else if(attribute=="HXAC")
        {
            current.setUnknow(HXAC);
        }
        else if(attribute=="HXDC")
        {
            current.setUnknow(HXDC);
        }
        else if(attribute=="HYAC")
        {
            current.setUnknow(HYAC);
        }
        else if(attribute=="HYDC")
        {
            current.setUnknow(HYDC);
        }
        else if(attribute=="HZAC")
        {
            current.setUnknow(HZAC);
        }
        else if(attribute=="HZDC")
        {
            current.setUnknow(HZDC);
        }
        else if(attribute=="EXR")
        {
            current.setUnknow(EXR);
        }
        else if(attribute.left(3)=="EYR")
        {
            current.setUnknow(EYR);
        }
        else if(attribute=="EAZM")
        {
            current.setUnknow(EAZM);
        }
        else if(attribute=="EXLN")
        {
            current.setUnknow(EXLN);
        }
        else if(attribute=="EYLN")
        {
            current.setUnknow(EYLN);
        }
        else if(attribute=="HAZM")
        {
            current.setUnknow(HAZM);
        }
        else if(attribute=="HXSN")
        {
            current.setUnknow(HXSN);
        }
        else if(attribute=="HYSN")
        {
            current.setUnknow(HYSN);
        }
        else if(attribute=="HZSN")
        {
            current.setUnknow(HZSN);
        }
        else if(attribute=="TOTL")
        {
            current.setUnknow(TOTL);
        }
        else if(attribute=="BADR")
        {
            current.setUnknow(BADR);
        }
        else if(attribute=="SATR")
        {
            current.setUnknow(SATR);
        }
        else if(attribute=="NSAT")
        {
            current.setUnknow(NSAT);
        }
        else if(attribute=="LFIX")
        {
            current.setUnknow(LFIX);
        }
        else if(attribute=="CLST")
        {
            current.setUnknow(CLST);
        }
        else if(attribute=="OCTR")
        {
            current.setUnknow(OCTR);
        }
        else if(attribute=="TERR")
        {
            current.setUnknow(TERR);
        }
        else if(attribute=="ELEV")
        {
            current.setUnknow(ELEV);
        }
        else if(attribute=="LATG")
        {
            current.setUnknow(LATG);
        }
        else if(attribute=="LNGG")
        {
            current.setUnknow(LNGG);
        }
        else if(attribute=="XDOS")
        {
            current.setUnknow(XDOS);
        }
        else if(attribute=="CALS")
        {
            current.setUnknow(CALS);
        }
        else if(attribute=="CCLS")
        {
            current.setUnknow(CCLS);
        }
        else if(attribute=="CALR")
        {
            current.setUnknow(CALR);
        }
        else if(attribute=="CCLT")
        {
            current.setUnknow(CCLT);
        }
        else if(attribute=="CFMN")
        {
            current.setUnknow(CFMN);
        }
        else if(attribute=="CFMX")
        {
            current.setUnknow(CFMX);
        }
        else if(attribute=="CCMN")
        {
            current.setUnknow(CCMN);
        }
        else if(attribute=="CCMX")
        {
            current.setUnknow(CCMX);
        }
        else if(attribute=="HATT")
        {
            current.setUnknow(HATT);
        }
        else if(attribute=="HNOM")
        {
            current.setUnknow(HNOM);
        }
        else if(attribute=="HAMP")
        {
            current.setUnknow(HAMP);
        }
        else if(attribute=="TCMB")
        {
            current.setUnknow(TCMB);
        }
        else if(attribute=="TALS")
        {
            current.setUnknow(TALS);
        }
        else if(attribute=="CPHC")
        {
            current.setUnknow(CPHC);
        }
        else if(attribute=="BAT1")
        {
            current.setUnknow(BAT1);
        }
        else if(attribute=="BAT2")
        {
            current.setUnknow(BAT2);
        }
        else if(attribute=="BAT3")
        {
            current.setUnknow(BAT3);
        }
        else if(attribute=="TEMP")
        {
            current.setUnknow(TEMP);
        }
        else if(attribute=="GFPG")
        {
            current.setUnknow(GFPG);
        }
        else if(attribute=="FFPG")
        {
            current.setUnknow(FFPG);
        }
        else if(attribute.left(3)=="DSP")
        {
            current.setUnknow(DSP);
        }
        else if(attribute=="DISK")
        {
            current.setUnknow(DISK);
        }
        else if(attribute=="CPTH")
        {
            current.setUnknow(CPTH);
        }
        else if(attribute=="DPTH")
        {
            current.setUnknow(DPTH);
        }
        else if(attribute=="TXPR")
        {
            current.setUnknow(TXPR);
        }
        else
        {
            qDebug()<<attribute;
        }
        tblWrite<<current;
    }
    qDebug()<<"----------TBL---------";
    tblFile.close();
    return true;
}

bool TBL::read()
{
    if(filePath.isEmpty())
    {
        return false;
    }
    QFile tblFile(filePath);
    if(!tblFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream tblRead(&tblFile);
    tblLines.clear();
    while(!tblRead.atEnd())
    {
        Tbl_Line tblline;
        tblRead>>tblline;
        tblLines<<tblline;
        QString attribute = QString(tblline.name);
        if(attribute=="INIT")
        {
            memcpy(&INIT, tblline.unknow,4);
        }
        else if(attribute=="RQST")
        {
            memcpy(&RQST, tblline.unknow,4);
        }
        else if(attribute=="MODE")
        {
            memcpy(&MODE, tblline.unknow,4);
        }
        else if(attribute=="AQST")
        {
            memcpy(&AQST, tblline.unknow,4);
        }
        else if(attribute=="SNUM")
        {
            memcpy(&SNUM, tblline.unknow,4);
        }
        else if(attribute=="VER")
        {
            VER = QString(tblline.unknow);
        }
        else if(attribute=="HW")
        {
            HW = QString(tblline.unknow);
        }
        else if(attribute=="SITE")
        {
            SITE = QString(tblline.unknow);
        }
        else if(attribute=="CMPY")
        {
            CMPY = QString(tblline.unknow);
        }
        else if(attribute=="SRVY")
        {
            SRVY = QString(tblline.unknow);
        }
        else if(attribute=="ATYP")
        {
            memcpy(&ATYP, tblline.unknow,4);
        }
        else if(attribute=="NREF")
        {
            memcpy(&NREF, tblline.unknow,4);
        }
        else if(attribute=="FILE")
        {
            FILE = QString(tblline.unknow);
        }
        else if(attribute=="NUTC")
        {
            NUTC = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="STIM")
        {
            STIM = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="ETIM")
        {
            ETIM = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="HTIM")
        {
            HTIM = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="ETMH")
        {
            ETMH = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="HSMP")
        {
            memcpy(&HSMP, tblline.unknow,4);
        }
        else if(attribute=="SGIN")
        {
            memcpy(&SGIN, tblline.unknow,4);
        }
        else if(attribute=="EGN")
        {
            memcpy(&EGN, tblline.unknow,4);
        }
        else if(attribute=="HGN")
        {
            memcpy(&HGN, tblline.unknow,4);
        }
        else if(attribute=="ACDC")
        {
            memcpy(&ACDC, tblline.unknow,4);
        }
        else if(attribute=="ACDH")
        {
            memcpy(&ACDH, tblline.unknow,4);
        }
        else if(attribute=="LPFR")
        {
            memcpy(&LPFR, tblline.unknow,4);
        }
        else if(attribute=="LFRQ")
        {
            memcpy(&LFRQ, tblline.unknow,4);
        }
        else if(attribute=="V5SR")
        {
            memcpy(&V5SR, tblline.unknow,4);
        }
        else if(attribute=="L3NS")
        {
            memcpy(&L3NS, tblline.unknow,4);
        }
        else if(attribute=="L4NS")
        {
            memcpy(&L4NS, tblline.unknow,4);
        }
        else if(attribute=="SRL3")
        {
            memcpy(&SRL3, tblline.unknow,4);
        }
        else if(attribute=="SRL4")
        {
            memcpy(&SRL4, tblline.unknow,4);
        }
        else if(attribute=="SRL5")
        {
            memcpy(&SRL5, tblline.unknow,4);
        }
        else if(attribute=="CHEX")
        {
            memcpy(&CHEX, tblline.unknow,4);
        }
        else if(attribute=="CHEY")
        {
            memcpy(&CHEY, tblline.unknow,4);
        }
        else if(attribute=="CHHX")
        {
            memcpy(&CHHX, tblline.unknow,4);
        }
        else if(attribute=="CHHY")
        {
            memcpy(&CHHY, tblline.unknow,4);
        }
        else if(attribute=="CHHZ")
        {
            memcpy(&CHHZ, tblline.unknow,4);
        }
        else if(attribute=="EXAC")
        {
            memcpy(&EXAC, tblline.unknow,8);
        }
        else if(attribute=="EXDC")
        {
            memcpy(&EXDC, tblline.unknow,8);
        }
        else if(attribute=="EYAC")
        {
            memcpy(&EYAC, tblline.unknow,8);
        }
        else if(attribute=="EYDC")
        {
            memcpy(&EYDC, tblline.unknow,8);
        }
        else if(attribute=="HXAC")
        {
            memcpy(&HXAC, tblline.unknow,8);
        }
        else if(attribute=="HXDC")
        {
            memcpy(&HXDC, tblline.unknow,8);
        }
        else if(attribute=="HYAC")
        {
            memcpy(&HYAC, tblline.unknow,8);
        }
        else if(attribute=="HYDC")
        {
            memcpy(&HYDC, tblline.unknow,8);
        }
        else if(attribute=="HZAC")
        {
            memcpy(&HZAC, tblline.unknow,8);
        }
        else if(attribute=="HZDC")
        {
            memcpy(&HZDC, tblline.unknow,8);
        }
        else if(attribute=="EXR")
        {
            memcpy(&EXR, tblline.unknow,4);
        }
        else if(attribute.left(3)=="EYR")
        {
            memcpy(&EYR, tblline.unknow,4);
        }
        else if(attribute=="EAZM")
        {
            memcpy(&EAZM, tblline.unknow,8);
        }
        else if(attribute=="EXLN")
        {
            memcpy(&EXLN, tblline.unknow,8);
        }
        else if(attribute=="EYLN")
        {
            memcpy(&EYLN, tblline.unknow,8);
        }
        else if(attribute=="HAZM")
        {
            memcpy(&HAZM, tblline.unknow,8);
        }
        else if(attribute=="HXSN")
        {
            HXSN = QString(tblline.unknow);
        }
        else if(attribute=="HYSN")
        {
            HYSN = QString(tblline.unknow);
        }
        else if(attribute=="HZSN")
        {
            HZSN = QString(tblline.unknow);
        }
        else if(attribute=="TOTL")
        {
            memcpy(&TOTL, tblline.unknow,4);
        }
        else if(attribute=="BADR")
        {
            memcpy(&BADR, tblline.unknow,4);
        }
        else if(attribute=="SATR")
        {
            memcpy(&SATR, tblline.unknow,4);
        }
        else if(attribute=="NSAT")
        {
            memcpy(&NSAT, tblline.unknow,4);
        }
        else if(attribute=="LFIX")
        {
            LFIX = QDateTime(QDate(tblline.unknow[5]+2000,tblline.unknow[4],tblline.unknow[3]),
                    QTime(tblline.unknow[2],tblline.unknow[1],tblline.unknow[0]));
        }
        else if(attribute=="CLST")
        {
            memcpy(&CLST, tblline.unknow,4);
        }
        else if(attribute=="OCTR")
        {
            memcpy(&OCTR, tblline.unknow,4);
        }
        else if(attribute=="TERR")
        {
            memcpy(&TERR, tblline.unknow,4);
        }
        else if(attribute=="ELEV")
        {
            memcpy(&ELEV, tblline.unknow,4);
        }
        else if(attribute=="LATG")
        {
            LATG = QString(tblline.unknow);
            LATG.remove(QRegExp("[NE, ]"));
        }
        else if(attribute=="LNGG")
        {
            LNGG = QString(tblline.unknow);
            LNGG.remove(QRegExp("[NE, ]"));
        }
        else if(attribute=="XDOS")
        {
            memcpy(&XDOS, tblline.unknow,4);
        }
        else if(attribute=="CALS")
        {
            memcpy(&CALS, tblline.unknow,4);
        }
        else if(attribute=="CCLS")
        {
            memcpy(&CCLS, tblline.unknow,4);
        }
        else if(attribute=="CALR")
        {
            CALR = QString(tblline.unknow);
        }
        else if(attribute=="CCLT")
        {
            memcpy(&CCLT, tblline.unknow,4);
        }
        else if(attribute=="CFMN")
        {
            memcpy(&CFMN, tblline.unknow,8);
        }
        else if(attribute=="CFMX")
        {
            memcpy(&CFMX, tblline.unknow,8);
        }
        else if(attribute=="CCMN")
        {
            memcpy(&CCMN, tblline.unknow,8);
        }
        else if(attribute=="CCMX")
        {
            memcpy(&CCMX, tblline.unknow,8);
        }
        else if(attribute=="HATT")
        {
            memcpy(&HATT, tblline.unknow,8);
        }
        else if(attribute=="HNOM")
        {
            memcpy(&HNOM, tblline.unknow,8);
        }
        else if(attribute=="HAMP")
        {
            memcpy(&HAMP, tblline.unknow,8);
        }
        else if(attribute=="TCMB")
        {
            memcpy(&TCMB, tblline.unknow,4);
        }
        else if(attribute=="TALS")
        {
            memcpy(&TALS, tblline.unknow,4);
        }
        else if(attribute=="CPHC")
        {
            memcpy(&CPHC, tblline.unknow,8);
        }
        else if(attribute=="BAT1")
        {
            memcpy(&BAT1, tblline.unknow,4);
        }
        else if(attribute=="BAT2")
        {
            memcpy(&BAT2, tblline.unknow,4);
        }
        else if(attribute=="BAT3")
        {
            memcpy(&BAT3, tblline.unknow,4);
        }
        else if(attribute=="TEMP")
        {
            memcpy(&TEMP, tblline.unknow,4);
        }
        else if(attribute=="GFPG")
        {
            memcpy(&GFPG, tblline.unknow,4);
        }
        else if(attribute=="FFPG")
        {
            memcpy(&FFPG, tblline.unknow,4);
        }
        else if(attribute.left(3)=="DSP")
        {
            memcpy(&DSP, tblline.unknow,4);
        }
        else if(attribute=="DISK")
        {
            memcpy(&DISK, tblline.unknow,4);
        }
        else if(attribute=="CPTH")
        {
            CPTH = QString(tblline.unknow);
        }
        else if(attribute=="DPTH")
        {
            DPTH = QString(tblline.unknow);
        }
        else if(attribute=="TXPR")
        {
            memcpy(&TXPR, tblline.unknow,4);
        }
        else if(attribute=="FSCV")
        {
            memcpy(&FSCV, tblline.unknow,8);
        }
        else
        {
            //qDebug()<<"attribute"<<attribute;
        }
    }
    tblFile.close();
    return true;
}

QDateTime TBL::startTime()  //采样开始时间
{
    return this->STIM;
}

QDateTime TBL::endTime()    //采样结束时间
{
    return this->ETIM;
}

QDateTime TBL::highFreStartTime()   //高频开始时间
{
    return this->HTIM;
}

QDateTime TBL::highFreEndTime() //高频结束时间
{
    return this->ETMH;
}

qint32 TBL::highFreSamp()   //高频采样间隔
{
    return this->HSMP;
}

qint32 TBL::MTU()      //MTU序列号
{
    return this->SNUM;
}

qint32 TBL::E_Gain()      //电场通道增益
{
    return this->EGN;
}

qint32 TBL::H_Gain()      //磁场通道增益
{
    return this->HGN;
}

qint32 TBL::L3Datas()     //L3数据采集量
{
    return this->L3NS;
}

qint32 TBL::L4Datas()     //L4数据采集量
{
    return this->L4NS;
}

qint32 TBL::L3SamplingRate()  //L3数据采样率
{
    return this->SRL3;
}

qint32 TBL::L4SamplingRate()  //L4数据采样率
{
    return this->SRL4;
}

qint32 TBL::L5SamplingRate()  //L5数据采样率
{
    return this->SRL5;
}

double TBL::ExAzimuth() //Ex方位角
{
    return this->EAZM;
}

double TBL::ExLength()  //Ex长度
{
    return this->EXLN;
}

double TBL::EyLength()  //Ey长度
{
    return this->EYLN;
}

QString TBL::HxCoil()   //Hx探头序列号
{
    return this->HXSN;
}

QString TBL::HyCoil()   //Hy探头序列号
{
    return this->HYSN;
}

QString TBL::HzCoil()   //Hz探头序列号
{
    return this->HZSN;
}

qint32 TBL::totalRecords()    //总记录数
{
    return this->TOTL;
}

qint32 TBL::badRecords()      //坏记录数
{
    return this->BADR;
}

qint32 TBL::saturationRecords()   //饱和记录数
{
    return this->SATR;
}

qint32 TBL::elevation()    //高程
{
    return this->ELEV;
}

double TBL::latitude()  //纬度
{
    QString lat = this->LATG;
    lat = lat.remove(lat.count()-2,2);
    int degree = int(lat.toDouble()/100);
    double minu = lat.toDouble()-degree*100;
    return (double)degree+ (double)minu/60;
}

double TBL::longitude() //经度
{
    QString log = this->LNGG;
    log = log.remove(log.count()-2,2);
    int degree = int(log.toDouble()/100);
    double minu = log.toDouble()-degree*100;
    return (double)degree+ (double)minu/60;
}

double TBL::attenuationCoefficient()    //探头衰减系数
{
    return this->HATT;
}

double TBL::ratedGain()     //探头额定增益
{
    return this->HNOM;
}
