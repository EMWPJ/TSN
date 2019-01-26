#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QFile infofile("info");
    color.clear();
    if(infofile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        filepath = infofile.readLine();
        filepath.remove("\n");
        changeDir();
        while(!infofile.atEnd())
        {
            QString line = infofile.readLine();
            QStringList linelist = line.split(QRegExp("[,\t\n]"), QString::SkipEmptyParts);
            QColor tempcolor;
            line = linelist.at(0);
            tempcolor.setRed(line.toInt());
            line = linelist.at(1);
            tempcolor.setGreen(line.toInt());
            line = linelist.at(2);
            tempcolor.setBlue(line.toInt());
            color<<tempcolor;
            clbcolor<<tempcolor;
        }
    }
    else
    {
        color<<Qt::blue<<Qt::green<<Qt::yellow<<Qt::red<<Qt::darkRed<<Qt::black;
        clbcolor<<Qt::blue<<Qt::green<<Qt::yellow<<Qt::red<<Qt::darkRed<<Qt::black;
    }
    ui->toolButton_color_1->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(0).red()) + tr(",") +
                                          QString::number(color.at(0).green()) + tr(",") +
                                          QString::number(color.at(0).blue()) + tr(");}"));
    ui->toolButton_color_2->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(1).red()) + tr(",") +
                                          QString::number(color.at(1).green()) + tr(",") +
                                          QString::number(color.at(1).blue()) + tr(");}"));
    ui->toolButton_color_3->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(2).red()) + tr(",") +
                                          QString::number(color.at(2).green()) + tr(",") +
                                          QString::number(color.at(2).blue()) + tr(");}"));
    ui->toolButton_color_4->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(3).red()) + tr(",") +
                                          QString::number(color.at(3).green()) + tr(",") +
                                          QString::number(color.at(3).blue()) + tr(");}"));
    ui->toolButton_color_5->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(4).red()) + tr(",") +
                                          QString::number(color.at(4).green()) + tr(",") +
                                          QString::number(color.at(4).blue()) + tr(");}"));
    ui->toolButton_color_6->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                          QString::number(color.at(5).red()) + tr(",") +
                                          QString::number(color.at(5).green()) + tr(",") +
                                          QString::number(color.at(5).blue()) + tr(");}"));

    ui->toolButton_clb1->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                       QString::number(clbcolor.at(0).red()) + tr(",") +
                                       QString::number(clbcolor.at(0).green()) + tr(",") +
                                       QString::number(clbcolor.at(0).blue()) + tr(");}"));
    ui->toolButton_clb2->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                       QString::number(clbcolor.at(1).red()) + tr(",") +
                                       QString::number(clbcolor.at(1).green()) + tr(",") +
                                       QString::number(clbcolor.at(1).blue()) + tr(");}"));
    ui->toolButton_clb3->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                       QString::number(clbcolor.at(2).red()) + tr(",") +
                                       QString::number(clbcolor.at(2).green()) + tr(",") +
                                       QString::number(clbcolor.at(2).blue()) + tr(");}"));
    ui->toolButton_clb4->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                       QString::number(clbcolor.at(3).red()) + tr(",") +
                                       QString::number(clbcolor.at(3).green()) + tr(",") +
                                       QString::number(clbcolor.at(3).blue()) + tr(");}"));
    ui->toolButton_clb5->setStyleSheet(tr("QToolButton{background-color: rgb(") +
                                       QString::number(clbcolor.at(4).red()) + tr(",") +
                                       QString::number(clbcolor.at(4).green()) + tr(",") +
                                       QString::number(clbcolor.at(4).blue()) + tr(");}"));
    infofile.close();
    index = 0;
    connect(ui->comboBox_scans,SIGNAL(currentIndexChanged(int)),
            this,SLOT(setlength()));
    connect(ui->checkBox_chan_1,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));
    connect(ui->checkBox_chan_2,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));
    connect(ui->checkBox_chan_3,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));
    connect(ui->checkBox_chan_4,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));
    connect(ui->checkBox_chan_5,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));
    connect(ui->checkBox_chan_6,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));

    connect(ui->checkBox_clb1,SIGNAL(clicked(bool)),
            this,SLOT(showclb()));
    connect(ui->checkBox_clb2,SIGNAL(clicked(bool)),
            this,SLOT(showclb()));
    connect(ui->checkBox_clb3,SIGNAL(clicked(bool)),
            this,SLOT(showclb()));
    connect(ui->checkBox_clb4,SIGNAL(clicked(bool)),
            this,SLOT(showclb()));
    connect(ui->checkBox_clb5,SIGNAL(clicked(bool)),
            this,SLOT(showclb()));

    connect(ui->checkBox_AutoAxis_ts,SIGNAL(clicked(bool)),
            this,SLOT(showTS()));

    connect(ui->widget_ts,SIGNAL(mouseMove(QMouseEvent*)),
            this,SLOT(mouseMoveEvent_TS(QMouseEvent*)));
    connect(ui->widget_ts,SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,SLOT(axisDoubleClick_TS(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));

    fftw = new FFTW_Parameter();
    connect(this,SIGNAL(setTblInfo(tblInfo,QString)),
            fftw,SLOT(setTblInfo(tblInfo,QString)));
    index = 1;
    length = 1;
    ui->widget_ts->setMouseTracking(true);
    currentfilename = QStringLiteral("NoTs");
    tsxmin = 0;
    tsxmax = 1;
    tsymin = -10000;
    tsymax = 10000;
    spxmin = 0;
    spxmax = 1200;
    spymin = -2;
    spymax = 10;
    coor_TS = new QCPItemText(ui->widget_ts);
    channel = 5;
    this->setWindowTitle(QStringLiteral("TSN 1.0.0"));
}

