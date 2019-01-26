#ifndef YUPREADER_H
#define YUPREADER_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>

#include "displays/statisticsspecturmdisplay.h"
#include "files/info.h"
#include "files/yup.h"
#include <shlobj.h>

namespace Ui {
class yupReader;
}

class timetable
{
public:
    QDateTime start;
    QDateTime end;
    double freq;
};

class yupReader : public QWidget
{
    Q_OBJECT

public:
    explicit yupReader(QWidget *parent = 0);
    ~yupReader();

private:
    Ui::yupReader *ui;

    statisticsSpecturmDisplay *spectD;

    long currentIndex;
    long segment;
    int currentCh;

    YUP yupFile;

    QVector< QVector< scatter > > orgScatters;
    QVector< QVector< scatter > > scatters;

    QVector< QVector < QVector< scatter > > > allData;

    QTimer *timer;

    int statisticTimeStyle;

    int unit;

    int exch=999;
    int eych=999;
    int hxch=999;
    int hych=999;

    QString stationName;
    QString unitName;
    QString chName;

    QString filePath;

    QVector<double> markFreqs;

    QVector<timetable> tables;

private:
    void initPrams();
    void setIndex(int index, int total);
    void getStatistics(int ch, int statististyle);
    void setDisplay(int index);
    QVector< QVector< scatter > > statistis(QVector< QVector< scatter > > temp, int style);
    QVector< scatter > overlayCal(QVector< QVector< scatter > > temp);
    void setTableView(QVector< scatter > temp);

    QVector< scatter > setUnit(QVector< scatter > buff,int unit);
    void refreshLabel();
    void regSwf();

    void readSettings();
    void saveSettings();

public:
    void setYUPPath(QString path);

private slots:
    void autoPlay();

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_index_clicked();
    void on_comboBox_activated(int index);
    void on_pushButton_auto_clicked();
    void on_comboBox_statisticstime_activated(int index);
    void on_comboBox_unit_activated(int index);
    void on_pushButton_refreshLabel_clicked();
    void on_comboBox_displaymode_activated(int index);
    void on_pushButton_markfreq_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_exportPic_clicked();
    void on_pushButton_importTime_clicked();
    void on_pushButton_exportCSV_clicked();
};

#endif // YUPREADER_H
