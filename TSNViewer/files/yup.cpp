#include "yup.h"

#include <QDateTime>
#include <QDebug>
#include <QObject>

YUP::YUP(QObject *parent): QObject(parent)
{

}

YUP::YUP(QString filename)
{
    setFileName(filename);
}

void YUP::setFileName(QString filename)
{
    if(filename.isEmpty())
    {
        return;
    }
    filePath = filename;
}

void YUP::write()
{
    if(filePath.isEmpty())
    {
        return;
    }
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QString line = tr("[STATION]  ")+name+tr("\n");
        line += tr("[LATITUDE]   ")+QString::number(latitude)+tr("\n");
        line += tr("[LONGITUDE]   ")+QString::number(longitude)+tr("\n");
        line += tr("[FREQS]   ");
        for(int i=0;i<freqs.count();i++)
        {
            line += QString::number(freqs.at(i))+tr(" ");
        }
        line += tr("\n");
        timecount = timefreqs.count();
        line += tr("[SEGMENT]   ")+QString::number(timecount)+tr("\n");
        line += tr("[CHANNEL]   ")+QString::number(chcount)+tr("\n");
        for(int i=0;i<chnames.count();i++)//通道名称
        {
            line += chnames.at(i)+tr(" ");
        }
        line += tr("\n");
        line += tr("0===========\n");
        for(int i=0;i<timecount;i++)
        {
            line += timefreqs[i].start.toString("[yyyy-MM-dd][hh:mm:ss]")+tr("\n");
            line += timefreqs[i].end.toString("[yyyy-MM-dd][hh:mm:ss]")+tr("\n");
            for(int m=0;m<timefreqs[i].psds.count();m++)
            {
                for(int n=0;n<timefreqs[i].psds[m].count();n++)
                {
                    qDebug()<<m<<n<<timefreqs.at(i).psds[m][n];
                    if(QString::number(timefreqs.at(i).psds[m][n])=="nan")
                    {
                        qDebug()<<filePath;
                        qDebug()<<"======nan======nan========nan=====";
                    }
                    line += QString::number(timefreqs.at(i).psds[m][n])+tr(" ");
                }
                line += tr("\n");
            }
            line += QString::number(i+1)+tr("===========\n");
        }
        file.write(line.toLocal8Bit().data());
    }
    file.close();
}

void YUP::read()
{
    if(filePath.isEmpty())
    {
        return;
    }
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString tempstring;
        QByteArray line = file.readLine();
        line.remove(0,tr("[STATION]  ").count());//tr("[STATION]  ")
        tempstring = line;
        name = tempstring.remove("\n");//"\n"
        line = file.readLine();
        line.remove(0,tr("[LATITUDE]   ").count());//tr("[LATITUDE]   ")
        tempstring = line;
        latitude = tempstring.remove("\n").toDouble();//"\n"
        line = file.readLine();
        line.remove(0,tr("[LONGITUDE]   ").count());//tr("[LONGITUDE]   ")
        tempstring = line;
        longitude = tempstring.remove("\n").toDouble();//"\n"
        line = file.readLine();
        line.remove(0,tr("[FREQS]   ").count());//tr("[FREQS]   ")
        tempstring = line;
        tempstring = tempstring.remove("\n");//"\n"
        QStringList spit = tempstring.split(' ');
        freqs.clear();
        for(int i=0;i<spit.count();i++)
        {
            QString currentfreqname = spit.at(i);
            if(!currentfreqname.isEmpty())
                freqs << currentfreqname.toDouble();
        }
        line = file.readLine();
        line.remove(0,tr("[SEGMENT]   ").count());//tr("[SEGMENT]   ")
        tempstring = line;
        timecount = tempstring.remove("\n").toInt();//"\n"
        line = file.readLine();
        line.remove(0,tr("[CHANNEL]   ").count());//tr("[CHANNEL]   ")
        tempstring = line;
        chcount = tempstring.remove("\n").toInt();//"\n"
        line = file.readLine();
        tempstring = line;
        tempstring = tempstring.remove("\n");//"\n"
        spit = tempstring.split(' ');
        chnames.clear();
        for(int i=0;i<spit.count();i++)
        {
            QString currentchname = spit.at(i);
            if(!currentchname.isEmpty())
                chnames << currentchname;
        }

        timefreqs.clear();
        for(int i=0;i<timecount;i++)
        {
            timeFreq temp;
            line = file.readLine();
            line = file.readLine();
            tempstring = line;
            tempstring = tempstring.remove("\n");//"\n"
            temp.start = QDateTime::fromString(tempstring,"[yyyy-MM-dd][hh:mm:ss]");
            line = file.readLine();
            tempstring = line;
            tempstring = tempstring.remove("\n");//"\n"
            temp.end = QDateTime::fromString(tempstring,"[yyyy-MM-dd][hh:mm:ss]");
            temp.psds.clear();
            for(int m=0;m<chcount;m++)
            {
                QVector<double> singlechpsd;

                line = file.readLine();
                tempstring = line;
                tempstring = tempstring.remove("\n");//"\n"
                QStringList spit = tempstring.split(' ');
                for(int n=0;n<spit.count();n++)
                {
                    QString currentpsd = spit.at(n);
                    if(!currentpsd.isEmpty())
                        singlechpsd << qAbs(currentpsd.toDouble());
                }
                temp.psds << singlechpsd;
            }
            timefreqs << temp;
        }
    }
    file.close();
}
