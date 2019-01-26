#ifndef FFTW_PARAMETER_H
#define FFTW_PARAMETER_H

#include <QWidget>
#include <QDebug>
#include <QVector>
#include <QFile>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QDateTime>
#include <QProgressDialog>
#include <QProcess>
#include "files/info.h"
#include "tblread.h"
#include "files/yup.h"
#include "yupreader.h"


namespace Ui {
class FFTW_Parameter;
}

class FFTW_Parameter : public QWidget
{
    Q_OBJECT

public:
    explicit FFTW_Parameter(QWidget *parent = 0);
    ~FFTW_Parameter();

private:
    Ui::FFTW_Parameter *ui;
    tblInfo info;
    QString tsfilename;
    QDateTime ftime,ltime;
    QStringList windows;
    QVector < double > frequency;
    QVector < fretime > freqs_times;
    QString filepath;
    QVector < QVector < QVector < scatter > > > spectrumdatas_all;
    QVector < double > emitfreq;

private:
    void getfreinfo();
    void readfrefile(QString filename);
    void readfretimefile(QString filename);
    void savefrefile(QString filename);
    void savefretimefile(QString filename);
    QVector < QVector < scatter > > statistical(QVector < QVector < QVector < scatter > > > input);
    scatter statistical1(QVector < scatter > input);

public slots:
    void setTblInfo(tblInfo temp,QString tsn);

private slots:
    void on_pushButton_frefile_clicked();
    void on_pushButton_saveFreq_clicked();
    void on_pushButton_fretimefile_clicked();
    void on_pushButton_saveFreqTime_clicked();
    void on_pushButton_makeFFT_clicked();
    void on_pushButton_freadd_clicked();
    void on_pushButton_fretimeadd_clicked();
    void on_pushButton_fredel_clicked();
    void on_pushButton_fretimedel_clicked();
    void on_pushButton_freall_clicked();
    void on_pushButton_fretimeall_clicked();
};

#endif // FFTW_PARAMETER_H