Widget::~Widget()
{
    QFile infofile("info");
    infofile.open(QIODevice::WriteOnly | QIODevice::Text);
    QString info = filepath + tr("\n");
    infofile.write(info.toLatin1().data());
    for(int i=0;i<color.count();i++)
    {
        info = QString::number(color.at(i).red()) + tr(",") + QString::number(color.at(i).green()) + tr(",") + QString::number(color.at(i).blue()) + tr("\n");
        infofile.write(info.toLatin1().data());
    }
    infofile.close();
    delete ui;
}

void Widget::on_pushButton_file_clicked()
{
    filepath = QFileDialog::getExistingDirectory();
    changeDir();
}

void Widget::changeDir()
{
    QDir dir(filepath);
    ui->treeWidget_tsfiles->setHeaderLabel(filepath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QStringList filter;
    filter<<"*.tbl";
    dir.setNameFilters(filter);
    QFileInfoList list = dir.entryInfoList();
    filelist.clear();
    ui->treeWidget_tsfiles->clear();
    tbls.clear();
    clbs.clear();
    clcs.clear();
    for(int i=0;i<list.size();i++)
    {
        QString name = list.at(i).fileName();
        QString tblfilename = filepath + tr("/") + name;
        TBL temptbl(tblfilename);
        tbls<<temptbl;
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_tsfiles,QStringList(QString(name)));
        name.remove(".tbl");
        name.remove(".TBL");
        QString clbfilename = filepath + tr("/") + QString::number(temptbl.MTU()) + tr(".CLB");
        if(QFile(clbfilename).exists())
        {
            QTreeWidgetItem *item1 = new QTreeWidgetItem(item,QStringList(QString::number(temptbl.MTU()) + tr(".CLB")));
        }
        QString clcfilename = filepath + tr("/") + temptbl.HxCoil() + tr(".CLC");
        if(QFile(clcfilename).exists())
        {
            QTreeWidgetItem *item2 = new QTreeWidgetItem(item,QStringList(temptbl.HxCoil() + tr(".CLC")));
        }
        clcfilename = filepath + tr("/") + temptbl.HyCoil() + tr(".CLC");
        if(QFile(clcfilename).exists())
        {
            QTreeWidgetItem *item3 = new QTreeWidgetItem(item,QStringList(temptbl.HyCoil() + tr(".CLC")));
        }
        clcfilename = filepath + tr("/") + temptbl.HzCoil() + tr(".CLC");
        if(QFile(clcfilename).exists())
        {
            QTreeWidgetItem *item4 = new QTreeWidgetItem(item,QStringList(temptbl.HzCoil() + tr(".CLC")));
        }
        QString tempts = filepath + tr("/") + name + tr(".TS2");
        if(QFile(tempts).exists())
        {
            QTreeWidgetItem *item5 = new QTreeWidgetItem(item,QStringList(QString(name + tr(".TS2"))));
        }
        tempts = filepath + tr("/") + name + tr(".TS3");
        if(QFile(tempts).exists())
        {
            QTreeWidgetItem *item6 = new QTreeWidgetItem(item,QStringList(QString(name + tr(".TS3"))));
        }
        tempts = filepath + tr("/") + name + tr(".TS4");
        if(QFile(tempts).exists())
        {
            QTreeWidgetItem *item7 = new QTreeWidgetItem(item,QStringList(QString(name + tr(".TS4"))));
        }
        tempts = filepath + tr("/") + name + tr(".TS5");
        if(QFile(tempts).exists())
        {
            QTreeWidgetItem *item8 = new QTreeWidgetItem(item,QStringList(QString(name + tr(".TS5"))));
        }
    }
}

