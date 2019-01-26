#include "tblread.h"

#include <QProgressDialog>
#include <QApplication>
#include <QString>
#include <QSettings>
#include <QProgressDialog>

tblRead::tblRead(QObject *parent): QObject(parent)
{

}

tblRead::tblRead(tblInfo temp)
{
    if(temp.boxsn==0)
    {
        qDebug()<<"tblRead temp.boxsn==0";
        return;
    }

    setFileName(temp);
}

void tblRead::setFileName(tblInfo temp)
{
    if(temp.boxsn==0)
    {
        qDebug()<<"setFileName temp.boxsn==0";
        return;
    }
    info = temp;

    tsn.exist = true;
    tsn.boxsn = info.boxsn;
    tsn.tblpath = info.tblpath;
    tsn.clbpath = info.clbpath;
    tsn.tspath = info.ts3path;//
    tsn.hxsn = info.hxsn;
    tsn.hysn = info.hysn;
    tsn.hzsn = info.hzsn;
    tsn.hxpath = info.hxclcpath;
    tsn.hypath = info.hyclcpath;
    tsn.hzpath = info.hzclcpath;
}

void tblRead::setFreqs(QVector<double> freqs)
{
    if(freqs.count()<=0)
    {
        return;
    }
    tsnFreqs.clear();
    tsnFreqs = freqs;
    h.clear();
    box.clear();
    h = getClc();
    box = getClb();
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"h"<<i<<h[0][i].freq
               <<h[0][i].value
              <<h[1][i].value
             <<h[2][i].value;
    }
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"box"<<i<<tsnFreqs.at(i)
               <<box[0][i].value
              <<box[1][i].value
             <<box[2][i].value
            <<box[3][i].value
           <<box[4][i].value;
    }
}

QVector< scatter > tblRead::statistics(int ch, int windows, int index, int length)
{
    QVector< scatter > result;
    tsRead  tsread;
    tsread.setFileName(tsn);
    int tscount =  tsread.count();
    if(index + length > tscount)
    {
        qDebug()<<"index and length error"<<index<<length<<tscount;
        return result;
    }
    dataInfo spectrum =  tsread.getSpectrum(index,length,ch,windows);
    QVector < double > freqs;
    freqs.clear();
    int totalfrescount = spectrum.value.count();
    int fftcount = (totalfrescount - 1) * 2;
    double  samplerate = spectrum.samplerate;
    for(int i=0;i<totalfrescount;i++)
    {
        freqs<<(double)i * (samplerate/fftcount);
    }
    int frecount = tsnFreqs.count();
    dataInfo tempspectrum;
    tempspectrum.value.clear();
    tempspectrum.value.resize(frecount);
    for(int i=0;i<frecount;i++)
    {
        tempspectrum.value[i] = 0;
        double currentfre = tsnFreqs.at(i);
        double basefre = currentfre * 0.98;
        double topfre = currentfre * 1.02;
        int tempcount = 0;
        for(int j=0;j<totalfrescount;j++)
        {
            if(freqs.at(j)>basefre && freqs.at(j)<topfre)
            {
                tempspectrum.value[i] += spectrum.value.at(j);
                tempcount++;
            }
        }
        tempspectrum.value[i] /= (double)tempcount;
        if(tempcount <= 0)
        {
            for(int j=0;j<totalfrescount;j++)
            {
                if(freqs.at(j+1)>basefre && freqs.at(j)<topfre)
                {
                    tempspectrum.value[i] = spectrum.value.at(j);
                    break;
                }
            }
        }
    }
    result.clear();
    for(int i=0;i<frecount;i++)
    {
        result << scatter(tsnFreqs.at(i),tempspectrum.value[i]);
        //        qDebug()<<"result"<<i<<result.at(i).freq<<result.at(i).value;
    }

    QVector <scatter> h;
    if(ch==3)
    {
        h = getHxClc();
    }
    else if(ch==4)
    {
        h = getHyClc();
    }
    else if(ch==5)
    {
        h = getHzClc();
    }
    else
    {
        for(int i=0;i<frecount;i++)
        {
            h << scatter(0,0);
        }
    }
    QVector <scatter> box = getClb(ch);

    //        for(int i=0;i<h.count();i++)
    //        {
    //            qDebug()<<"h"<<h.at(i).value;
    //        }
    //        for(int i=0;i<box.count();i++)
    //        {
    //            qDebug()<<"box"<<i<<box.at(i).value;
    //        }


    for(int i=0;i<result.count();i++)
    {
        double freq = result[i].freq;
        if(ch>=2)
        {
            for(int j=0;j<h.count();j++)
            {
                if(freq == h[j].freq)
                {
                    result[i].value = result[i].value/(h[j].value*h[j].value);
                }
            }
        }
        for(int j=0;j<box.count();j++)
        {
            if(freq == box[j].freq)
            {
                result[i].value = result[i].value/(box[i].value*box[i].value);
            }
        }
    }
    return result;
}


