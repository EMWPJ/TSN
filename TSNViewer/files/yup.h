#ifndef YUP_H
#define YUP_H
#include <QFile>
#include <QObject>
#include <QDebug>
#include <QDateTime>
class timeFreq
{
public:
    QDateTime start;//起始时间
    QDateTime end;//结束时间
    QVector< QVector<double> > psds;//功率谱
};

class YUP : public QObject
{
    Q_OBJECT
public:
    explicit YUP(QObject *parent = 0);
    explicit YUP(QString filename);

public:
    QString filePath;

public:
    QString name;//站点名
    double latitude;//纬度
    double longitude;//经度
    QVector<double> freqs;//频率表
    int timecount;//时间段数
    int chcount;//通道数
    QStringList chnames;//通道名称
    QVector<timeFreq> timefreqs;//不同时间的功率谱

public:
    void setFileName(QString filename);
    void write();
    void read();
};

#endif // YUP_H