void Widget::on_treeWidget_tsfiles_itemClicked(QTreeWidgetItem *item, int column)
{
    currentfilename = filepath + "/" + item->text(column);

    sitename = currentfilename;
    sitename = sitename.remove(currentfilename.count()-4,4);
    QString tblfilename = sitename + tr(".tbl");
    tblinfo.tblpath = tblfilename;
    TBL tbl(tblfilename);
    QString boxname = filepath + tr("/") + QString::number(tbl.MTU()) + tr(".clb");
    tblinfo.clbpath = boxname;

    QString Hxname = filepath + tr("/") + tbl.HxCoil() + tr(".clc");
    QString Hyname = filepath + tr("/") + tbl.HyCoil() + tr(".clc");
    QString Hzname = filepath + tr("/") + tbl.HzCoil() + tr(".clc");
    tblinfo.hxclcpath = Hxname;
    tblinfo.hyclcpath = Hyname;
    tblinfo.hzclcpath = Hzname;
    tblinfo.boxsn = tbl.MTU();
    tblinfo.hxsn = tbl.HXSN;
    tblinfo.hysn = tbl.HYSN;
    tblinfo.hzsn = tbl.HZSN;
    tblinfo.latitude = tbl.latitude();
    tblinfo.longitude = tbl.longitude();
    tblinfo.name = tbl.SITE;
    tblinfo.ts2path = sitename + tr(".ts2");
    tblinfo.ts3path = sitename + tr(".ts3");
    tblinfo.ts4path = sitename + tr(".ts4");

    QString back = currentfilename;
    back.remove(0,back.count()-3);
    if(back == QString("TBL") || back == QString("tbl"))
    {
        ui->tabWidget->setCurrentWidget(ui->tab_tbl);
        TBL temptbl(currentfilename);
        ui->lineEdit_attenuationCoefficient->setText(QString::number(temptbl.attenuationCoefficient()));
        ui->lineEdit_badRecords->setText(QString::number(temptbl.badRecords()));
        ui->lineEdit_starttime->setText(temptbl.startTime().toString("yyyy/MM/dd-hh:mm:ss"));
        ui->lineEdit_boxsn->setText(QString::number(temptbl.MTU()));
        ui->lineEdit_egain->setText(QString::number(temptbl.E_Gain()));
        ui->lineEdit_elevation->setText(QString::number(temptbl.elevation()));
        ui->lineEdit_endtime->setText(temptbl.endTime().toString("yyyy/MM/dd-hh:mm:ss"));
        ui->lineEdit_exazimuth->setText(QString::number(temptbl.ExAzimuth()));
        ui->lineEdit_exlength->setText(QString::number(temptbl.ExLength()));
        ui->lineEdit_eyength->setText(QString::number(temptbl.EyLength()));
        ui->lineEdit_hgain->setText(QString::number(temptbl.H_Gain()));
        ui->lineEdit_highfreendtime->setText(temptbl.highFreEndTime().toString("yyyy/MM/dd-hh:mm:ss"));
        ui->lineEdit_highfresample->setText(QString::number(temptbl.highFreSamp()));
        ui->lineEdit_highfrestarttime->setText(temptbl.highFreStartTime().toString("yyyy/MM/dd-hh:mm:ss"));
        ui->lineEdit_hxcoil->setText(temptbl.HxCoil());
        ui->lineEdit_hycoil->setText(temptbl.HyCoil());
        ui->lineEdit_hzcoil->setText(temptbl.HzCoil());
        ui->lineEdit_l3data->setText(QString::number(temptbl.L3Datas()));
        ui->lineEdit_l3samplingrate->setText(QString::number(temptbl.L3SamplingRate()));
        ui->lineEdit_l4data->setText(QString::number(temptbl.L4Datas()));
        ui->lineEdit_l4samplingrate->setText(QString::number(temptbl.L4SamplingRate()));
        ui->lineEdit_l5samplingrate->setText(QString::number(temptbl.L5SamplingRate()));
        ui->lineEdit_latitude->setText(QString::number(temptbl.latitude()));
        ui->lineEdit_longitude->setText(QString::number(temptbl.longitude()));
        ui->lineEdit_ratedGain->setText(QString::number(temptbl.ratedGain()));
        ui->lineEdit_saturationRecords->setText(QString::number(temptbl.saturationRecords()));
        ui->lineEdit_site->setText(temptbl.SITE);
        ui->lineEdit_starttime->setText(temptbl.startTime().toString("yyyy/MM/dd-hh:mm:ss"));
        ui->lineEdit_totalrecords->setText(QString::number(temptbl.totalRecords()));
        return;
    }
    else if(back == QString("CLB") || back == QString("clb"))
    {
        ui->tabWidget->setCurrentWidget(ui->tab_clb);
        showclb();
        return;
    }
    else if(back == QString("CLC") || back == QString("clc"))
    {
        ui->tabWidget->setCurrentWidget(ui->tab_clc);
        showclc();
        return;
    }
    else if(back == QString("TS2") || back == QString("ts2") ||
            back == QString("TS3") || back == QString("ts3") ||
            back == QString("TS4") || back == QString("ts4") ||
            back == QString("TS5") || back == QString("ts5"))
    {
        ui->tabWidget->setCurrentWidget(ui->tab_ts);
        index = 1;
        ts.setFileName(currentfilename);
        sample = ts.sampleRate();
        channel = ts.NumberofChannels;
        tscount = ts.segment();
        ui->horizontalScrollBar_index->setRange(1,tscount);
        ui->horizontalScrollBar_index->setValue(1);
        ui->checkBox_chan_6->setDisabled(false);
        ui->checkBox_chan_5->setDisabled(false);
        ui->checkBox_chan_4->setDisabled(false);
        ui->checkBox_chan_3->setDisabled(false);
        ui->checkBox_chan_2->setDisabled(false);
        ui->checkBox_chan_1->setDisabled(false);
        if(channel<6)
        {
            ui->checkBox_chan_6->setDisabled(true);
        }
        if(channel<5)
        {
            ui->checkBox_chan_5->setDisabled(true);
        }
        if(channel<4)
        {
            ui->checkBox_chan_4->setDisabled(true);
        }
        if(channel<3)
        {
            ui->checkBox_chan_3->setDisabled(true);
        }
        if(channel<2)
        {
            ui->checkBox_chan_2->setDisabled(true);
        }
        if(channel<1)
        {
            return;
        }
        ui->label_samplingrate->setText(QString::number(sample));
        ui->label_samplelength->setText(QString::number(ts.NumberofScans));
        scans.clear();
        scans<<"1"<<"2"<<"4"<<"8"<<"16"<<"32"<<"64"<<"128"<<"256"<<"512";
        if(sample==150)
        {
            scans.clear();
            scans<<"1"<<"2"<<"4"<<"8"<<"16"<<"32"<<"64"<<"128"<<"256"<<"512";
        }
        if(sample==2400)
        {
            scans.clear();
            scans<<"1"<<"2"<<"4"<<"8"<<"16"<<"32"<<"64";
        }
        if(sample==24000)
        {
            scans.clear();
            scans<<"1"<<"2"<<"4"<<"8"<<"16"<<"32"<<"64";
        }
        ui->comboBox_scans->clear();
        ui->comboBox_scans->addItems(scans);
        Ts temp = ts.getdata(1);
        ftime.setDate(QDate(2000+temp.year,temp.mouth,temp.day));
        ftime.setTime(QTime(temp.hour,temp.minute,temp.second));
        ui->label_begin_time->setText(ftime.toString(tr("yyyy/MM/dd hh:mm:ss")));
        temp = ts.getdata(tscount);
        ltime.setDate(QDate(2000+temp.year,temp.mouth,temp.day));
        ltime.setTime(QTime(temp.hour,temp.minute,temp.second));
        ui->label_end_time->setText(ltime.toString(tr("yyyy/MM/dd hh:mm:ss")));
        length = ui->comboBox_scans->currentText().toInt();
        Ts tempts = ts.getdata(1);
        QDateTime begintime;
        begintime.setDate(QDate(2000+tempts.year,tempts.mouth,tempts.day));
        begintime.setTime(QTime(tempts.hour,tempts.minute,tempts.second));
        tempts = ts.getdata(tscount);
        QDateTime endtime;
        endtime.setDate(QDate(2000+tempts.year,tempts.mouth,tempts.day));
        endtime.setTime(QTime(tempts.hour,tempts.minute,tempts.second));
        qint32 seconds = begintime.secsTo(endtime);
        qint32 hour = seconds / 3600;
        qint32 minute = (seconds - hour * 3600) / 60;
        qint32 second = seconds - hour * 3600 - minute * 60;
        QString total_time = QString::number(hour) + tr("h ") + QString::number(minute) + tr("m ") + QString::number(second) + tr("s");
        ui->label_total_time->setText(total_time);
        setdatas(1,length);
    }
    return;
}

