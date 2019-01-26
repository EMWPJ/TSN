#include "intercal.h"

#include <QDebug>

interCal::interCal(QObject *parent) : QObject(parent)
{
    ts.clear();

    ts << 0.001 << 0.002 << 0.005 << 0.1 << 0.017689 << 0.2 << 0.35378 << 0.5 << 0.6 << 0.70755 << 0.8 << 1 << 1.5 << 2
       << 3 << 4 << 5 << 6 << 7 << 8 << 10 << 11.321 << 12 << 14 << 16 << 18 << 20 << 22.642 << 25 << 27.5 << 30 << 32
       << 35 << 37 << 40 << 42 << 45.283 << 55 << 60 << 63 << 65 << 70 << 73 << 76.19 << 80 << 85 << 88 << 90.567 <<95
       << 105 << 110 << 115 << 120 << 125 << 128 << 130 << 135 << 140 << 160 << 170 << 180 << 185.81 << 190
       << 210 << 216 << 220 << 230 << 240 << 250 << 256 << 260 << 280 << 300 << 320 << 340 << 360 << 390 << 425 << 475
       << 525 << 575 << 630 << 690 << 760 << 830 << 920 << 1020;
}

QVector<scatter> interCal::secDerv(QVector<scatter> info)//二阶求导
{
    QVector<scatter> sp;
    int n = info.count();
    if(n<=0)
    {
        return sp;
    }

    QVector < double > x;
    QVector < double > y;
    x.resize(n);
    y.resize(n);
    for(int i=0;i<n;i++)
    {
        x[i] = info[i].freq;
        y[i] = info[i].value;
    }

    QVector < double > y2;
    y2.resize(n);
    y2[0] = 0;

    double u[5000];
    u[0] = 0;

    for(int i=1;i<n-2;i++)
    {
        double sig = (x[i]-x[i-1])/(x[i+1]-x[i-1]);
        double p = sig*y2[i-1]+2;
        y2[i] = (sig-1.0)/p;
        //u(i)=(6.*((y(i+1)-y(i))/(x(i+1)-x(i))-(y(i)-y(i-1))/(x(i)-x(i-1)))/(x(i+1)-x(i-1))-sig*u(i-1))/p
        u[i] = (6.0*((y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/(x[i]-x[i-1]))/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }

    double qn = 0;
    double un = 0;
    y2[n-1] = (un-qn*u[n-2])/(qn*y2[n-2]+1.0);

    for(int i=n-2;i>=0;i--)
    {
        y2[i] = y2[i]*y2[i+1]+u[i];
    }

    for(int i=0;i<n;i++)
    {
        scatter temp;
        temp.value = y2[i];
        temp.freq = info[i].freq;
        sp << temp;
    }

    return sp;
}

double interCal::splint(QVector<scatter> org,QVector<scatter> sp,double freq)
{
    double y = 0;
    int n = org.count();
    if(n<=0)
    {
        return y;
    }

    QVector < double > xa;
    QVector < double > ya;
    QVector < double > y2a;
    xa.resize(n);
    ya.resize(n);
    y2a.resize(n);

    for(int i=0;i<n;i++)
    {
        xa[i] = org[i].freq;
        ya[i] = org[i].value;
        y2a[i] = sp[i].value;
    }

    int klo = 1;
    int khi = n;

    while(1)
    {
        double aaa = 0;
        if(khi-klo>1)
        {
            aaa = 2.0;
            int k = (khi+klo)/2;
            if(xa[k-1]>freq)
            {
                khi = k;
            }
            else
            {
                klo = k;
            }
            if(aaa<=1)
            {
                break;
            }
        }
        if(aaa<=1)
        {
            break;
        }
    }

    double h = xa[khi-1]-xa[klo-1];

    if(h==0)
    {
        return y;
    }

    double a = (xa[khi-1]-freq)/h;
    double b = (freq-xa[klo-1])/h;

    y = a*ya[klo-1]+b*ya[khi-1]+((pow(a,3)-a)*y2a[klo-1]+(pow(b,3)-b)*y2a[khi-1])*(pow(h,2))/6;

    return y;
}


QVector<scatter_c> interCal::spline(QVector<scatter_c> info, QVector<double> freqs)
{
    int count = info.count();
    QVector<scatter> amp,pha;
    amp.clear();
    pha.clear();
    for(int i=0;i<count;i++)
    {
        amp<<scatter(info.at(i).freq,info.at(i).amp);
        pha<<scatter(info.at(i).freq,info.at(i).pha);
    }
    QVector<scatter> ampresult = spline(amp,freqs);
    QVector<scatter> pharesult = spline(pha,freqs);
    QVector<scatter_c> result;
    result.clear();
    for(int i=0;i<freqs.count();i++)
    {
        result<<scatter_c(freqs.at(i),ampresult.at(i).value,pharesult.at(i).value);
    }
    return result;
}

QVector<scatter> interCal::spline(QVector<scatter> info, QVector<double> freqs)
{
    QVector< scatter > values;
    values.clear();
    if(info.count()<=0)
    {
        return values;
    }
    if(freqs.count()<=0)
    {
        return values;
    }

    info = sortPoint(info);
    freqs = sortPoint(freqs);
    QVector< scatter > buff = secDerv(info);
    for(int i=0;i<freqs.count();i++)
    {
        scatter temp;
        if(freqs[i]<info[0].freq)
        {
            temp.freq = freqs[i];
            temp.value = info[0].value;
            temp.time = info[0].time;
        }
        else if(freqs[i]>info[info.count()-1].freq)
        {
            temp.freq = freqs[i];
            temp.value = info[info.count()-1].value;
            temp.time = info[info.count()-1].time;
        }
        else
        {
            temp.freq = freqs[i];
            temp.value = splint(info,buff,freqs.at(i));
            temp.time = info[0].time;
        }
        values << temp;
        qDebug()<<"temp"<<i<<temp.freq<<temp.value;
    }
    return values;
}

QVector <scatter> interCal::sortPoint(QVector <scatter> data)
{
    QVector <scatter> temp;
    temp.clear();
    if(data.count()<=0)
    {
        return temp;
    }

    QList < double > freqs;
    freqs.clear();

    for(int i=0;i<data.count();i++)
    {
        freqs << data[i].freq;
    }

    qSort(freqs.begin(),freqs.end());

    for(int i=0;i<freqs.count();i++)
    {
        for(int m=0;m<data.count();m++)
        {
            if(freqs[i]==data[m].freq)
            {
                temp << data[m];
                data.remove(m);
            }
        }
    }

    return temp;
}

QVector <double> interCal::sortPoint(QVector <double> data)
{
    QVector <double> temp;
    temp.clear();
    if(data.count()<=0)
    {
        return temp;
    }

    QList < double > freqs;
    freqs.clear();

    for(int i=0;i<data.count();i++)
    {
        freqs << data[i];
    }

    qSort(freqs.begin(),freqs.end());

    for(int i=0;i<freqs.count();i++)
    {
        temp << freqs.at(i);
    }

    return temp;
}
