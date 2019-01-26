#include "yupReader.h"
#include "ui_yupReader.h"

#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QFileDialog>
#include <QSettings>
#include <QtMath>

yupReader::yupReader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::yupReader)
{
    ui->setupUi(this);

    initPrams();
    regSwf();
}

yupReader::~yupReader()
{
    delete ui;
}

void yupReader::initPrams()
{
    setAcceptDrops(true);
    spectD = new statisticsSpecturmDisplay(this);
    ui->stackedWidget->addWidget(spectD);
    ui->stackedWidget->setCurrentWidget(spectD);

    currentIndex = 0;
    segment = 0;
    currentCh = 3;

    statisticTimeStyle = 0;

    unit = 0;

    stationName = QStringLiteral("0000");
    unitName = QStringLiteral("功率谱");
    chName = QStringLiteral("Hx");


    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(autoPlay()));

    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,5);


}

void yupReader::setIndex(int index, int total)
{
    ui->pushButton_index->setText(QString::number(index)+tr("/")+QString::number(total));
}

void yupReader::setYUPPath(QString path)
{
    this->setWindowTitle(path);
    if(path.isEmpty())
    {
        return;
    }
    QFile info(path);
    if(!info.exists())
    {
        return;
    }

    filePath = path;
    this->setWindowTitle(filePath);

    QFileInfo fileinfo(path);
    QString basename = fileinfo.baseName();
    stationName = basename;
    unitName = "功率谱";
    //unitName = "信噪比";
    chName = "Hx";
    refreshLabel();

    yupFile.setFileName(path);
    yupFile.read();
    QStringList chnames = yupFile.chnames;
    ui->comboBox->clear();
    ui->comboBox->addItems(chnames);
    currentIndex = 1;
    currentCh = 3;
    ui->comboBox->setCurrentIndex(currentCh-1);
    QApplication::processEvents();
    QVector<double> freqs = yupFile.freqs;
    QVector<timeFreq> timefreqs = yupFile.timefreqs;

    exch=999;
    eych=999;
    hxch=999;
    hych=999;

    for(int i=0;i<chnames.count();i++)
    {
        QString chname = chnames.at(i);
        if(chname=="Ex"||chname=="ex"||chname=="EX")
        {
            exch = i;
        }
        if(chname=="Ey"||chname=="ey"||chname=="EY")
        {
            eych = i;
        }
        if(chname=="Hx"||chname=="hx"||chname=="HX")
        {
            hxch = i;
        }
        if(chname=="Hy"||chname=="hy"||chname=="HY")
        {
            hych = i;
        }
    }

    if(exch>5)
    {
        qDebug()<<"Ex not find";
    }
    if(eych>5)
    {
        qDebug()<<"Ey not find";
    }
    if(hxch>5)
    {
        qDebug()<<"Hx not find";
    }
    if(hych>5)
    {
        qDebug()<<"Hy not find";
    }

    int chcount = yupFile.chcount;
    int timecount = yupFile.timecount;
    int freqcount = freqs.count();
    allData.resize(chcount);
    for(int i=0;i<chcount;i++)
    {
        allData[i].resize(timecount);
        for(int m=0;m<timecount;m++)
        {
            allData[i][m].resize(freqcount);
        }
    }

    for(int i=0;i<timecount;i++)
    {
        timeFreq current = timefreqs[i];
        for(int m=0;m<chcount;m++)
        {
            for(int n=0;n<freqcount;n++)
            {
                scatter point;
                point.time = current.start;
                point.freq = freqs[n];
//                if(m==exch||m==eych)
//                {
//                    point.value = current.psds[m][n]*(1000000);
//                }
//                else
                {
                    point.value = current.psds[m][n];
                }
                allData[m][i][n] = point;
            }
        }
    }

    QApplication::processEvents();
    getStatistics(currentCh,statisticTimeStyle);

    QString chname = ui->comboBox->currentText();
    if(chname=="Hx"||chname=="hx"||chname=="HX"||chname=="Hy"||chname=="hy"||chname=="HY"||chname=="Hz"||chname=="hz"||chname=="HZ")
    {
        if(unit==0)
        {
            spectD->setUnit((tr("功率谱(nt²/Hz)")));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱(nt/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="Ex"||chname=="ex"||chname=="EX"||chname=="Ey"||chname=="ey"||chname=="EY"||chname=="Ez"||chname=="ez"||chname=="EZ")
    {
        if(unit==0)
        {
            spectD->setUnit(tr("功率谱((mV/km)²/Hz)"));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱((mV/km)/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="RXY"||chname=="rxy"||chname=="Rxy"||chname=="RYX"||chname=="ryx"||chname=="Ryx")
    {
        spectD->setUnit(tr("Ω.m"));
        unitName = "视电阻率";
    }
    //spectD->setUnit(tr("信噪比"));
    //unitName = "信噪比";
    refreshLabel();
}

void yupReader::getStatistics(int ch,int statististyle)
{
    scatters = allData[ch-1];
    orgScatters = scatters;
    scatters = statistis(orgScatters,statististyle);
    segment = scatters.count();
    setIndex(currentIndex,segment);
    setDisplay(currentIndex);
}

void yupReader::setDisplay(int index)
{
    if(scatters.count()<=0)
    {
        qDebug()<<"statisticsWaveDialog setDisplay scatters.count()<=0";
        return;
    }
    if(index<=0)
    {
        index=1;
        currentIndex = 1;
    }
    if(index>segment)
    {
        index=1;
        currentIndex = 1;
    }

    QString text;
    if(scatters[index-1].count()<=0)
    {
        qDebug()<<"statisticsWaveDialog setDisplay scatters[index-1].count()<=0";
        return;
    }
    QDateTime starttime = yupFile.timefreqs[index-1].start;
    QDateTime endtime = yupFile.timefreqs[index-1].end;
    starttime = starttime.addSecs(8*60*60);
    endtime = endtime.addSecs(8*60*60);
    QString freqlabel;
    for(int i=0;i<tables.count();i++)
    {
        if((starttime==tables[i].start)&&(endtime==tables[i].end))
        {
            if(tables[i].freq>0)
            {
                freqlabel = QString::number(tables[i].freq)+"Hz";
            }
            else
            {
                freqlabel = "底噪";
            }
            break;
        }
    }
    //qDebug()<<freqlabel<<starttime<<endtime;
    QString season;
    int month = starttime.date().month();
    switch(month)
    {
    case 12: season = tr("冬（12月、1月、2月）");break;
    case 1: season = tr("冬（12月、1月、2月）");break;
    case 2: season = tr("冬（12月、1月、2月）");break;
    case 3: season = tr("春（3月、4月、5月）");break;
    case 4: season = tr("春（3月、4月、5月）");break;
    case 5: season = tr("春（3月、4月、5月）");break;
    case 6: season = tr("夏（6月、7月、8月）");break;
    case 7: season = tr("夏（6月、7月、8月）");break;
    case 8: season = tr("夏（6月、7月、8月）");break;
    case 9: season = tr("秋（9月、10月、11月）");break;
    case 10: season = tr("秋（9月、10月、11月）");break;
    case 11: season = tr("秋（9月、10月、11月）");break;
    }

//    switch(statisticTimeStyle)
//    {
//    case 0: text = time.toString("yyyy年MM月dd日 hh:mm-")+time.addSecs(60*60*4-1).toString("hh:mm");break;
//    case 1: text = time.toString("yyyy年MM月dd日");break;
//    case 2: text = time.toString("yyyy年 第")+QString::number(time.date().weekNumber())+tr("周");break;
//    case 3: text = time.toString("yyyy年MM月");break;
//    case 4: text = time.toString("yyyy年 ")+season;break;
//    case 5: text = time.toString("yyyy年");break;
//    }
    text = starttime.toString("yyyy年MM月dd日 hh:mm:ss - ")+endtime.toString("yyyy年MM月dd日 hh:mm:ss ")+freqlabel;

    ui->label->setText(tr("  ")+text);
    //qDebug()<<"scatters[index-1] count ="<<scatters[index-1].count();

    QVector<scatter> result = setUnit(scatters[index-1],unit);
    spectD->setData(result);
    if(index)
    {
        setIndex(currentIndex,segment);
        setTableView(result);
    }
}

void yupReader::on_pushButton_left_clicked()
{
    if(currentIndex>1)
    {
        currentIndex--;
    }
    else
    {
        currentIndex = segment;
    }
    setDisplay(currentIndex);
}

void yupReader::on_pushButton_right_clicked()
{
    if(currentIndex<segment)
    {
        currentIndex++;
    }
    else
    {
        currentIndex = 1;
    }
    setDisplay(currentIndex);
}

void yupReader::on_pushButton_index_clicked()
{
    int index = QInputDialog::getInt(this,tr("查看段"),tr("段序号："),currentIndex,1,segment);
    if(index>0)
    {
        currentIndex = index;
        setDisplay(currentIndex);
    }
}

void yupReader::on_comboBox_activated(int index)
{
    QApplication::processEvents();
    currentCh = index+1;
    getStatistics(currentCh,statisticTimeStyle);
    QString chname = ui->comboBox->currentText();
    if(chname=="Hx"||chname=="hx"||chname=="HX"||chname=="Hy"||chname=="hy"||chname=="HY"||chname=="Hz"||chname=="hz"||chname=="HZ")
    {
        ui->comboBox_unit->show();
        if(unit==0)
        {
            spectD->setUnit((tr("功率谱(nt²/Hz)")));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱(nt/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="Ex"||chname=="ex"||chname=="EX"||chname=="Ey"||chname=="ey"||chname=="EY"||chname=="Ez"||chname=="ez"||chname=="EZ")
    {
        ui->comboBox_unit->show();
        if(unit==0)
        {
            spectD->setUnit(tr("功率谱((mV/km)²/Hz)"));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱((mV/km)/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="RXY"||chname=="rxy"||chname=="Rxy"||chname=="RYX"||chname=="ryx"||chname=="Ryx")
    {
        ui->comboBox_unit->hide();
        unit = 0;
        spectD->setUnit(tr("Ω.m"));
        unitName = "视电阻率";
    }
    chName = chname;
    refreshLabel();
    setDisplay(currentIndex);
}

void yupReader::on_pushButton_auto_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->pushButton_auto->setText(tr("自动播放"));
    }
    else
    {
        timer->start(500);
        ui->pushButton_auto->setText(tr("正在播放..."));
    }
}

void yupReader::autoPlay()
{
    if(currentIndex<segment)
    {
        currentIndex++;
    }
    else
    {
        currentIndex = 1;
    }
    setDisplay(currentIndex);
}

void yupReader::on_comboBox_statisticstime_activated(int index)
{
    //4小时 天 周 月 季度 年
    statisticTimeStyle = index;
    QApplication::processEvents();
    scatters = statistis(orgScatters,statisticTimeStyle);
    segment = scatters.count();
    setIndex(currentIndex,segment);
    setDisplay(currentIndex);
}

QVector< QVector< scatter > > yupReader::statistis(QVector< QVector< scatter > > temp,int style)
{
    QVector< QVector< scatter > > result;
    if(style==0)//4hours
    {
        result = temp;
    }
    if(style==1)//day
    {
        int count = temp.count();
        for(int i=0;i<count;i++)
        {
            for(int m=0;m<temp[i].count();m++)
            {
                QDateTime time = temp[i][m].time;
                temp[i][m].time = QDateTime(time.date(),QTime(0,0,0));
            }
        }

        QVector< QVector< QVector< scatter > > >statisticbuff;
        int diem = 0;
        statisticbuff.resize(diem+1);
        statisticbuff[diem] << temp[0];

        for(int i=1;i<temp.count();i++)
        {
            if(temp[i-1][0].time==temp[i][0].time)
            {
                statisticbuff[diem] << temp[i];
            }
            else
            {
                diem ++;
                statisticbuff.resize(diem+1);
                statisticbuff[diem] << temp[i];
            }
        }
        for(int i=0;i<statisticbuff.count();i++)
        {
            result << overlayCal(statisticbuff[i]);
        }
    }
    if(style==2)//week
    {
        QVector< QVector< QVector< scatter > > >statisticbuff;
        int diem = 0;
        statisticbuff.resize(diem+1);
        statisticbuff[diem] << temp[0];

        for(int i=1;i<temp.count();i++)
        {
            if((temp[i-1][0].time.date().year()==temp[i][0].time.date().year())&&(temp[i-1][0].time.date().weekNumber()==temp[i][0].time.date().weekNumber()))
            {
                statisticbuff[diem] << temp[i];
            }
            else
            {
                diem ++;
                statisticbuff.resize(diem+1);
                statisticbuff[diem] << temp[i];
            }
        }
        for(int i=0;i<statisticbuff.count();i++)
        {
            result << overlayCal(statisticbuff[i]);
        }
    }
    if(style==3)//month
    {
        int count = temp.count();
        for(int i=0;i<count;i++)
        {
            for(int m=0;m<temp[i].count();m++)
            {
                QDateTime time = temp[i][m].time;
                temp[i][m].time = QDateTime(QDate(time.date().year(),time.date().month(),1),QTime(0,0,0));
            }
        }

        QVector< QVector< QVector< scatter > > >statisticbuff;
        int diem = 0;
        statisticbuff.resize(diem+1);
        statisticbuff[diem] << temp[0];

        for(int i=1;i<temp.count();i++)
        {
            if(temp[i-1][0].time==temp[i][0].time)
            {
                statisticbuff[diem] << temp[i];
            }
            else
            {
                diem ++;
                statisticbuff.resize(diem+1);
                statisticbuff[diem] << temp[i];
            }
        }
        for(int i=0;i<statisticbuff.count();i++)
        {
            result << overlayCal(statisticbuff[i]);
        }
    }
    if(style==4)//season
    {
        int count = temp.count();
        for(int i=0;i<count;i++)
        {
            for(int m=0;m<temp[i].count();m++)
            {
                QDateTime time = temp[i][m].time;
                int month = time.date().month();
                switch(month)
                {
                case 12: month = 12;break;
                case 1: month = 12;break;
                case 2: month = 12;break;
                case 3: month = 3;break;
                case 4: month = 3;break;
                case 5: month = 3;break;
                case 6: month = 6;break;
                case 7: month = 6;break;
                case 8: month = 6;break;
                case 9: month = 9;break;
                case 10: month = 9;break;
                case 11: month = 9;break;
                }

                temp[i][m].time = QDateTime(QDate(time.date().year(),month,1),QTime(0,0,0));
            }
        }

        QVector< QVector< QVector< scatter > > >statisticbuff;
        int diem = 0;
        statisticbuff.resize(diem+1);
        statisticbuff[diem] << temp[0];

        for(int i=1;i<temp.count();i++)
        {
            if(temp[i-1][0].time==temp[i][0].time)
            {
                statisticbuff[diem] << temp[i];
            }
            else
            {
                diem ++;
                statisticbuff.resize(diem+1);
                statisticbuff[diem] << temp[i];
            }
        }
        for(int i=0;i<statisticbuff.count();i++)
        {
            result << overlayCal(statisticbuff[i]);
        }
    }
    if(style==5)//year
    {
        int count = temp.count();
        for(int i=0;i<count;i++)
        {
            for(int m=0;m<temp[i].count();m++)
            {
                QDateTime time = temp[i][m].time;
                temp[i][m].time = QDateTime(QDate(time.date().year(),1,1),QTime(0,0,0));
            }
        }

        QVector< QVector< QVector< scatter > > >statisticbuff;
        int diem = 0;
        statisticbuff.resize(diem+1);
        statisticbuff[diem] << temp[0];

        for(int i=1;i<temp.count();i++)
        {
            if(temp[i-1][0].time==temp[i][0].time)
            {
                statisticbuff[diem] << temp[i];
            }
            else
            {
                diem ++;
                statisticbuff.resize(diem+1);
                statisticbuff[diem] << temp[i];
            }
        }
        for(int i=0;i<statisticbuff.count();i++)
        {
            result << overlayCal(statisticbuff[i]);
        }
    }
    return result;
}

QVector< scatter > yupReader::overlayCal(QVector< QVector< scatter > > temp)
{
    QVector< scatter > result;
    int count = temp.count();
    if(count<=0)
    {
        qDebug()<<"statisticsWaveDialog overlayCal count<=0";
        return result;
    }
    if(count == 1)
    {
        qDebug()<<"statisticsWaveDialog overlayCal count == 1";
        return temp[0];
    }

    //search min size
    int min_size = temp[0].count();
    for(int i=1;i<count;i++)
    {
        if(temp[i].count()<min_size)
        {
            min_size = temp[i].count();
        }
    }

    result.resize(min_size);
    for(int i=0;i<min_size;i++)
    {
        result[i].value = 0;
    }

    for(int i=0;i<count;i++)
    {
        for(int m=0;m<min_size;m++)
        {
            result[m].freq = temp[i][m].freq;
            result[m].time = temp[i][m].time;
            result[m].value += double(temp[i][m].value)/count;
        }
    }

    return result;
}

void yupReader::setTableView(QVector< scatter > temp)
{
    ui->tableWidget->clear();
    QString chname = ui->comboBox->currentText();
    QString unit2;
    if(chname=="Hx"||chname=="hx"||chname=="HX"||chname=="Hy"||chname=="hy"||chname=="HY"||chname=="Hz"||chname=="hz"||chname=="HZ")
    {
        if(unit==0)
        {
            unit2 = (tr("功率谱(nt²/Hz)"));
        }
        if(unit==1)
        {
            unit2 = (tr("振幅谱(nt/sqrt(Hz))"));
        }
    }
    if(chname=="Ex"||chname=="ex"||chname=="EX"||chname=="Ey"||chname=="ey"||chname=="EY"||chname=="Ez"||chname=="ez"||chname=="EZ")
    {
        if(unit==0)
        {
            unit2 = (tr("功率谱((mV/km)²/Hz)"));
        }
        if(unit==1)
        {
            unit2 = (tr("振幅谱((mV/km)/sqrt(Hz))"));
        }
    }
    if(chname=="RXY"||chname=="rxy"||chname=="Rxy"||chname=="RYX"||chname=="ryx"||chname=="Ryx")
    {
        unit2 = (tr("Ω.m"));
    }
    QStringList header;
    header<<"频率(Hz)"<<unit2;
    ui->tableWidget->setHorizontalHeaderLabels(header);
    int count = temp.count();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget->setRowCount(i+1);
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Unchecked);
        double freq = temp[i].freq;
        check->setText(QString::number(freq));
        ui->tableWidget->setItem(i,0,check);
        double value = temp[i].value;
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(value)));
    }
}

void yupReader::on_comboBox_unit_activated(int index)
{
    //功率谱 振幅谱
    if(index<0)
    {
        return;
    }
    unit = index;
    setDisplay(currentIndex);
    QString chname = ui->comboBox->currentText();
    if(chname=="Hx"||chname=="hx"||chname=="HX"||chname=="Hy"||chname=="hy"||chname=="HY"||chname=="Hz"||chname=="hz"||chname=="HZ")
    {
        if(unit==0)
        {
            spectD->setUnit((tr("功率谱(nt²/Hz)")));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱(nt/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="Ex"||chname=="ex"||chname=="EX"||chname=="Ey"||chname=="ey"||chname=="EY"||chname=="Ez"||chname=="ez"||chname=="EZ")
    {
        if(unit==0)
        {
            spectD->setUnit(tr("功率谱((mV/km)²/Hz)"));
            unitName = "功率谱";
        }
        if(unit==1)
        {
            spectD->setUnit(tr("振幅谱((mV/km)/sqrt(Hz))"));
            unitName = "振幅谱";
        }
    }
    if(chname=="RXY"||chname=="rxy"||chname=="Rxy"||chname=="RYX"||chname=="ryx"||chname=="Ryx")
    {
        spectD->setUnit(tr("Ω.m"));
        unitName = "视电阻率";
    }
    refreshLabel();
}

QVector<scatter> yupReader::setUnit(QVector<scatter> buff, int unit)
{
    //功率谱 振幅谱
    if(unit==0)
    {
        return buff;
    }
    if(unit==1)
    {
        QVector<scatter> result;
        for(int i=0;i<buff.count();i++)
        {
            scatter point;
            point.time = buff[i].time;
            point.freq = buff[i].freq;
            point.value = sqrt(fabs(buff[i].value));
            result << point;
        }
        return result;
    }
    if(unit==2)
    {
        return buff;
    }
    return buff;
}

void yupReader::on_pushButton_refreshLabel_clicked()
{
    if(ui->tableWidget->rowCount()<=0)
    {
        return;
    }
    QVector<double> freqlabel;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QTableWidgetItem *check = ui->tableWidget->item(i,0);
        if(check->checkState()==Qt::Checked)
        {
            freqlabel << check->text().toDouble();
        }
    }
    spectD->setLabels(freqlabel);
}

void yupReader::refreshLabel()
{
    QString chname = ui->comboBox->currentText();
    //qDebug()<<chname;
    spectD->setText(stationName+"  "+unitName+" ("+chname+")");
}

void yupReader::on_comboBox_displaymode_activated(int index)
{
    spectD->setDisplayMode(index);
}

void yupReader::on_pushButton_markfreq_clicked()
{
    if(filePath.isEmpty())
    {
        return;
    }
    QFileInfo fileinfo(filePath);
    QString path = fileinfo.absolutePath();
    QString filename = QFileDialog::getOpenFileName(this,"选择需要标记的频率文件",path,tr("文本 (*.txt)"));
    if(filename.isEmpty())
    {
        return;
    }
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    markFreqs.clear();
    while(!file.atEnd())
    {
        QString line = file.readLine();
        line = line.remove("\n").remove(" ");
        if(line.toDouble()>0)
        {
            markFreqs << line.toDouble();
        }
    }
    spectD->setMarkFreqs(markFreqs);
}

void yupReader::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void yupReader::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    //往文本框中追加文件名
    foreach(QUrl url, urls) {
        QString itemtext = url.toLocalFile();
        QFileInfo fileinfo(itemtext);
        if(fileinfo.exists())
        {
            QString suffix = fileinfo.suffix();
            if(suffix.contains("yup")||suffix.contains("YUP"))
            {
                setYUPPath(itemtext);
                return;
            }
        }
    }
}

void yupReader::on_pushButton_open_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("选择YUP文件"),"",tr("YUP (*.yup)"));
    if(filepath.isEmpty())
    {
        return;
    }
    setYUPPath(filepath);
}

