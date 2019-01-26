#include "fftw_parameter.h"
#include "ui_fftw_parameter.h"

FFTW_Parameter::FFTW_Parameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFTW_Parameter)
{
    ui->setupUi(this);
    windows<<"Rectangle"<<"Hamming"<<"Hanning"<<"Blackman";
    ui->comboBox_windows->clear();
    ui->comboBox_windows->addItems(windows);
    ui->comboBox_windows->setCurrentIndex(2);
    this->setWindowTitle("FFTW");
}

FFTW_Parameter::~FFTW_Parameter()
{
    delete ui;
}

void FFTW_Parameter::setTblInfo(tblInfo temp, QString tsn)
{
    info = temp;
    tsn.remove(tsn.count()-1,1);
    tsn+="3";
    ui->label_TS_filename->setText(tsn);
    tsfilename = tsn;
    TSN ts(tsn);
    ftime = ts.begintime();
    ltime = ts.endtime();
    ui->label_ftime->setText(ftime.toString("yyyy/MM/dd hh:mm:ss"));
    ui->label_ltime->setText(ltime.toString("yyyy/MM/dd hh:mm:ss"));
    filepath = tsn;
    int path = filepath.lastIndexOf('/');
    filepath = filepath.left(path+1);
    if(QFile(filepath + "fre.txt").exists())
    {
        readfrefile(filepath + "fre.txt");
    }
    if(QFile(filepath + "freqs-times.txt").exists())
    {
        readfretimefile(filepath + "freqs-times.txt");
    }
    this->show();
}

void FFTW_Parameter::on_pushButton_frefile_clicked()
{
    QString frefilename = QFileDialog::getOpenFileName(this, tr("Open Frequency File"),
                                                       filepath,
                                                       tr("Frequency (*.*)"));
    readfrefile(frefilename);
    return;
}

void FFTW_Parameter::on_pushButton_fretimefile_clicked()
{
    QString fretimefilename = QFileDialog::getOpenFileName(this, tr("Open Freq-Times File"),
                                                           filepath,
                                                           tr("Freq-Times (*.*)"));
    readfretimefile(fretimefilename);
    return;
}

void FFTW_Parameter::readfrefile(QString filename)
{
    QFile frefile(filename);
    if(!frefile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    frequency.clear();
    QString frefiledatas = frefile.readAll();
    QStringList freslines = frefiledatas.split(QRegExp("[ \t\n]"),QString::SkipEmptyParts);
    for(int i=0;i<freslines.count();i++)
    {
        QString temp = freslines.at(i);
        frequency<<temp.toDouble();
    }
    frefile.close();
    ui->tableWidget_fre->clear();
    QStringList frehead;
    frehead.clear();
    frehead<<QStringLiteral("待求频率(Hz)");
    ui->tableWidget_fre->setHorizontalHeaderLabels(frehead);
    int count = frequency.count();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_fre->setRowCount(i+1);
        QTableWidgetItem *check = new QTableWidgetItem();
        check->setCheckState(Qt::Checked);
        check->setText(QString::number(frequency.at(i)));
        ui->tableWidget_fre->setItem(i,0,check);
    }
    return;
}

void FFTW_Parameter::readfretimefile(QString filename)
{
    QFile fretimefile(filename);
    if(!fretimefile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    while(!fretimefile.atEnd())
    {
        QString line = fretimefile.readLine();
        QStringList linelist = line.split(QRegExp("[ \t\n]"),QString::SkipEmptyParts);
        if(linelist.count()<2)
        {
            continue;
        }
        fretime temp;
        line = linelist.at(0);
        temp.begin = QDateTime::fromString(line,tr("yyyyMMdd_hh_mm_ss"));
        line = linelist.at(1);
        temp.end = QDateTime::fromString(line,tr("yyyyMMdd_hh_mm_ss"));
        line = linelist.at(2);
        temp.fre = line.toDouble();
        freqs_times<<temp;
    }
    fretimefile.close();
    ui->tableWidget_fretime->clear();
    QStringList fretimehead;
    fretimehead.clear();
    fretimehead<<QStringLiteral("发射频率")<<QStringLiteral("起始时间")<<QStringLiteral("结束时间");
    ui->tableWidget_fretime->setHorizontalHeaderLabels(fretimehead);
    int count = freqs_times.count();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_fretime->setRowCount(i+1);
        QTableWidgetItem *check = new QTableWidgetItem();
        if(ftime.secsTo(freqs_times.at(i).begin)>0
                &&freqs_times.at(i).end.secsTo(ltime)>0)
        {
            check->setCheckState(Qt::Checked);
        }
        else
        {
            check->setCheckState(Qt::Unchecked);
        }
        check->setText(QString::number(freqs_times.at(i).fre));
        ui->tableWidget_fretime->setItem(i,0,check);
        ui->tableWidget_fretime->setItem(i,1,new QTableWidgetItem(freqs_times.at(i).begin.toString("yyyy/MM/dd hh:mm:ss")));
        ui->tableWidget_fretime->setItem(i,2,new QTableWidgetItem(freqs_times.at(i).end.toString("yyyy/MM/dd hh:mm:ss")));
    }
    return;
}