void Widget::setdatas()
{
    //qDebug()<<currentfilename;
    if(currentfilename == tr("NoTs"))
    {
        return;
    }
    setdatas(index,length);
}

void Widget::setdatas(qint32 beg,qint32 len)
{
    //qDebug()<<"begin and length"<<beg<<len;
    if(currentfilename == tr("NoTs"))
    {
        return;
    }
    Ts tempts = ts.getdata(beg);
    QDateTime time;
    time.setDate(QDate(2000+tempts.year,tempts.mouth,tempts.day));
    time.setTime(QTime(tempts.hour,tempts.minute,tempts.second));
    ui->label_currentbegin->setText(time.toString(tr("yyyy/MM/dd hh:mm:ss")));
    tempts = ts.getdata(beg + len);
    time.setDate(QDate(2000+tempts.year,tempts.mouth,tempts.day));
    time.setTime(QTime(tempts.hour,tempts.minute,tempts.second));
    ui->label_currentend->setText(time.toString(tr("yyyy/MM/dd hh:mm:ss")));
    tsdatas.clear();
    tsdatas.resize(channel);
    for(int i=beg;i<beg+ui->comboBox_scans->currentText().toInt();i++)
    {
        QVector < QVector < qint32 > > temp = ts.getdata(i).datas;
        for(int j=0;j<temp.count();j++)
        {
            QVector < qint32 > temptemp = temp.at(j);
            qint32 temptempcount = temptemp.count();
            for(int k=0;k<temptempcount;k++)
            {
                tsdatas[j]<<temptemp.at(k);
            }
        }
    }
    showTS();
}