void yupReader::regSwf()
{
    QString appPath = qApp->applicationFilePath();
    // 注意路径的替换
    appPath.replace("/", "\\");
    QString type = "yupReader";
    QSettings *regType = new QSettings("HKEY_CLASSES_ROOT\\.yup", QSettings::NativeFormat);
    QSettings *regIcon = new QSettings("HKEY_CLASSES_ROOT\\.yup\\DefaultIcon", QSettings::NativeFormat);
    QSettings *regShell = new QSettings("HKEY_CLASSES_ROOT\\yupReader\\shell\\open\\command", QSettings::NativeFormat);

    regType->remove("Default");
    regType->setValue("Default", type);

    regIcon->remove("Default");
    // 0 使用当前程序内置图标
    regIcon->setValue("Default", appPath + ",0");

     // 百分号问题
    QString shell = "\"" + appPath + "\" ";
    shell = shell + "\"%1\"";
    regShell->remove("Default");
    regShell->setValue("Default", shell);
    delete regIcon;
    delete regShell;
    delete regType;
    // 通知系统刷新
    ::SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST|SHCNF_FLUSH, 0, 0);
}

void yupReader::on_pushButton_exportPic_clicked()
{
    int oldindex = currentIndex;
    int oldunit = ui->comboBox_unit->currentIndex();
    int oldch = ui->comboBox->currentIndex();
    int olddisplaymode = ui->comboBox_displaymode->currentIndex();
    currentIndex = segment;
    setDisplay(currentIndex);
    ui->comboBox_unit->setCurrentIndex(1);
    on_comboBox_unit_activated(1);
    int chcount = ui->comboBox->count();
    for(int i=0;i<chcount;i++)
    {
        ui->comboBox->setCurrentIndex(i);
        on_comboBox_activated(i);
        if(ui->comboBox->itemText(i)=="RXY"||ui->comboBox->itemText(i)=="RYX")
        {
            ui->comboBox_displaymode->setCurrentIndex(1);
            on_comboBox_displaymode_activated(1);
        }
        else
        {
            ui->comboBox_displaymode->setCurrentIndex(0);
            on_comboBox_displaymode_activated(0);
        }
        //Sleep(100);
        QFileInfo fileinfo(filePath);
        QString pic_file = fileinfo.absolutePath()+"/"+stationName+"-"+ui->comboBox->itemText(i)+".png";
        QPixmap p = spectD->getGrap();
        p.save(pic_file);
    }
    currentIndex = oldindex;
    setDisplay(currentIndex);
    ui->comboBox_unit->setCurrentIndex(oldunit);
    on_comboBox_unit_activated(oldunit);
    ui->comboBox->setCurrentIndex(oldch);
    on_comboBox_activated(oldch);
    ui->comboBox_displaymode->setCurrentIndex(olddisplaymode);
    on_comboBox_displaymode_activated(olddisplaymode);
}

