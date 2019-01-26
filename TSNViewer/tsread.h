#ifndef TSREAD_H
#define TSREAD_H

#include <QObject>
#include "files/info.h"
#include "tbl.h"
#include "tsn.h"
#include "clb.h"
#include "clc.h"

#include "spectrumcal.h"
#include "intercal.h"

class tsRead
{
public:
    tsRead();
    tsRead(tsInfo temp);

private:
    TSN tsFile;
    TBL tblFile;
    tsInfo info;

public:
    void setFileName(tsInfo temp);
    dataInfo getData(int index, int ch);
    dataInfo getDatas(int index,int length, int ch);
    QVector < dataInfo > getDatas(int index,int length);
    dataInfo getSpectrum(int index,int length,int ch,int windows);
    QVector < dataInfo > getSpectrum(int index,int length,int windows);

    QVector <scatter> getHxClc();
    QVector <scatter> getHyClc();
    QVector <scatter> getHzClc();
    QVector <scatter> getClb(int channl);

    int chCount();
    int segment();
    int count();
};

#endif // TSREAD_H
