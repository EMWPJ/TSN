#include "statisticsspecturmdisplay.h"
#include "ui_statisticsspecturmdisplay.h"

#include <QList>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QMenu>
#include <math.h>
#include <QClipboard>
#include <QFileDialog>

statisticsSpecturmDisplay::statisticsSpecturmDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statisticsSpecturmDisplay)
{
    ui->setupUi(this);

    initPrams();
}

statisticsSpecturmDisplay::~statisticsSpecturmDisplay()
{
    delete ui;
}

void statisticsSpecturmDisplay::initPrams()
{
    info.clear();

    unit = tr("nt²/Hz");

    boundXMax = 0;
    boundXMin = 0;
    boundYMax = 0;
    boundYMin = 0;

    freqLabels.clear();

    displayMode = 0;

    markFreqs.clear();

    //init menu
    actSaveAS = new QAction(QStringLiteral("保存截图"),this);
    connect(actSaveAS,SIGNAL(triggered()),this,SLOT(act_saveas()));
    actCopy = new QAction(QStringLiteral("复制截图"),this);
    connect(actCopy,SIGNAL(triggered()),this,SLOT(act_copy()));
    actSaveDat = new QAction(QStringLiteral("导出数据到文件"),this);
    connect(actSaveDat,SIGNAL(triggered()),this,SLOT(act_savedat()));
}

void statisticsSpecturmDisplay::setData(QVector< scatter > temp)
{
    if(temp.count()<=0)
    {
        qDebug()<<"statisticsSpecturmDisplay::setData temp.count()<=0";
        return;
    }

    for(int i=0;i<temp.count();)
    {
        if(temp[i].value<=0)
        {
            temp.remove(i);
        }
        else if(temp[i].freq<=0)
        {
            temp.remove(i);
        }
        else
        {
            i++;
        }
    }

    info = temp;
    time = info[0].time;

    boundXMin = temp[0].freq;
    boundXMax = temp[0].freq;
    boundYMin = temp[0].value;
    boundYMax = temp[0].value;
    for(int i=0;i<temp.count();i++)
    {
        if(temp[i].freq<boundXMin)
        {
            boundXMin = temp[i].freq;
        }
        if(temp[i].freq>boundXMax)
        {
            boundXMax = temp[i].freq;
        }
        if(temp[i].value<boundYMin)
        {
            boundYMin = temp[i].value;
        }
        if(temp[i].value>boundYMax)
        {
            boundYMax = temp[i].value;
        }
    }

    //qDebug()<<"first "<<boundYMin<<boundYMax;

    double n,m;
    if(boundXMin>0&&boundXMax>0)
    {
        n = log10(boundXMin);
        m = log10(boundXMax);
    }
    else
    {
        n = 3;
        m = 4;
    }

    int min = n;
    while(min>n)
    {
        min--;
    }
    boundXMin = min-1;
    int max = m;
    while(max<m)
    {
        max++;
    }

    boundXMax = max+1;

    if(boundYMin>0&&boundYMax>0)
    {
        n = log10(boundYMin);
        m = log10(boundYMax);
    }
    else
    {
        n = 3;
        m = 4;
    }

    //qDebug()<<" n m "<<n<<m;
    min = n;
    while(min>n)
    {
        min--;
    }
    boundYMin = min-1;
    max = m;
    while(max<m)
    {
        max++;
    }
    boundYMax = max+1;

    //qDebug()<<"LOG "<<boundXMin<<boundXMax<<boundYMin<<boundYMax;

    update();
}

void statisticsSpecturmDisplay::refresh()
{
    update();
}

