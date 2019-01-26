#include "tsread.h"

#include <QDebug>

tsRead::tsRead()
{

}

tsRead::tsRead(tsInfo temp)
{
    if(temp.exist==false)
    {
        qDebug()<<"tsRead temp.exist==false";
        return;
    }

    setFileName(temp);
}

void tsRead::setFileName(tsInfo temp)
{
    if(temp.exist==false)
    {
        qDebug()<<"tsRead  setFileName temp.exist==false";
        return;
    }

    info = temp;
    tsFile.setFileName(info.tspath);
    //tsDatas = tsFile.tsdatas;

    if(tsFile.segment()<=0)
    {
        qDebug()<<"tsRead  setFileName tsDatas.count()<=0";
        return;
    }
    tblFile.setFileName(info.tblpath);
}

dataInfo tsRead::getData(int index, int ch)
{
    dataInfo datainfo;
    //    int samplerate = tsFile.sampleRate();

    int scans = 1;
    //    if(samplerate<150)//ts5
    //    {
    //        scans = 100;
    //    }
    //    else if(samplerate<2400)//ts4
    //    {
    //        scans = tblFile.L4Datas();
    //    }
    //    else //ts3
    //    {
    //        scans = tblFile.L3Datas();
    //    }

    index = (index-1)*scans+1;
    if(index>tsFile.segment())
    {
        qDebug()<<"tsRead  getData index>tsFile.segment()";
        return datainfo;
    }
    datainfo = tsFile.getData(index,ch);
    for(int i=1;i<scans;i++)
    {
        dataInfo temp = tsFile.getData(index+i,ch);
        datainfo.value << temp.value;
    }

    double ratio = 0;

    double fscv = tblFile.FSCV;
    //qDebug()<<"fscv"<<fscv;

    if(ch == tblFile.CHEX)
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.ExLength())/fscv;
    }
    else if(ch == tblFile.CHEY)
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.EyLength())/fscv;
    }
    else if(ch == tblFile.CHHX)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else if(ch == tblFile.CHHY)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else if(ch == tblFile.CHHZ)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.ExLength())/fscv;
    }

    if(ratio==0)
    {
        qDebug()<<"ch"<<ch;
        qDebug()<<tblFile.CHEX<<tblFile.CHEY<<tblFile.CHHX<<tblFile.CHHY<<tblFile.CHHZ;
        qDebug()<<"tsRead  getData ratio==0";
        datainfo.value.clear();
        return datainfo;
    }

    //ratio=1;

    for(int i=0;i<datainfo.value.count();i++)
    {
        datainfo.value[i] = datainfo.value[i]/ratio;
    }
    return datainfo;
}


dataInfo tsRead::getDatas(int index,int length, int ch)
{
    dataInfo datainfo;
    if(index+length>tsFile.segment())
    {
        qDebug()<<"tsRead  getData index>tsFile.segment()";
        return datainfo;
    }
    datainfo = tsFile.getData(index,ch);
    for(int i=1;i<length;i++)
    {
        dataInfo temp = tsFile.getData(index+i,ch);
        datainfo.value << temp.value;
    }

    double ratio = 0;

    double fscv = tblFile.FSCV;
    //qDebug()<<"fscv"<<fscv;

    if(ch == tblFile.CHEX)
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.ExLength())/fscv / 1E6;
    }
    else if(ch == tblFile.CHEY)
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.EyLength())/fscv / 1E6;
    }
    else if(ch == tblFile.CHHX)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else if(ch == tblFile.CHHY)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else if(ch == tblFile.CHHZ)
    {
        ratio = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    }
    else
    {
        ratio = (8388608.0*tblFile.E_Gain()*tblFile.ExLength())/fscv;
    }

    if(ratio==0)
    {
        qDebug()<<"ch"<<ch;
        qDebug()<<tblFile.CHEX<<tblFile.CHEY<<tblFile.CHHX<<tblFile.CHHY<<tblFile.CHHZ;
        qDebug()<<"tsRead  getData ratio==0";
        datainfo.value.clear();
        return datainfo;
    }

    //ratio=1;
    for(int i=0;i<datainfo.value.count();i++)
    {
        datainfo.value[i] = datainfo.value[i]/ratio;
    }
    return datainfo;
}

