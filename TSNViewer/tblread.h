#ifndef TBLREAD_H
#define TBLREAD_H

#include <QObject>

#include "files/info.h"
#include "tbl.h"
#include "tsn.h"
#include "clb.h"
#include "clc.h"
#include "tsread.h"

#include "spectrumcal.h"
#include "intercal.h"

#include <QDateTime>

class tblRead : public QObject
{
    Q_OBJECT
public:
    explicit tblRead(QObject *parent = 0);
    explicit tblRead(tblInfo temp);

private:
    tblInfo info;
    tsInfo tsn;
    QVector<double> tsnFreqs;
    QVector < QVector <scatter> > h;
    QVector < QVector <scatter> > box;

public:
    void setFileName(tblInfo temp);
    QVector< scatter > statistics(int ch, int windows, int index, int length);
    QVector < QVector< scatter > >statistics(int windows, int index, int length);
    void setFreqs(QVector<double> freqs);

private:
   QVector <scatter> getHxClc();
   QVector <scatter> getHyClc();
   QVector <scatter> getHzClc();
   QVector <scatter> getClb(int channl);
   QVector < QVector < scatter > > getClb();
   QVector < QVector < scatter > > getClc();

};

#endif // TBLREAD_H