void FFTW_Parameter::on_pushButton_saveFreq_clicked()
{
    getfreinfo();
    QString frefilename = QFileDialog::getOpenFileName(this, tr("Save Frequency File"),
                                                       filepath,
                                                       tr("Frequency (*.*)"));
    savefrefile(frefilename);
}

void FFTW_Parameter::on_pushButton_saveFreqTime_clicked()
{
    getfreinfo();
    QString fretimefilename = QFileDialog::getOpenFileName(this, tr("Save Freq-Times File"),
                                                           filepath,
                                                           tr("Freq-Times (*.*)"));
    savefretimefile(fretimefilename);
}

void FFTW_Parameter::savefrefile(QString filename)
{
    int count = frequency.count();
    if(count<=0)
    {
        return;
    }
    QFile frefile(filename);
    if(!frefile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QString line;
    line.clear();
    for(int i=0;i<count;i++)
    {
        line += QString::number(frequency.at(i)) + tr("\n");
    }
    frefile.write(line.toLatin1().data());
    frefile.close();
    return;
}

void FFTW_Parameter::savefretimefile(QString filename)
{
    int count = freqs_times.count();
    if(count<=0)
    {
        return;
    }
    QFile fretimefile(filename);
    if(!fretimefile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QString line;
    line.clear();
    for(int i=0;i<count;i++)
    {
        line += freqs_times.at(i).begin.toString(tr("yyyyMMdd_hh_mm_ss")) + tr("\t")
                +freqs_times.at(i).end.toString(tr("yyyyMMdd_hh_mm_ss")) + tr("\t")
                +QString::number(freqs_times.at(i).fre) + tr("\n");
    }
    fretimefile.write(line.toLatin1().data());
    fretimefile.close();
    return;
}

void FFTW_Parameter::getfreinfo()
{
    int frecount = ui->tableWidget_fre->rowCount();
    if(frecount<=0)
    {
        return;
    }
    frequency.clear();
    for(int i=0;i<frecount;i++)
    {
        QTableWidgetItem *check = ui->tableWidget_fre->item(i,0);
        if(check->checkState() == Qt::Checked)
        {
            frequency<<check->text().toDouble();
        }
    }
    int freqtimecount = ui->tableWidget_fretime->rowCount();
    if(freqtimecount<=0)
    {
        return;
    }
    freqs_times.clear();
    for(int i=0;i<freqtimecount;i++)
    {
        QTableWidgetItem *check = ui->tableWidget_fretime->item(i,0);
        if(check->checkState() == Qt::Checked)
        {
            fretime temp;
            temp.fre = check->text().toDouble();
            QTableWidgetItem *begin = ui->tableWidget_fretime->item(i,1);
            temp.begin = QDateTime::fromString(begin->text(),"yyyy/MM/dd hh:mm:ss");
            QTableWidgetItem *end = ui->tableWidget_fretime->item(i,2);
            temp.end = QDateTime::fromString(end->text(),"yyyy/MM/dd hh:mm:ss");
            freqs_times<<temp;
        }
    }
}

void FFTW_Parameter::on_pushButton_freadd_clicked()
{
    int count = ui->tableWidget_fre->rowCount();
    ui->tableWidget_fre->setRowCount(count + 1);
    QTableWidgetItem *check = new QTableWidgetItem();
    check->setCheckState(Qt::Unchecked);
    check->setText("1.000");
    ui->tableWidget_fre->setItem(count,0,check);
}

void FFTW_Parameter::on_pushButton_fretimeadd_clicked()
{
    int count = ui->tableWidget_fretime->rowCount();
    ui->tableWidget_fretime->setRowCount(count + 1);
    QTableWidgetItem *check = new QTableWidgetItem();
    check->setCheckState(Qt::Unchecked);
    check->setText("1.000");
    ui->tableWidget_fretime->setItem(count,0,check);
    ui->tableWidget_fretime->setItem(count,1,new QTableWidgetItem(QDateTime::currentDateTimeUtc().toString("yyyy/MM/dd hh:mm:ss")));
    ui->tableWidget_fretime->setItem(count,2,new QTableWidgetItem(QDateTime::currentDateTimeUtc().toString("yyyy/MM/dd hh:mm:ss")));
    return;
}

void FFTW_Parameter::on_pushButton_fredel_clicked()
{
    int index = ui->tableWidget_fre->currentRow();
    ui->tableWidget_fre->removeRow(index);
    return;
}

void FFTW_Parameter::on_pushButton_fretimedel_clicked()
{
    int index = ui->tableWidget_fretime->currentRow();
    ui->tableWidget_fretime->removeRow(index);
    return;
}

void FFTW_Parameter::on_pushButton_freall_clicked()
{
    int count = ui->tableWidget_fre->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_fre->itemAt(i,0)->setCheckState(Qt::Checked);
    }
    return;
}

void FFTW_Parameter::on_pushButton_fretimeall_clicked()
{
    int count = ui->tableWidget_fretime->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_fretime->item(i,0)->setCheckState(Qt::Checked);
    }
    return;
}