void Widget::showTS()
{
    if(tsdatas.isEmpty())
    {
        return;
    }

    ui->widget_ts->clearGraphs();
    QVector < double > x;
    for(int i=0;i<tsdatas.at(0).count();i++)
    {
        x<<(double)i / (double)sample;
    }
    ui->widget_ts->setInteractions(QCP::iRangeDrag
                                   | QCP::iRangeZoom
                                   | QCP::iMultiSelect
                                   | QCP::iSelectOther
                                   | QCP::iSelectItems
                                   | QCP::iSelectPlottables
                                   );
    ui->widget_ts->xAxis->setLabel("t/s");
    ui->widget_ts->xAxis->setRange(tsxmin,tsxmax);
    ui->widget_ts->yAxis->setRange(tsymin,tsymax);
    int graphindex = 0;
    for(int i=0;i<channel;i++)
    {
        if((i==0 && ui->checkBox_chan_1->isChecked()) ||
                (i==1 && ui->checkBox_chan_2->isChecked()) ||
                (i==2 && ui->checkBox_chan_3->isChecked()) ||
                (i==3 && ui->checkBox_chan_4->isChecked()) ||
                (i==4 && ui->checkBox_chan_5->isChecked()) ||
                (i==6 && ui->checkBox_chan_6->isChecked()))
        {
            ui->widget_ts->addGraph();
            QPen tempPen;
            tempPen.setColor(color.at(i));
            tempPen.setWidth(1);
            ui->widget_ts->graph(graphindex)->setPen(tempPen);
            ui->widget_ts->graph(graphindex)->setName(QString::number(i+1));
            QVector < double > eh;
            eh.clear();
            for(int j=0;j<tsdatas.at(i).count();j++)
            {
                eh<<(double)tsdatas.at(i).at(j);
            }
            ui->widget_ts->graph(graphindex)->setData(x,eh);
            graphindex++;
        }
    }
    if(ui->checkBox_AutoAxis_ts->isChecked())
    {
        ui->widget_ts->rescaleAxes();
    }
    ui->widget_ts->replot();
}

