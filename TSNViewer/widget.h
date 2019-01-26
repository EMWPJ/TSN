#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QColor>
#include <QDateTime>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "qcustomplot.h"
#include "tsn.h"
#include "tbl.h"
#include "clc.h"
#include "clb.h"
#include "tblread.h"
#include "fftw_parameter.h"

#include <QDebug>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_file_clicked();

    void on_toolButton_back_clicked();

    void on_toolButton_front_clicked();

    void on_toolButton_color_1_clicked();

    void on_toolButton_color_2_clicked();

    void on_toolButton_color_3_clicked();

    void on_toolButton_color_4_clicked();

    void on_toolButton_color_5_clicked();

    void on_toolButton_color_6_clicked();

    void on_horizontalScrollBar_index_valueChanged(int value);

    void on_treeWidget_tsfiles_itemClicked(QTreeWidgetItem *item, int column);

    void on_toolButton_clb1_clicked();

    void on_toolButton_clb2_clicked();

    void on_toolButton_clb3_clicked();

    void on_toolButton_clb4_clicked();

    void on_toolButton_clb5_clicked();

    void on_pushButton_clicked();


private:
    Ui::Widget *ui;

    QVector < TBL > tbls;
    QVector < CLB > clbs;
    QVector < CLC > clcs;

    QString currentfilename;
    QString sitename;
    qint32 index,length;
    QStringList scans;
    QStringList windows;
    QStringList samples_FFT;
    QVector < QVector < qint32 > > tsdatas;
    QVector < double > frequency;
    TSN ts;
    qint16 channel;
    qint32 tscount;
    qint32 fftsample;
    qint32 sample;
    QDateTime ftime,ltime;

    QVector < QColor > color,clbcolor;
    QPointF tspoint,sppoint;
    double tsxmin,tsxmax,tsymin,tsymax,spxmin,spxmax,spymin,spymax;

    QString filepath;
    QString filename;
    QStringList filenames;
    QString filefloder;
    QStringList filelist;

    QCPItemText *coor_TS;

    interCal *intercal;

    tblInfo tblinfo;

    FFTW_Parameter *fftw;

private:
    void changeDir();
    void setdatas(qint32 beg,qint32 len);
    QString Q2F(double temp,QString ef,int a,int b);
    QString Q2F(int temp, int a);

public slots:
    void showTS();
    void showclb();
    void showclc();
    void mouseMoveEvent_TS(QMouseEvent *e);
    void setdatas();
    void setlength();
    void axisDoubleClick_TS(QCPAxis *a,QCPAxis::SelectablePart,QMouseEvent *e);

signals:
    void setTblInfo(tblInfo temp, QString ts);
};

#endif // WIDGET_H