void yupReader::readSettings()
{
    //QSettings settings()
}

void yupReader::saveSettings()
{

}

void yupReader::on_pushButton_importTime_clicked()
{
    QString timefreqslist = QFileDialog::getOpenFileName(this,"导入时间段文件","","文本 (*.txt)");
    if(timefreqslist.isEmpty())
    {
        return;
    }
    QFile file(timefreqslist);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    tables.clear();
    while(!file.atEnd())
    {
        QString line = file.readLine();
        //qDebug()<<line;
        QStringList list = line.split("\t");
        if(list.count()<3)
        {
            continue;
        }
        timetable current;
        current.start = QDateTime::fromString(list.at(0),"yyyyMMdd_hh_mm_ss");
        current.end = QDateTime::fromString(list.at(1),"yyyyMMdd_hh_mm_ss");
        QString freq = list.at(2);
        current.freq = freq.remove("\n").remove(" ").toDouble();
        qDebug()<<current.start<<current.end<<current.freq;
        tables << current;
    }
    file.close();
}

void yupReader::on_pushButton_exportCSV_clicked()
{
    int timefreqscount = yupFile.timefreqs.count();
    int nosieindex = timefreqscount;
    for(int i=0;i<timefreqscount;i++)
    {
        QDateTime starttime = yupFile.timefreqs[i].start;
        QDateTime endtime = yupFile.timefreqs[i].end;
        starttime = starttime.addSecs(8*60*60);
        endtime = endtime.addSecs(8*60*60);
        for(int m=0;m<tables.count();m++)
        {
            if((starttime==tables[m].start)&&(endtime==tables[m].end))
            {
                if(tables[m].freq==0)
                {
                    nosieindex = i;
                    break;
                }
            }
        }
    }
    if(nosieindex>=timefreqscount)
    {
        qDebug()<<"nosieindex>=timefreqscount"<<nosieindex<<timefreqscount;
        return;
    }
    int chcount = yupFile.chcount;
    int exindex = chcount;
    int eyindex = chcount;
    int hxindex = chcount;
    int hyindex = chcount;
    int hzindex = chcount;
    int rxyindex = chcount;
    int ryxindex = chcount;
    for(int i=0;i<yupFile.chnames.count();i++)
    {
        QString chname = yupFile.chnames[i];
        if(chname.contains("EX",Qt::CaseInsensitive))
        {
            exindex = i;
        }
        if(chname.contains("EY",Qt::CaseInsensitive))
        {
            eyindex = i;
        }
        if(chname.contains("HX",Qt::CaseInsensitive))
        {
            hxindex = i;
        }
        if(chname.contains("HY",Qt::CaseInsensitive))
        {
            hyindex = i;
        }
        if(chname.contains("HZ",Qt::CaseInsensitive))
        {
            hzindex = i;
        }
        if(chname.contains("RXY",Qt::CaseInsensitive))
        {
            rxyindex = i;
        }
        if(chname.contains("RYX",Qt::CaseInsensitive))
        {
            ryxindex = i;
        }
    }
    if(exindex>=chcount||eyindex>=chcount||hxindex>=chcount||hyindex>=chcount||hzindex>=chcount||rxyindex>=chcount||ryxindex>=chcount)
    {
        qDebug()<<"index error"<<exindex<<eyindex<<hxindex<<hyindex<<hzindex<<rxyindex<<ryxindex;
        return;
    }
    QFileInfo fileinfo(filePath);
    QString csv_file = fileinfo.absolutePath()+"/"+stationName+"-时间频率表.csv";
    qDebug()<<"CSV"<<csv_file;
    QFile csvfile(csv_file);
    if(!csvfile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    timeFreq noise = yupFile.timefreqs[nosieindex];
    QString Line = "STARTDATE,STARTTIME,ENDDATE,ENDTIME,FREQ,EX,EXSN,EY,EYSN,HX,HXSN,HY,HYSN,HZ,HZSN,RXY,RYX\n";
    for(int i=0;i<timefreqscount-1;i++)
    {
        if(i==nosieindex)
        {
            qDebug()<<"i==nosieindex"<<i<<nosieindex;
            continue;
        }
        QDateTime starttime = yupFile.timefreqs[i].start;
        QDateTime endtime = yupFile.timefreqs[i].end;
        starttime = starttime.addSecs(8*60*60);
        endtime = endtime.addSecs(8*60*60);
        double freq = 0;
        for(int m=0;m<tables.count();m++)
        {
            if((starttime==tables[m].start)&&(endtime==tables[m].end))
            {
                if(tables[m].freq>0)
                {
                     freq = tables[m].freq;
                     break;
                }
            }
        }
        if(freq<=0)
        {
            qDebug()<<"freq<=0"<<i<<freq;
            continue;
        }
        int freqindex = yupFile.freqs.count();
        for(int m=0;m<yupFile.freqs.count();m++)
        {
            if(freq == yupFile.freqs[m])
            {
                freqindex = m;
            }
        }
        if(freqindex>=yupFile.freqs.count())
        {
            qDebug()<<"freqindex>=yupFile.freqs.count()"<<i<<freqindex<<yupFile.freqs.count();
            continue;
        }
        double ex = qSqrt(yupFile.timefreqs[i].psds[exindex][freqindex]);
        double sn = qSqrt(noise.psds[exindex][freqindex]);
        double exsn = (ex-sn)/ex;
        if(exsn<0)
        {
            exsn = 0;
        }
        double ey = qSqrt(yupFile.timefreqs[i].psds[eyindex][freqindex]);
        sn = qSqrt(noise.psds[eyindex][freqindex]);
        double eysn = (ey-sn)/ey;
        if(eysn<0)
        {
            eysn = 0;
        }
        double hx = qSqrt(yupFile.timefreqs[i].psds[hxindex][freqindex]);
        sn = qSqrt(noise.psds[hxindex][freqindex]);
        double hxsn = (hx-sn)/hx;
        if(hxsn<0)
        {
            hxsn = 0;
        }
        double hy = qSqrt(yupFile.timefreqs[i].psds[hyindex][freqindex]);
        sn = qSqrt(noise.psds[hyindex][freqindex]);
        double hysn = (hy-sn)/hy;
        if(hysn<0)
        {
            hysn = 0;
        }
        double hz = qSqrt(yupFile.timefreqs[i].psds[hzindex][freqindex]);
        sn = qSqrt(noise.psds[hzindex][freqindex]);
        double hzsn = (hz-sn)/hz;
        if(hzsn<0)
        {
            hzsn = 0;
        }
        double rxy = yupFile.timefreqs[i].psds[rxyindex][freqindex];
        double ryx = yupFile.timefreqs[i].psds[ryxindex][freqindex];
        Line += starttime.toString("yyyy/MM/dd,hh:mm:ss")+",";
        Line += endtime.toString("yyyy/MM/dd,hh:mm:ss")+",";
        Line += QString::number(freq)+",";
        Line += QString::number(ex,'e',4)+","+QString::number(exsn*100,'f',2)+',';
        Line += QString::number(ey,'e',4)+","+QString::number(eysn*100,'f',2)+',';
        Line += QString::number(hx,'e',4)+","+QString::number(hxsn*100,'f',2)+',';
        Line += QString::number(hy,'e',4)+","+QString::number(hysn*100,'f',2)+',';
        Line += QString::number(hz,'e',4)+","+QString::number(hzsn*100,'f',2)+',';
        Line += QString::number(rxy,'e',4)+","+QString::number(ryx,'e',4)+"\n";
    }
    csvfile.write(Line.toLocal8Bit().data());
    csvfile.close();
}
