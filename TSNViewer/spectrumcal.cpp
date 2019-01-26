#include "spectrumcal.h"

#include <QFile>
#include <QDebug>

spectrumCal::spectrumCal()
{
    sw = 0;
    PI = 4.0*atan(1.0);
}


QVector < dataInfo > spectrumCal::spectrum_v(QVector < dataInfo > wave,qint32 windows)
{
    QVector < dataInfo > result;
    result.clear();
    for(qint32 i=0;i<wave.count();i++)
    {
        result<<spectrum(wave[i],windows);
    }
    return result;
}

dataInfo spectrumCal::spectrum(dataInfo wave,qint32 windows)
{
    dataInfo temp;
    if(wave.value.size()<=0)
    {
        return temp;
    }
    if(wave.samplerate<=0)
    {
        return temp;
    }
    QVector< double > value = wave.value;
    N = wave.value.count();
    fftin = (double *) fftw_malloc(sizeof(double) * N);
    fftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1));

    switch(windows)
    {
    case W_RECT: value = rectWindows(value);break;
    case W_HAMM: value = hammingWindows(value);break;
    case W_HANN: value = hanningWindows(value);break;
    case W_BLAC: value = blackmanWindows(value);break;
    }
    sw = 0;
    switch(windows)
    {
    case W_RECT: for(qint32 i=0;i<N;i++) sw++;break;
    case W_HAMM: for(qint32 i=0;i<N;i++) sw+=(0.54-0.46*cos(2*PI*(i)/(N-1)))*(0.54-0.46*cos(2*PI*(i)/(N-1)));break;
    case W_HANN: for(qint32 i=0;i<N;i++) sw+=(0.5*(1-cos(2*PI*(i+1)/(N+1))))*(0.5*(1-cos(2*PI*(i+1)/(N+1))));break;
    case W_BLAC: for(qint32 i=0;i<N;i++) sw+=((0.42-0.5*cos(2*PI*(i)/(N-1))+0.08*cos(4*PI*(i)/(N-1))))
                *((0.42-0.5*cos(2*PI*(i)/(N-1))+0.08*cos(4*PI*(i)/(N-1))));break;
    }
    for(qint32 i=0;i<N;i++)
    {
        fftin[i] = value.at(i);
    }
    fftw=fftw_plan_dft_r2c_1d(N,fftin,fftout, FFTW_ESTIMATE);
    fftw_execute(fftw);
    value.clear();
    for(qint32 i=0;i<N/2+1;i++)
    {
        double real = fftout[i][0];
        double imag = fftout[i][1];
        value << (real*real + imag*imag)*(2/(sw * wave.samplerate));
    }
    fftw_destroy_plan(fftw);
    fftw_free(fftin);
    fftw_free(fftout);
    temp.value = value;
    temp.samplerate = wave.samplerate;
    temp.time = wave.time;
    temp.sn = wave.sn;
    return temp;
}


QVector<double> spectrumCal::rectWindows(QVector<double>buff)
{
    QVector<double> result;
    result.clear();
    int count = buff.count();
    if(count<=0)
    {
        return result;
    }

    for(int i=0;i<count;i++)
    {
        result << buff[i];
    }
    return result;
}

QVector<double> spectrumCal::hammingWindows(QVector<double>buff)
{
    QVector<double> result;
    result.clear();
    int count = buff.count();
    if(count<=0)
    {
        return result;
    }

    for(int i=0;i<count;i++)
    {
        result << buff[i]*(0.54-0.46*cos(2*PI*(i)/(count-1)));
    }

    return result;
}

QVector<double> spectrumCal::hanningWindows(QVector<double>buff)
{
    QVector<double> result;
    result.clear();
    int count = buff.count();
    if(count<=0)
    {
        return result;
    }

    for(int i=0;i<count;i++)
    {
        result << buff[i]*0.5*(1-cos(2*PI*(i+1)/(count+1)));
    }

    return result;
}

QVector<double> spectrumCal::blackmanWindows(QVector<double>buff)
{
    QVector<double> result;
    result.clear();
    int count = buff.count();
    if(count<=0)
    {
        return result;
    }

    for(int i=0;i<count;i++)
    {
        result << buff[i]*(0.42-0.5*cos(2*PI*(i)/(count-1))+0.08*cos(4*PI*(i)/(count-1)));
    }

    return result;
}