void Widget::on_toolButton_back_clicked()
{
    if(index - length > 0)
    {
        index = index - length;
    }
    ui->horizontalScrollBar_index->setValue(index);
    setdatas();
}

void Widget::on_toolButton_front_clicked()
{
    qint32 count = ts.segment();
    if(index + length < count)
    {
        index = index + length;
    }
    ui->horizontalScrollBar_index->setValue(index);
    setdatas();
}

void Widget::on_toolButton_color_1_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(0,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_1->setStyleSheet(style);
    showTS();
}


void Widget::on_toolButton_color_2_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(1,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_2->setStyleSheet(style);
    showTS();
}

void Widget::on_toolButton_color_3_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(2,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_3->setStyleSheet(style);
    showTS();
}

void Widget::on_toolButton_color_4_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(3,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_4->setStyleSheet(style);
    showTS();
}

void Widget::on_toolButton_color_5_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(4,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_5->setStyleSheet(style);
    showTS();
}

void Widget::on_toolButton_color_6_clicked()
{
    QColor temp = QColorDialog::getColor();
    color.replace(5,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_color_6->setStyleSheet(style);
    showTS();
}

void Widget::on_horizontalScrollBar_index_valueChanged(int value)
{
    index = value;
    setdatas();
}

void Widget::mouseMoveEvent_TS(QMouseEvent *e)
{
    QString temp = tr("(") + QString::number(ui->widget_ts->xAxis->pixelToCoord(e->x())) +
            tr(",") + QString::number(ui->widget_ts->yAxis->pixelToCoord(e->y())) + tr(")") ;
    coor_TS->setColor(Qt::black);
    coor_TS->setText(temp);
    coor_TS->position->setCoords(ui->widget_ts->xAxis->pixelToCoord(e->x()),ui->widget_ts->yAxis->pixelToCoord(e->y()));
    coor_TS->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
    ui->widget_ts->replot();
}

void Widget::setlength()
{
    length = ui->comboBox_scans->currentText().toInt();
    setdatas();
}

void Widget::axisDoubleClick_TS(QCPAxis *a, QCPAxis::SelectablePart, QMouseEvent *e)
{
    if(ui->checkBox_AutoAxis_ts->isChecked())
    {
        return;
    }
    bool ok = true;
    if(e->button() & Qt::LeftButton)
    {
        if(a == ui->widget_ts->xAxis)
        {
            tsxmin = QInputDialog::getDouble(this,tr("Input the min of xAxis"),tr("Xmin"),tsxmin,-3600,3600,4,&ok);
            tsxmax = QInputDialog::getDouble(this,tr("Input the max of xAxis"),tr("Xmax"),tsxmax,-3600,3600,4,&ok);
        }
        else if(a == ui->widget_ts->yAxis)
        {
            tsymin = QInputDialog::getDouble(this,tr("Input the min of yAxis"),tr("Ymin"),tsymin,-65535,65535,0,&ok);
            tsymax = QInputDialog::getDouble(this,tr("Input the max of yAxis"),tr("Ymax"),tsymax,-65535,65535,0,&ok);
        }
    }
    showTS();
}

void Widget::showclb()
{
    CLB tempclb(currentfilename);
    //qDebug()<<"CLB"<<tempclb.HW;
    QVector < Info > infos = tempclb.information();
    ui->widget_clbamp->clearGraphs();
    ui->widget_clbpha->clearGraphs();
    ui->widget_clbamp->setInteractions(QCP::iRangeDrag
                                       | QCP::iRangeZoom
                                       | QCP::iMultiSelect
                                       | QCP::iSelectOther
                                       | QCP::iSelectItems
                                       | QCP::iSelectPlottables
                                       );
    ui->widget_clbpha->setInteractions(QCP::iRangeDrag
                                       | QCP::iRangeZoom
                                       | QCP::iMultiSelect
                                       | QCP::iSelectOther
                                       | QCP::iSelectItems
                                       | QCP::iSelectPlottables
                                       );
    QVector < double > fre;
    QVector < double > amp1,amp2,amp3,amp4,amp5;
    QVector < double > pha1,pha2,pha3,pha4,pha5;
    fre.clear();
    amp1.clear();
    pha1.clear();
    amp2.clear();
    pha2.clear();
    amp3.clear();
    pha3.clear();
    amp4.clear();
    pha4.clear();
    amp5.clear();
    pha5.clear();
    for(int i=0;i<infos.count()/5;i++)
    {
        if(infos.at(i*5).fre>400)
        {
            break;
        }
        fre<<log10(infos.at(i*5).fre);
        amp1<<infos.at(i*5).Amp;
        amp2<<infos.at(i*5+1).Amp;
        amp3<<infos.at(i*5+2).Amp;
        amp4<<infos.at(i*5+3).Amp;
        amp5<<infos.at(i*5+4).Amp;
        pha1<<infos.at(i*5).pha;
        pha2<<infos.at(i*5+1).pha;
        pha3<<infos.at(i*5+2).pha;
        pha4<<infos.at(i*5+3).pha;
        pha5<<infos.at(i*5+4).pha;
    }
    ui->widget_clbamp->xAxis->setLabel("f/Hz");
    ui->widget_clbpha->xAxis->setLabel("f/Hz");

    int graphcount = 0;
    if(ui->checkBox_clb1->isChecked())
    {
        ui->widget_clbamp->addGraph();
        ui->widget_clbpha->addGraph();
        ui->widget_clbamp->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbamp->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(0),8));
        ui->widget_clbpha->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbpha->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(0),8));
        ui->widget_clbamp->graph(graphcount)->setData(fre,amp1);
        ui->widget_clbpha->graph(graphcount)->setData(fre,pha1);
        graphcount++;
    }
    if(ui->checkBox_clb2->isChecked())
    {
        ui->widget_clbamp->addGraph();
        ui->widget_clbpha->addGraph();
        ui->widget_clbamp->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbamp->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(1),8));
        ui->widget_clbpha->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbpha->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(1),8));
        ui->widget_clbamp->graph(graphcount)->setData(fre,amp2);
        ui->widget_clbpha->graph(graphcount)->setData(fre,pha2);
        graphcount++;
    }
    if(ui->checkBox_clb3->isChecked())
    {
        ui->widget_clbamp->addGraph();
        ui->widget_clbpha->addGraph();
        ui->widget_clbamp->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbamp->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(2),8));
        ui->widget_clbpha->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbpha->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(2),8));
        ui->widget_clbamp->graph(graphcount)->setData(fre,amp3);
        ui->widget_clbpha->graph(graphcount)->setData(fre,pha3);
        graphcount++;
    }
    if(ui->checkBox_clb4->isChecked())
    {
        ui->widget_clbamp->addGraph();
        ui->widget_clbpha->addGraph();
        ui->widget_clbamp->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbamp->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(3),8));
        ui->widget_clbpha->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbpha->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(3),8));
        ui->widget_clbamp->graph(graphcount)->setData(fre,amp4);
        ui->widget_clbpha->graph(graphcount)->setData(fre,pha4);
        graphcount++;
    }
    if(ui->checkBox_clb5->isChecked())
    {
        ui->widget_clbamp->addGraph();
        ui->widget_clbpha->addGraph();
        ui->widget_clbamp->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbamp->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(4),8));
        ui->widget_clbpha->graph(graphcount)->setLineStyle(QCPGraph::lsNone);
        ui->widget_clbpha->graph(graphcount)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,clbcolor.at(4),8));
        ui->widget_clbamp->graph(graphcount)->setData(fre,amp5);
        ui->widget_clbpha->graph(graphcount)->setData(fre,pha5);
        graphcount++;
    }
    ui->widget_clbamp->rescaleAxes();
    ui->widget_clbpha->rescaleAxes();
    ui->widget_clbamp->xAxis->setRange(-2,3);
    ui->widget_clbpha->xAxis->setRange(-2,3);
    ui->widget_clbamp->yAxis->setRange(0,2);
    ui->widget_clbpha->yAxis->setRange(-180,180);
    ui->widget_clbamp->xAxis->setRangeReversed(true);
    ui->widget_clbpha->xAxis->setRangeReversed(true);
    ui->widget_clbamp->replot();
    ui->widget_clbpha->replot();
}