QVector < QVector< scatter > > tblRead::statistics(int windows, int index, int length)
{
    QVector < QVector< scatter > > result;
    tsRead  tsread;
    tsread.setFileName(tsn);
    int tscount =  tsread.count();
    if(index + length > tscount)
    {
        qDebug()<<"index and length error"<<index<<length<<tscount;
        return result;
    }
    QVector < dataInfo > spectrum =  tsread.getSpectrum(index,length,windows);
    //    return result;
    int channels = spectrum.count();
    int totalfrescount =  spectrum[0].value.count();
    int fftcount = (totalfrescount - 1) * 2;
    QVector < double > freqs;
    freqs.clear();
    for(int i=0;i<totalfrescount;i++)
    {
        freqs<<(double)(i+1)/(double)fftcount*spectrum[0].samplerate;
    }
    int frecount = tsnFreqs.count();
    QVector < dataInfo > tempspectrum;
    tempspectrum.resize(channels);
    for(int k=0;k<channels;k++)
    {
        tempspectrum[k].value.resize(frecount);

        for(int i=0;i<frecount;i++)
        {
            tempspectrum[k].value[i] = 0;
            double currentfre = tsnFreqs.at(i);
            double basefre = currentfre * 0.9;
            double topfre = currentfre * 1.1;
            int tempcount = 0;
            for(int j=0;j<totalfrescount;j++)
            {
                if(freqs.at(j)>basefre && freqs.at(j)<topfre)
                {
                    tempspectrum[k].value[i] += spectrum[k].value.at(j);
                    tempcount++;
                }
            }
            //            qDebug()<<frecount<<i<<"tempcount"<<tempcount;
            tempspectrum[k].value[i] /= (double)tempcount;
            if(tempcount==0)
            {
                for(int j=0;j<totalfrescount;j++)
                {
                    if(freqs.at(j)<=currentfre && freqs.at(j+1)>=currentfre)
                    {
                        tempspectrum[k].value[i] = spectrum[k].value.at(j);
                        break;
                    }
                }
            }
        }
    }
    //    for(int i=0;i<frecount;i++)
    //    {
    //        qDebug()<<"before clb clc"<<i<<tsnFreqs.at(i)
    //               <<tempspectrum[0].value[i]
    //                 <<tempspectrum[1].value[i]
    //                   <<tempspectrum[2].value[i]
    //                     <<tempspectrum[3].value[i]
    //                       <<tempspectrum[4].value[i];
    //    }

    result.clear();
    result.resize(channels);
    for(int j=0;j<channels;j++)
    {
        result[j].clear();
        for(int i=0;i<frecount;i++)
        {
            result[j] << scatter(tsnFreqs.at(i),tempspectrum[j].value[i]);
            //        qDebug()<<"result"<<i<<result.at(i).freq<<result.at(i).value;
        }
    }


    for(int k=0;k<channels;k++)
    {
        for(int i=0;i<result[k].count();i++)
        {
            double freq = result[k][i].freq;
            if(k>1)
            {
                for(int j=0;j<h[k-2].count();j++)
                {
                    if(freq == h[k-2][j].freq)
                    {
                        result[k][i].value = result[k][i].value/(h[k-2][j].value*h[k-2][j].value);
                    }
                }
            }
            for(int j=0;j<box[k].count();j++)
            {
                if(freq == box[k][j].freq)
                {
                    result[k][i].value = result[k][i].value/(box[k][i].value*box[k][i].value);
                }
            }
        }
    }
    //    for(int i=0;i<frecount;i++)
    //    {
    //        qDebug()<<"result"<<i<<tsnFreqs.at(i)
    //               <<result[0][i].value
    //              <<result[1][i].value
    //             <<result[2][i].value
    //            <<result[3][i].value
    //           <<result[4][i].value;
    //    }

    return result;
}