void statisticsSpecturmDisplay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint (QPainter::Antialiasing,true);
    double wi_width = this->width();
    double wi_height = this->height();

    painter.fillRect(0,0,wi_width,wi_height,Qt::white);//背景底色

    QPen pen;
    pen.setWidth (2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(60,30,wi_width-90,wi_height-60);//边框

    if(boundXMax-boundXMin==0||boundYMax-boundYMin==0)
    {
        return;
    }

    QString firstleg = "标记频率";
    QString secleg = "其他频率";
    pen.setWidth (1);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(wi_width-30-secleg.count()*14,25,secleg);
    painter.drawText(wi_width-30-secleg.count()*14-firstleg.count()*14-15,25,firstleg);
    pen.setWidth (5);
    pen.setColor (Qt::red);
    pen.setStyle (Qt::SolidLine);
    painter.setPen (pen);
    painter.drawPoint(wi_width-30-secleg.count()*14-5,20);
    pen.setWidth (5);
    pen.setColor (Qt::darkBlue);
    pen.setStyle (Qt::SolidLine);
    painter.setPen (pen);
    painter.drawPoint(wi_width-30-secleg.count()*14-firstleg.count()*14-15-5,20);
    pen.setWidth (1);
    pen.setColor (Qt::black);
    pen.setStyle (Qt::DotLine);
    painter.setPen (pen);
    double xscalecount = boundXMax-boundXMin;
    if(xscalecount==0)
    {
        return;
    }
    double scale_x_mark = double(wi_width-90)/xscalecount;
    for(int i=0; i<xscalecount-1; i++)
    {
        painter.drawLine(60+double(i+1)*(scale_x_mark),30,60+double(i+1)*(scale_x_mark),wi_height-30);
    }
    double yscalecount = boundYMax-boundYMin;
    if(yscalecount==0)
    {
        return;
    }
    double scale_y_mark = double(wi_height-60)/yscalecount;
    for(int i=0; i<yscalecount-1; i++)
    {
        painter.drawLine(60,30+double(i+1)*scale_y_mark,wi_width-30,30+double(i+1)*scale_y_mark);
    }

    painter.rotate(270);
    painter.drawText(-1*wi_height/2-21,10,unit);
    painter.rotate(-270);

    pen.setWidth (1);
    pen.setColor (Qt::black);
    pen.setStyle (Qt::SolidLine);
    painter.setPen (pen);

    double x_scale = double(wi_width-90)/(xscalecount);
    double y_scale = double(wi_height-60)/(yscalecount);

    for(int i=0; i<yscalecount+1; i++)
    {
        double currenty = pow(10,(double(i)*scale_y_mark)/y_scale+boundYMin);
        QString text = QString::number(currenty,'e',1);
        painter.drawText(56-text.count()*6,wi_height-30-double(i)*scale_y_mark+3,text);
    }

    for(int i=0; i<xscalecount+1; i++)
    {
        double currentx = pow(10,(double(i)*scale_x_mark)/x_scale+boundXMin);
        QString text = QString::number(currentx);
        painter.drawText(60+double(i)*scale_x_mark-text.count()*3,wi_height-15,text);
    }
    pen.setWidth (4);
    pen.setColor (Qt::red);
    pen.setStyle (Qt::SolidLine);
    painter.setPen (pen);
    int count = info.count();
    //qDebug()<<count;
    double orgx = 60;
    double orgy = wi_height-30;
    for(int i=0;i<count;i++)
    {
        double value = info[i].value;
        double freq = info[i].freq;
        if(value<=0||freq<=0)
        {
            continue;
        }

        double x = 60+(log10(freq)-boundXMin)*x_scale;
        double y = wi_height-30-(log10(value)-boundYMin)*y_scale;

        if(x<60||x>wi_width-30||y<30||y>wi_height-30)
        {
            continue;
        }
        if(displayMode==0||displayMode==1)
        {
            pen.setWidth (4);
            pen.setColor (Qt::red);
            pen.setStyle (Qt::SolidLine);
            painter.setPen (pen);
            painter.drawPoint(x,y);
        }
        for(int m=0;m<freqLabels.count();m++)
        {
            if(freqLabels[m]==freq)
            {
                pen.setWidth (1);
                pen.setColor (Qt::red);
                pen.setStyle (Qt::SolidLine);
                painter.setPen (pen);
                QString text = QString::number(freq)+tr("Hz");
                painter.drawText(x-text.count()*4,y-8,text);
            }
        }

        if(markFreqs.count()>0)
        {
            if(markFreqs.contains(freq))
            {
                pen.setWidth (5);
                pen.setColor (Qt::darkBlue);
                pen.setStyle (Qt::SolidLine);
                painter.setPen (pen);
                painter.drawPoint(x,y);
            }
        }

        if((i>0)&&(displayMode==0||displayMode==2))
        {
            pen.setWidth (1);
            pen.setColor (Qt::red);
            pen.setStyle (Qt::SolidLine);
            painter.setPen (pen);
            painter.drawLine(orgx,orgy,x,y);
        }
        orgx = x;
        orgy = y;
    }
}