void Widget::showclc()
{
    CLC tempclc(currentfilename);
    //qDebug()<<"CLC"<<tempclc.HW;
    QVector < Info > infos = tempclc.information();
    ui->widget_clcamp->clearGraphs();
    ui->widget_clcpha->clearGraphs();
    ui->widget_clcamp->setInteractions(QCP::iRangeDrag
                                       | QCP::iRangeZoom
                                       | QCP::iMultiSelect
                                       | QCP::iSelectOther
                                       | QCP::iSelectItems
                                       | QCP::iSelectPlottables
                                       );
    ui->widget_clcpha->setInteractions(QCP::iRangeDrag
                                       | QCP::iRangeZoom
                                       | QCP::iMultiSelect
                                       | QCP::iSelectOther
                                       | QCP::iSelectItems
                                       | QCP::iSelectPlottables
                                       );
    QVector < double > fre;
    QVector < double > amp;
    QVector < double > pha;
    fre.clear();
    amp.clear();
    pha.clear();
    for(int i=0;i<infos.count();i++)
    {
        //        if(infos.at(i).fre>400)
        //        {
        //            break;
        //        }
        fre<<log10(infos.at(i).fre);
        amp<<infos.at(i).Amp;
        pha<<infos.at(i).pha;
        //qDebug()<<i<<infos.at(i).fre<<infos.at(i).Amp<<infos.at(i).pha;
    }
    ui->widget_clcamp->xAxis->setLabel("f/Hz");
    ui->widget_clcpha->xAxis->setLabel("f/Hz");
    ui->widget_clcamp->addGraph();
    ui->widget_clcpha->addGraph();
    ui->widget_clcamp->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget_clcamp->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));
    ui->widget_clcpha->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget_clcpha->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,8));
    ui->widget_clcamp->graph(0)->setData(fre,amp);
    ui->widget_clcpha->graph(0)->setData(fre,pha);
    ui->widget_clcamp->rescaleAxes();
    ui->widget_clcpha->rescaleAxes();
    ui->widget_clcamp->xAxis->setRange(-2,3);
    ui->widget_clcpha->xAxis->setRange(-2,3);
    ui->widget_clcamp->yAxis->setRange(0,2);
    ui->widget_clcpha->yAxis->setRange(-180,180);
    ui->widget_clcamp->xAxis->setRangeReversed(true);
    ui->widget_clcpha->xAxis->setRangeReversed(true);
    ui->widget_clcamp->replot();
    ui->widget_clcpha->replot();
}

