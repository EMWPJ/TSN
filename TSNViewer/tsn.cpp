#include "tsn.h"

#include <QFile>
#include <QFileInfo>
#include <QDataStream>

TSN::TSN()
{
    filepath.clear();
}

TSN::TSN(QString filename)
{
    setFileName(filename);
}

void TSN::setFileName(QString filename)
{
    filepath = filename;
    if(!QFile(filepath).exists())
    {
        filepath.clear();
        return;
    }
    QFile tsnfile(filepath);
    if(!tsnfile.open(QIODevice::ReadOnly))
    {
        return;
    }
    Ts temp;
    tsnfile.read((char*)&temp.second,sizeof(temp.second));
    tsnfile.read((char*)&temp.minute,sizeof(temp.minute));
    tsnfile.read((char*)&temp.hour,sizeof(temp.hour));
    tsnfile.read((char*)&temp.day,sizeof(temp.day));
    tsnfile.read((char*)&temp.mouth,sizeof(temp.mouth));
    tsnfile.read((char*)&temp.year,sizeof(temp.year));
    tsnfile.read((char*)&temp.week,sizeof(temp.week));
    tsnfile.read((char*)&temp.century,sizeof(temp.century));
    tsnfile.read((char*)&temp.BoxSN,sizeof(temp.BoxSN));
    tsnfile.read((char*)&temp.NumberofScans,sizeof(temp.NumberofScans));
    tsnfile.read((char*)&temp.NumberofChannels,sizeof(temp.NumberofChannels));
    tsnfile.read((char*)&temp.LengthofTS,sizeof(temp.LengthofTS));
    tsnfile.read((char*)&temp.StatusCodes,sizeof(temp.StatusCodes));
    tsnfile.read((char*)&temp.SaturationFlag,sizeof(temp.SaturationFlag));
    tsnfile.read((char*)&temp.Reservedbit,sizeof(temp.Reservedbit));
    tsnfile.read((char*)&temp.Samplelength,sizeof(temp.Samplelength));
    tsnfile.read((char*)&temp.SampleRate,sizeof(temp.SampleRate));
    tsnfile.close();
    NumberofChannels = temp.NumberofChannels;
    NumberofScans = temp.NumberofScans;
    SamplingRate = temp.SampleRate;
    length_pre = 32 + NumberofChannels * NumberofScans * 3;
    QFileInfo tsnfileinfo(filepath);
    tscount =  tsnfileinfo.size() / length_pre;
}

qint32 TSN::sampleRate()
{
    return SamplingRate;
}

Ts TSN::getdata(qint32 index)
{
    Ts temp;
    if(index>tscount)
    {
        return temp;
    }
    QFile tsnfile(filepath);
    if(!tsnfile.open(QIODevice::ReadOnly))
    {
        return temp;
    }
    tsnfile.seek((index - 1) * length_pre);

    tsnfile.read((char*)&temp.second,sizeof(temp.second));
    tsnfile.read((char*)&temp.minute,sizeof(temp.minute));
    tsnfile.read((char*)&temp.hour,sizeof(temp.hour));
    tsnfile.read((char*)&temp.day,sizeof(temp.day));
    tsnfile.read((char*)&temp.mouth,sizeof(temp.mouth));
    tsnfile.read((char*)&temp.year,sizeof(temp.year));
    tsnfile.read((char*)&temp.week,sizeof(temp.week));
    tsnfile.read((char*)&temp.century,sizeof(temp.century));
    tsnfile.read((char*)&temp.BoxSN,sizeof(temp.BoxSN));
    tsnfile.read((char*)&temp.NumberofScans,sizeof(temp.NumberofScans));
    tsnfile.read((char*)&temp.NumberofChannels,sizeof(temp.NumberofChannels));
    tsnfile.read((char*)&temp.LengthofTS,sizeof(temp.LengthofTS));
    tsnfile.read((char*)&temp.StatusCodes,sizeof(temp.StatusCodes));
    tsnfile.read((char*)&temp.SaturationFlag,sizeof(temp.SaturationFlag));
    tsnfile.read((char*)&temp.Reservedbit,sizeof(temp.Reservedbit));
    tsnfile.read((char*)&temp.Samplelength,sizeof(temp.Samplelength));
    tsnfile.read((char*)&temp.SampleRate,sizeof(temp.SampleRate));
    tsnfile.read((char*)&temp.SampleRateUnits,sizeof(temp.SampleRateUnits));
    tsnfile.read((char*)&temp.ClockStatus,sizeof(temp.ClockStatus));
    tsnfile.read((char*)&temp.ClockError,sizeof(temp.ClockError));
    tsnfile.read((char*)&temp.keep0,sizeof(temp.keep0));
    tsnfile.read((char*)&temp.keep1,sizeof(temp.keep1));
    tsnfile.read((char*)&temp.keep2,sizeof(temp.keep2));
    tsnfile.read((char*)&temp.keep3,sizeof(temp.keep3));
    tsnfile.read((char*)&temp.keep4,sizeof(temp.keep4));
    tsnfile.read((char*)&temp.keep5,sizeof(temp.keep5));
    temp.time = QDateTime(QDate(temp.year+temp.century*100,temp.mouth,temp.day),QTime(temp.hour,temp.minute,temp.second));
    temp.datas.clear();
    temp.datas.resize(temp.NumberofChannels);
    for(int i=0;i<temp.NumberofChannels;i++)
    {
        temp.datas[i].resize(temp.NumberofScans);
    }
    char *bytes = new char[3];
    for(int i=0;i<temp.NumberofScans;i++)
    {
        for(int j=0;j<temp.NumberofChannels;j++)
        {
            tsnfile.read(bytes,3);
            temp.datas[j][i] = bigbytesToInt32(QByteArray::fromRawData(bytes,3));
        }
    }
    tsnfile.close();
    return temp;
}

qint32 TSN::segment()
{
    return tscount;
}

qint32 TSN::scans()
{
    return NumberofScans;
}

qint32 TSN::channels()
{
    return NumberofChannels;
}

dataInfo TSN::getData(int index, int ch)
{
    dataInfo tempdatainfo;
    Ts temp;
    if(index>tscount)
    {
        return tempdatainfo;
    }
    temp = getdata(index);
    tempdatainfo.samplerate = temp.SampleRate;
    //    qDebug()<<tempdatainfo.samplerate<<temp.SampleRate;
    tempdatainfo.sn = temp.BoxSN;
    tempdatainfo.time = temp.time;
    tempdatainfo.value.clear();
    for(int i=0;i<temp.datas.at(ch-1).count();i++)
    {
        tempdatainfo.value<<temp.datas.at(ch-1).at(i);
    }
    return tempdatainfo;
}

QVector < dataInfo > TSN::getData(int index)
{
    QVector < dataInfo > tempdatainfo;
    Ts temp;
    if(index>tscount)
    {
        return tempdatainfo;
    }
    temp = getdata(index);
    tempdatainfo.resize(NumberofChannels);
    for(int j=0;j<NumberofChannels;j++)
    {
        tempdatainfo[j].samplerate = temp.SampleRate;
        //    qDebug()<<tempdatainfo.samplerate<<temp.SampleRate;
        tempdatainfo[j].sn = temp.BoxSN;
        tempdatainfo[j].time = temp.time;
        tempdatainfo[j].value.clear();
        for(int i=0;i<temp.datas.at(j).count();i++)
        {
            tempdatainfo[j].value<<temp.datas.at(j).at(i);
        }
    }
    return tempdatainfo;
}

QDateTime TSN::begintime()
{
    return getdata(1).time;
}

QDateTime TSN::endtime()
{
    return getdata(tscount).time;
}