QVector < dataInfo > tsRead::getDatas(int index,int length)
{
    QVector < dataInfo > datainfo;
    if(index+length>tsFile.segment())
    {
        qDebug()<<"tsRead  getData index>tsFile.segment()";
        return datainfo;
    }
    datainfo.clear();
    datainfo.resize(chCount());
    for(int i=0;i<chCount();i++)
    {
        datainfo[i].value.clear();
    }
    for(int i=0;i<length;i++)
    {
        Ts tempts = tsFile.getdata(index+i);
        for(int j=0;j<chCount();j++)
        {
            for(int k=0;k<tempts.datas[j].count();k++)
            {
                datainfo[j].value<<(double)tempts.datas[j][k];
            }
            datainfo[j].samplerate = tempts.SampleRate;
        }
    }
    QVector < double > ratio;
    ratio.resize(datainfo.count());
    double fscv = tblFile.FSCV;
    ratio[tblFile.CHEX-1] = (8388608.0*tblFile.E_Gain()*tblFile.ExLength())/fscv / 1E6;
    ratio[tblFile.CHEY-1] = (8388608.0*tblFile.E_Gain()*tblFile.EyLength())/fscv / 1E6;
    ratio[tblFile.CHHX-1] = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    ratio[tblFile.CHHY-1] = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    ratio[tblFile.CHHZ-1] = (8388608.0*tblFile.H_Gain()*tblFile.attenuationCoefficient())/fscv;
    for(int j=0;j<datainfo.count();j++)
    {
        for(int i=0;i<datainfo[j].value.count();i++)
        {
            datainfo[j].value[i] = datainfo[j].value[i]/ratio[j];
        }
    }
    return datainfo;
}

dataInfo tsRead::getSpectrum(int index, int length, int ch, int windows = 0)
{
    spectrumCal spect;
    return spect.spectrum(getDatas(index,length,ch),windows);
}


QVector < dataInfo > tsRead::getSpectrum(int index, int length, int windows = 0)
{
    spectrumCal spect;
    return spect.spectrum_v(getDatas(index,length),windows);
}

QVector <scatter> tsRead::getHxClc()
{
    CLC file(info.hxpath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    interCal inter;
    return inter.spline(infoscatters,inter.ts);
}

QVector <scatter> tsRead::getHyClc()
{
    CLC file(info.hypath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    interCal inter;
    return inter.spline(infoscatters,inter.ts);
}

QVector <scatter> tsRead::getHzClc()
{
    CLC file(info.hzpath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    interCal inter;
    return inter.spline(infoscatters,inter.ts);
}

QVector <scatter> tsRead::getClb(int channl)
{
    CLB file(info.clbpath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    for(int i=0;i<infos.count()/file.nchn();i++)
    {
        scatter temp;
        temp.freq = infos[i*file.nchn()+channl].fre;
        temp.value = infos[i*file.nchn()+channl].Amp;
        infoscatters << temp;
    }
    interCal inter;
    return inter.spline(infoscatters,inter.ts);
}

int tsRead::chCount()
{
    return tsFile.NumberofChannels;
}

int tsRead::segment()
{
    int segment = tsFile.segment();
    int samplerate = tsFile.sampleRate();
    int scans = 1;
    if(samplerate<150)//ts5
    {
        scans = 100;
    }
    else if(samplerate<2400)//ts4
    {
        scans = tblFile.L4Datas();
    }
    else //ts3
    {
        scans = tblFile.L3Datas();
    }
    return segment/scans;
}

int tsRead::count()
{
    return tsFile.segment();
}