void Widget::on_toolButton_clb1_clicked()
{
    QColor temp = QColorDialog::getColor();
    clbcolor.replace(0,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_clb1->setStyleSheet(style);
    showclb();
}

void Widget::on_toolButton_clb2_clicked()
{
    QColor temp = QColorDialog::getColor();
    clbcolor.replace(1,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_clb2->setStyleSheet(style);
    showclb();
}

void Widget::on_toolButton_clb3_clicked()
{
    QColor temp = QColorDialog::getColor();
    clbcolor.replace(2,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_clb3->setStyleSheet(style);
    showclb();
}

void Widget::on_toolButton_clb4_clicked()
{
    QColor temp = QColorDialog::getColor();
    clbcolor.replace(3,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_clb4->setStyleSheet(style);
    showclb();
}

void Widget::on_toolButton_clb5_clicked()
{
    QColor temp = QColorDialog::getColor();
    clbcolor.replace(4,temp);
    QString style = tr("QToolButton{background-color: rgb(") +
            QString::number(temp.red()) + tr(",") +
            QString::number(temp.green()) + tr(",") +
            QString::number(temp.blue()) + tr(");}");
    ui->toolButton_clb5->setStyleSheet(style);
    showclb();
}

QString Widget::Q2F(double temp, QString ef, int a, int b)
{
    QString string;
    if(ef=="f"||ef=="F")
    {
        string = QString::number(temp,'f',b);
    }
    else if(ef=="e"||ef=="E")
    {
        string = QString::number(temp,'e',b);
    }
    int length = string.count();
    if(a-length<=0)
    {
        string.clear();
        for(int i=0;i<a;i++)
        {
            string += "*";
        }
        return string;
    }
    for(int i=0;i<a-length;i++)
    {
        string.push_front(" ");
    }
    return string;
}

QString Widget::Q2F(int temp, int a)
{
    QString string = QString::number(temp);
    int length = string.count();
    if(a-length<=0)
    {
        string.clear();
        for(int i=0;i<a;i++)
        {
            string += "*";
        }
        return string;
    }
    for(int i=0;i<a-length;i++)
    {
        string.push_front(" ");
    }
    return string;
}

void Widget::on_pushButton_clicked()
{
    emit setTblInfo(tblinfo,currentfilename);
    return;
}
