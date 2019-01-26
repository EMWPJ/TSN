#ifndef STATISTICSSPECTURMDISPLAY_H
#define STATISTICSSPECTURMDISPLAY_H

#include <QWidget>
#include "../files/info.h"

namespace Ui {
class statisticsSpecturmDisplay;
}

class statisticsSpecturmDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit statisticsSpecturmDisplay(QWidget *parent = 0);
    ~statisticsSpecturmDisplay();

private:
    Ui::statisticsSpecturmDisplay *ui;

    QVector< scatter > info;

    QDateTime time;

    QAction *actSaveAS;
    QAction *actCopy;
    QAction *actSaveDat;

    double boundXMax;
    double boundXMin;
    double boundYMax;
    double boundYMin;
    QString unit;

    QVector<double> freqLabels;
    int displayMode;

    QVector<double> markFreqs;

private:
    void initPrams();
    void refresh();

public:
    void setData(QVector< scatter > temp);
    void contextMenuEvent(QContextMenuEvent *);
    void setText(QString);
    void setUnit(QString);
    void setLabels(QVector<double> freqtext);
    void setDisplayMode(int mode);
    void setMarkFreqs(QVector<double> freqs);
    QPixmap getGrap();

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent (QWheelEvent *e);

private slots:
    void act_saveas();
    void act_copy();
    void act_savedat();
};

#endif // STATISTICSSPECTURMDISPLAY_H