QVector <scatter> tblRead::getHxClc()
{
    CLC file(info.hxclcpath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        //        qDebug()<<"Hxclc"<<i<<temp.freq<<temp.value;
        infoscatters << temp;
    }
    interCal inter;
    QVector <scatter> after = inter.spline(infoscatters,tsnFreqs);
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"Hx"<<after.at(i).freq<<after.at(i).value;
    }
    return after;
}

QVector <scatter> tblRead::getHyClc()
{
    CLC file(info.hyclcpath);
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
    QVector <scatter> after = inter.spline(infoscatters,tsnFreqs);
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"Hy"<<after.at(i).freq<<after.at(i).value;
    }
    return after;
}

QVector <scatter> tblRead::getHzClc()
{
    CLC file(info.hzclcpath);
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
    QVector <scatter> after = inter.spline(infoscatters,tsnFreqs);
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"Hz"<<after.at(i).freq<<after.at(i).value;
    }
    return after;
}


QVector < QVector < scatter > > tblRead::getClc()
{
    QVector < QVector < scatter > > result;
    result.clear();
    QVector <scatter> infoscatters;
    QVector < Info > infos;
    interCal inter;
    CLC file(info.hxclcpath);
    infos = file.information();
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    result << inter.spline(infoscatters,tsnFreqs);
    file.setFileName(info.hyclcpath);
    infos = file.information();
    infoscatters.clear();
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    result << inter.spline(infoscatters,tsnFreqs);
    file.setFileName(info.hzclcpath);
    infos = file.information();
    infoscatters.clear();
    for(int i=0;i<infos.count();i++)
    {
        scatter temp;
        temp.freq = infos[i].fre;
        temp.value = infos[i].Amp;
        infoscatters << temp;
    }
    result << inter.spline(infoscatters,tsnFreqs);
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"HxHyHz"<<result.at(0).at(i).freq
               <<result.at(0).at(i).value
              <<result.at(1).at(i).value
             <<result.at(2).at(i).value;
    }
    return result;
}

QVector <scatter> tblRead::getClb(int channl)
{
    channl = channl-1;
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
    return inter.spline(infoscatters,tsnFreqs);
}

QVector < QVector <scatter> > tblRead::getClb()
{
    QVector < QVector <scatter> > result;
    CLB file(info.clbpath);
    QVector < Info > infos = file.information();
    QVector <scatter> infoscatters;
    result.clear();
    for(int channl = 0;channl<file.nchn();channl++)
    {
        infoscatters.clear();
        for(int i=0;i<infos.count()/file.nchn();i++)
        {
            scatter temp;
            temp.freq = infos[i*file.nchn()+channl].fre;
            temp.value = infos[i*file.nchn()+channl].Amp;
            infoscatters << temp;
//            qDebug()<<channl<<i<<temp.freq<<temp.value;
        }
        interCal inter;
        result<<inter.spline(infoscatters,tsnFreqs);
    }
    for(int i=0;i<tsnFreqs.count();i++)
    {
        qDebug()<<"CLB"<<result.at(0).at(i).freq
               <<result.at(0).at(i).value
              <<result.at(1).at(i).value
             <<result.at(2).at(i).value
            <<result.at(3).at(i).value
           <<result.at(4).at(i).value;
    }
    return result;
}
