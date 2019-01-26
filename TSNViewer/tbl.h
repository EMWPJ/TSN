#ifndef TBL_H
#define TBL_H
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QVector>
#include <QDebug>

typedef struct
{
public:
    char name[5];
    qint16 control_int;
    qint32 control_qint32;
    char type[1];
    char unknow[13];
public:
    void setUnknow(qint32 data)
    {
        memcpy(unknow,&data,4);
    }

    void setUnknow(double data)
    {
        memcpy(unknow,&data,8);
    }

    void setUnknow(QString data)
    {
        QByteArray temp = data.toLatin1();
        char *temp1 = temp.data();
        memcpy(unknow,temp1,13);
    }

    void setUnknow(QDateTime data)
    {
        unknow[0] = data.time().second();
        unknow[1] = data.time().minute();
        unknow[2] = data.time().hour();
        unknow[3] = data.date().day();
        unknow[4] = data.date().month();
        unknow[5] = data.date().year()-2000;
    }
}Tbl_Line;



inline QDataStream &operator<<(QDataStream &out,const Tbl_Line &tbl)
{
    out.setByteOrder(QDataStream::BigEndian);
    out.writeRawData(tbl.name,5);
    out.setByteOrder(QDataStream::LittleEndian);
    out<<tbl.control_int<<tbl.control_qint32;
    out.setByteOrder(QDataStream::BigEndian);
    out.writeRawData(tbl.type,1);
    out.writeRawData(tbl.unknow,13);
    return out;
}

inline QDataStream &operator>>(QDataStream &in,Tbl_Line &tbl)
{
    in.setByteOrder(QDataStream::BigEndian);
    in.readRawData(tbl.name,5);
    in.setByteOrder(QDataStream::LittleEndian);
    in>>tbl.control_int>>tbl.control_qint32;
    in.setByteOrder(QDataStream::BigEndian);
    in.readRawData(tbl.type,1);
    in.readRawData(tbl.unknow,13);
    return in;
}

class TBL
{
private:
    QString filePath;

public:
    QVector < Tbl_Line > tblLines;
    qint32 INIT;   //  初始化状态
    qint32 RQST;  // 控制MODE值
    qint32 MODE;  // 工作模式
    qint32 AQST;  // 采集状态
    qint32 SNUM;  // MTU序列
    QString VER;  // 软件版本号
    QString HW;  // 盒子硬件配置
    QString SITE; // 测点号
    QString CMPY; // 施工单位
    QString SRVY; // 勘探位置
    qint32 ATYP; // 装置类型
    qint32 NREF; // 磁北极参考值
    QString FILE; // 数据文件名
    QDateTime NUTC; // 格林威治时间
    QDateTime STIM; // 采集开始时间
    QDateTime ETIM; // 采集结束时间
    QDateTime HTIM; // 高频开始时间
    QDateTime ETMH; //高频结束时间
    qint32 HSMP; // 高频采样间隔
    qint32 SGIN; // 信号输入源参数
    qint32 EGN; // 电场通道增益
    qint32 HGN; // 磁场通道增益
    qint32 ACDC; // 电场耦合装置
    qint32 ACDH; // 磁场耦合装置
    qint32 LPFR; // 低通滤波参数
    qint32 LFRQ; // 工业频率
    qint32 V5SR; // V5采样率
    qint32 L3NS; // L3数据采集量
    qint32 L4NS; // L4数据采集量
    qint32 SRL3; // L3数据采样率
    qint32 SRL4; // L4数据采样率
    qint32 SRL5; // L5数据采样率
    qint32 CHEX; // EX通道号
    qint32 CHEY; // EY通道号
    qint32 CHEZ; // EZ通道号
    qint32 CHHX; // HX通道号
    qint32 CHHY; // HY通道号
    qint32 CHHZ; // HZ通道号
    double EXAC; // EX交流电位
    double EXDC; // EX直流电位
    double EYAC; // EY交流电位
    double EYDC; // EY直流电位
    double HXAC; // EX交流电位
    double HXDC; // EX直流电位
    double HYAC; // EX交流电位
    double HYDC; // EX直流电位
    double HZAC; // EX交流电位
    double HZDC; // EX直流电位
    qint32 EXR; // Ex电阻率
    qint32 EYR; // Ey电阻率
    double EAZM; // Ex方位角
    double EXLN; // Ex长度
    double EYLN; // Ey长度
    double HAZM; // Hx方位角
    QString HXSN; // Hx探头序列号
    QString HYSN; // Hy探头序列号
    QString HZSN; // Hz探头序列号
    qint32 TOTL; // 总记录数
    qint32 BADR; // 坏记录数
    qint32 SATR; // 饱和记录数
    qint32 NSAT; // 获取卫星数
    QDateTime LFIX; // 卫星锁定时间
    qint32 CLST; // 时钟状态
    qint32 OCTR; // 晶振记录数
    qint32 TERR; // 采样时间误差
    qint32 ELEV; // 高程值
    QString LATG; // 纬度值
    QString LNGG; // 经度值
    qint32 XDOS; // 采集完毕状态值
    qint32 CALS; // 盒子标定状态
    qint32 CCLS; // 探头标定状态
    QString CALR; // 标定结果
    qint32 CCLT; // 探头标定时间乘数
    double CFMN; // 最低角频率值
    double CFMX; // 最高角频率值
    double CCMN; // 探头最低频率
    double CCMX; // 探头最高频率
    double HATT; // 探头衰减系数
    double HNOM; // 探头额定增益
    double HAMP; // 探头测试振幅
    qint32 TCMB; // 梳状滤波器型号
    qint32 TALS; // 锯齿滤波器型号
    double CPHC; // 磁棒高通夹角频率
    qint32 BAT1; // 1号电池电压
    qint32 BAT2; // 2号电池电压
    qint32 BAT3; // 3号电池电压
    qint32 TEMP; // 内部温度值
    qint32 GFPG; // GPS板返回值
    qint32 FFPG; // 前置板返回值
    qint32 DSP; // DSP返回值
    qint32 DISK;// 可用磁盘空间
    QString CPTH; // 标定文件路径
    QString DPTH;// 数据文件路径
    qint32 TXPR; // 极化信号周期
    double FSCV;// 参考电压

public:
    TBL();
    TBL(QString filename);
    void setFileName(QString filename);
    bool read();
    bool write();
    QDateTime startTime();  //采样开始时间
    QDateTime endTime();    //采样结束时间
    QDateTime highFreStartTime();   //高频开始时间
    QDateTime highFreEndTime(); //高频结束时间
    qint32 highFreSamp();   //高频采样间隔
    qint32 MTU();       //MTU序列号
    qint32 E_Gain();      //电场通道增益
    qint32 H_Gain();      //磁场通道增益
    qint32 L3Datas();     //L3数据采集量
    qint32 L4Datas();     //L4数据采集量
    qint32 L3SamplingRate();  //L3数据采样率
    qint32 L4SamplingRate();  //L4数据采样率
    qint32 L5SamplingRate();  //L5数据采样率
    double ExAzimuth(); //Ex方位角
    double ExLength();  //Ex长度
    double EyLength();  //Ey长度
    QString HxCoil();   //Hx探头序列号
    QString HyCoil();   //Hy探头序列号
    QString HzCoil();   //Hz探头序列号
    qint32 totalRecords();    //总记录数
    qint32 badRecords();      //坏记录数
    qint32 saturationRecords();   //饱和记录数
    qint32 elevation();    //高程
    double latitude();  //纬度
    double longitude(); //经度
    double attenuationCoefficient();    //探头衰减系数
    double ratedGain();     //探头额定增益
};




#endif // TBL_H