void FFTW_Parameter::on_pushButton_makeFFT_clicked()
{
    getfreinfo();
    YUP yup;
    QString yupname = info.tblpath;
    yupname.remove(".tbl");
    yup.name = yupname;
    yupname += ".yup";
    yup.setFileName(yupname);
    TBL tbl(info.tblpath);
    yup.latitude = tbl.latitude();
    yup.longitude = tbl.longitude();
    //qDebug()<<"L L"<<tbl.latitude()<<tbl.longitude();
    yup.chnames.clear();
    yup.chnames<<"EX"<<"EY"<<"HX"<<"HY"<<"HZ"<<"RXY"<<"RYX";
    yup.timefreqs.clear();
    tblRead tblread;
    tblread.setFileName(info);
    tblread.setFreqs(frequency);
    yup.freqs = frequency;
    int timecount = 0;
    TSN tstemp(tsfilename);

    yup.chcount = tstemp.channels() + 2;
    QDateTime ftime = tstemp.begintime();
    QDateTime ltime = tstemp.endtime();
    spectrumdatas_all.clear();
    emitfreq.clear();
    int tsindex,tslength;
    int length = ui->lineEdit_length->text().toInt();
    int win = ui->comboBox_windows->currentIndex();
    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(QObject::tr("FFT"));
    progress.setLabelText(QObject::tr("Making FFT..."));
    progress.setRange(0,freqs_times.count());
    progress.show();
    QCoreApplication::processEvents();
    for(int i=0;i<freqs_times.count();i++)
    {
        progress.setValue(i);
        if(progress.wasCanceled())
        {
            return;
        }
        qDebug()<<freqs_times.count()<<i;
        fretime temp = freqs_times.at(i);
        if(ftime.secsTo(temp.begin)>0
                &&temp.end.secsTo(ltime)>0)
        {
            timecount++;
            emitfreq<<temp.fre;
            tsindex = ftime.secsTo(temp.begin);
            tslength = temp.begin.secsTo(temp.end);
            while(tslength<=0)
            {
                tslength += 3600;
            }
            QVector < QVector < QVector < scatter > > > results;
            results.clear();
            for(int j=0;j<tslength-length;j+=length)
            {
                results << tblread.statistics(win,tsindex+j,length);
            }
            QVector < QVector < scatter > > result = statistical(results);
            spectrumdatas_all<<result;
            timeFreq temptimeFreq;
            temptimeFreq.start = temp.begin;
            temptimeFreq.end = temp.end;
            temptimeFreq.psds.clear();
            for(int j=0;j<result.count();j++)
            {
                QVector<double> tempvd;
                tempvd.clear();
                for(int k=0;k<result[j].count();k++)
                {
                    tempvd<<result[j][k].value;
                }
                temptimeFreq.psds<<tempvd;
            }
            QVector<double> tempvd;
            tempvd.clear();
            for(int k=0;k<result[0].count();k++)
            {
                tempvd<<(result[0][k].value/result[3][k].value)*0.2/frequency.at(k);
            }
            temptimeFreq.psds<<tempvd;
            tempvd.clear();
            for(int k=0;k<result[0].count();k++)
            {
                tempvd<<(result[1][k].value/result[2][k].value)*0.2/frequency.at(k);
            }
            temptimeFreq.psds<<tempvd;
            yup.timefreqs<<temptimeFreq;
        }
//        break;
    }
    progress.accept();
    yup.timecount = timecount;
    yupReader *yupreader = new yupReader();
    yup.write();
    yupreader->setYUPPath(yup.filePath);
    yupreader->show();
    return;
}

QVector < QVector < scatter > > FFTW_Parameter::statistical(QVector < QVector < QVector < scatter > > > input)
{
    QVector < QVector < scatter > > result;
    result.clear();
    int count = input.count();
    if(count<=0)
    {
        return result;
    }
    int count1 = input[0].count();
    int count2 = input[0][0].count();
    for(int i=0;i<count1;i++)
    {
        QVector < scatter > result1;
        result1.clear();
        for(int j=0;j<count2;j++)
        {
            QVector < scatter > temp;
            temp.clear();
            for(int k=0;k<count;k++)
            {
                temp<<input[k][i][j];
            }
            result1 << statistical1(temp);
        }
        result<<result1;
    }
    return result;
}


scatter FFTW_Parameter::statistical1(QVector < scatter > input)
{
    scatter result;
    int count = input.count();
    if(count<=0)
    {
        return result;
    }
    result.freq = input[0].freq;
    result.value = 0;
    for(int i=0;i<count;i++)
    {
        result.value += input[i].value;
    }
    result.value /= (double)count;
    QVector < double > variance;
    variance.clear();
    for(int i=0;i<count;i++)
    {
        variance << fabs((result.value - input[i].value)/result.value);
    }
    QVector < scatter > del;
    del.clear();
    for(int i=0;i<count;i++)
    {
        if(variance[i]<0.2)
        {
            del<<input[i];
        }
    }
    if(del.count()<=0)
    {
        return result;
    }
    result.value = 0;
    for(int i=0;i<del.count();i++)
    {
        result.value += del[i].value;
    }
    result.value /= (double)del.count();
    return result;
}