void statisticsSpecturmDisplay::mousePressEvent(QMouseEvent *e)
{

}

void statisticsSpecturmDisplay::mouseMoveEvent(QMouseEvent *e)
{

}

void statisticsSpecturmDisplay::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void statisticsSpecturmDisplay::mouseReleaseEvent(QMouseEvent *e)
{

}

void statisticsSpecturmDisplay::wheelEvent (QWheelEvent *e)
{

}

void statisticsSpecturmDisplay::contextMenuEvent(QContextMenuEvent *)
{
    QCursor cur = this->cursor();
    QMenu *menu = new QMenu(this);
    menu->addAction(actCopy);
    menu->addAction(actSaveAS);
    menu->addAction(actSaveDat);
    menu->exec(cur.pos());
}

void statisticsSpecturmDisplay::act_saveas()
{
    QString pic_file = QFileDialog::getSaveFileName(this
                                                    ,tr("保存图片")
                                                    ,tr("[spectrum]")
                                                    ,tr("图片(*.png)"));
    if(pic_file.isEmpty())
        return;

    QPixmap p = this->grab();
    p.save(pic_file);
}

QPixmap statisticsSpecturmDisplay::getGrap()
{
    return this->grab();
}

void statisticsSpecturmDisplay::act_copy()
{
    QPixmap p = this->grab();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(p);
}

void statisticsSpecturmDisplay::act_savedat()
{
    int count = info.count ();
    if(count<=0)
    {
        return;
    }

    QString dat_file = QFileDialog::getSaveFileName(this
                                                    ,tr("保存功率谱数据")
                                                    ,tr("[统计功率谱]")+time.toString("[yyyy-MM-dd][hh-mm-ss]")
                                                    ,tr("文本文档 (*.txt)"));
    if(dat_file.isEmpty())
        return;

    QFile savefile(dat_file);

    if(!savefile.open(QIODevice::WriteOnly))
    {
        return;
    }

    QString alltext;
    for(int i=0;i<count;i++)
    {
        QString line = info[i].time.toString("yyyy-MM-dd hh:mm:ss")
                +tr("\t")
                +QString::number (info[i].freq)
                +tr("\t")
                +QString::number (info[i].value)
                +tr("\n");
        alltext += line;
        savefile.write(line.toLatin1 ().data ());
    }

    savefile.close ();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText (alltext);
}

void statisticsSpecturmDisplay::setText(QString text)
{
    //qDebug()<<text;
    ui->label->setText(text);
    update();
}

void statisticsSpecturmDisplay::setUnit(QString text)
{
    if(text.isEmpty())
    {
        unit = tr("功率谱(nt²/Hz)");
        return;
    }
    unit = text;
    update();
}

void statisticsSpecturmDisplay::setLabels(QVector<double> freqtext)
{
    freqLabels = freqtext;
    update();
}

void statisticsSpecturmDisplay::setDisplayMode(int mode)
{
    displayMode = mode;
    update();
}

void statisticsSpecturmDisplay::setMarkFreqs(QVector<double> freqs)
{
    markFreqs = freqs;
    update();
}
