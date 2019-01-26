#ifndef SPECTRUMCAL_H
#define SPECTRUMCAL_H

#include <QObject>
#include "files/info.h"
#include "fftw3.h"
#include "math.h"


#define W_RECT 0
#define W_HAMM 1
#define W_HANN 2
#define W_BLAC 3

class spectrumCal
{
public:
    spectrumCal();

private:
   double PI;

public:
   double sw;
   double *fftin;
   fftw_complex *fftout;
   fftw_plan fftw;
   int N;

private:
   QVector<double> rectWindows(QVector<double>buff);
   QVector<double> hammingWindows(QVector<double>buff);
   QVector<double> hanningWindows(QVector<double>buff);
   QVector<double> blackmanWindows(QVector<double>buff);

public:
    dataInfo spectrum(dataInfo wave,int windows);
    QVector < dataInfo > spectrum_v(QVector < dataInfo > wave,int windows);
};

#endif // SPECTRUMCAL_H
