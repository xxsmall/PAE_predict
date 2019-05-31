#include "func.h"
#include "ui_func.h"

func::func(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::func)
{
    ui->setupUi(this);
}

func::~func()
{
    delete ui;
}

void func::writeFile(QList<QString> textList, QString fileName)
{
    //writetext
    if(textList.size()>0)
    {
       QFile file(fileName);
       bool ok;
       //ok=file.open( QIODevice::ReadWrite | QIODevice::Append);
       if(file.exists())
       {
           if(file.isOpen())
           {
               file.close();
           }
           file.remove();
       }
       ok=file.open( QIODevice::ReadWrite );
       if(ok)
       {
           QTextStream out(&file);

           for(int i=0;i<textList.size();i++)
           {
               out << textList[i];
           }

           file.close();
          // QMessageBox::information(this,"info",fileName+" create success!");
       }else
       {
            QMessageBox::information(this,"info","can not open file");
       }
    }else
    {
        QMessageBox::information(this,"info","no data need to write");
    }
}

QList<QString>  func::readFile(QString fileName)
{
    QFile file(fileName);
    QList<QString> dataList;
    dataList.clear();

    if(file.isOpen())
    {
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly))
    {
         return dataList;
    }
    QTextStream in(&file);

    int i=0;
    while(!in.atEnd())
    {
        QString onecount = in.readLine();
        i=i+1;
        dataList.append(onecount);
        //qDebug() << i <<"  "<< onecount;
    }
    file.close();
    return dataList;

}


QString  func::removeMoreSpace(QString str) //将每行连续的空格变为单个空格
{
    int size = str.length();
    QChar  a;
    QString strOut="";
    bool pastIsSpace=false;
    bool nowIsSpace=false;
    for(int i=0 ;i<size;i++)
    {

        a=str[i];
        if(a==' ')
        {
            nowIsSpace = true;
        }else
        {
            nowIsSpace = false;
        }

        if(nowIsSpace && pastIsSpace)
        {

        }else
        {
           strOut=strOut + a;
        }

        pastIsSpace = nowIsSpace ;
    }
    //qDebug()<<strOut;
    return strOut;

}

QString  func::strToNwidth(int needWidth,QString addDirection,QString oldStr)
{
     int size = oldStr.size();
     if(size >= needWidth || needWidth<=0)
     {
         return oldStr;
     }

     QString newStr = "";
     QString strSpace = " ";
     if(needWidth > size)
     {
         newStr = oldStr ;
         for(int i=0;i<(needWidth-size);i++)
         {
             if(addDirection.toUpper() == "LEFT")
             {
                 newStr=strSpace + newStr;
             }else
             {
                 newStr= newStr + strSpace ;
             }
         }
     }

     return newStr;

}

QString   func::getTableData(int row,int col ,QAbstractItemModel* modelTemp)
{
    QAbstractItemModel   *model= modelTemp;

    QModelIndex  index=model->index(row,col);

    QVariant data=model->data(index);


    QString str=data.toString();

    return str;


}

QString   func::getNotepadPath()
{
    QString endStr = "notepad";
     QList<QString> pathList;
     pathList.clear();
     QString str = QCoreApplication::applicationDirPath();

     QString fileName = str+"/notepad.txt";
     QFile a(fileName);
     if(a.exists())
     {
         pathList = readFile(fileName);
         if(pathList.size()>0)
         {
             endStr = pathList[0];
             return endStr;

         }else
         {
             QMessageBox::information(this,"error","notepad.txt has error!" );
             return endStr;
         }
     }else
     {
         QMessageBox::information(this,"error","notepad.txt not exists!" );
         return  endStr;
     }
}

double func::jde(int Y,int M,int D,int hour,int min,float sec)
{
    long int f,g,mid1,mid2;
    double J,JDE,A;

    if(M>=3)
    {
        f=Y;
        g=M;
    }

    if(M==1||M==2)
    {
        f=Y-1;
        g=M=12;
    }

    mid1=floor(365.25*f);
    mid2=floor(30.6001*(g+1));
    A=2-floor(f/100.0)+floor(f/400.0);
    J=mid1+mid2+D+A+1720994.5;
    JDE=J+hour/24.0+min/1440.0+sec/86400.0;
    return JDE;
}

QDate func::getDateFromStr(QString year,QString month,QString day)//转成QDate类型
{
    QDate date;
    int y = year.toInt();
    int m = month.toInt();
    int d = day.toInt();
    date.setDate(y,m,d);
    return date;
}


QTime func::getTimeFromStr(QString hour,QString min,QString second)//转成QTime类型，秒是带.需要特别处理
{
    QTime time;
    int h = hour.toInt();
    int m = min.toInt();
    int s=0;
    int ms=0;

    if(second.contains(".")) //second = “5.21”；要进行分割提取s和ms
    {
//        QStringList strList =  second.split(".");
//        QString strS = strList[0];
//        QString strMS = strList[1];
        double f_second = second.toDouble();
        s = int(f_second);
        double f_ms = f_second - s ;
        ms = int(f_ms*1000);

//        s = strS.toInt();
//        ms = strMS.toInt();
    }else                 //second = "5" ;没有 “.” 所以ms=0；
    {
        s = second.toInt();
        ms = 0;
    }

    time.setHMS(h,m,s,ms);
    return time;

}

QString func::removeNotNumberChar(QString strOld)
{

    QList<QString> charList;
    charList.clear();

    charList.append(".");

    charList.append("0");
    charList.append("1");
    charList.append("2");
    charList.append("3");
    charList.append("4");

    charList.append("5");
    charList.append("6");
    charList.append("7");
    charList.append("8");
    charList.append("9");
    charList.append("-");

    int size = strOld.length();
    QString str = strOld;


    for(int i=0;i<size;i++)
    {
        QChar aa =   str[i];
        QString charStr = aa ;

        bool find = false;
        for(int j=0 ;j<charList.size();j++)
        {
            QString listI = charList[j];
            if(charStr == listI )
            {
                find = true;
                break ;
            }
        }

        if(!find)
        {
            str = str.replace(charStr," ");
        }
    }

    return str;

}

double    func::getAngle(double source)
{
    double need;
    need = 0;

    double dec = need /360;
    int a = (int)(dec) ;
    double angle = need - 360*a;
    return angle;

}

QDateTime    func::getTleTime(QString tleLine1)
{
    //完成tle第一行数据中年月日时分秒的提取
    QString str = tleLine1;
    if(str.length()>=69)
    {
        QString yearStr = "20"+str.mid(18,2);
        int year = yearStr.toInt();
        QDate date(year,1,1);  //取当年的1月1日作为参考点，计算其儒略日，
        //qDebug()<<date<<yearStr<<str.size();
        QString jsStr = str.mid(20,12);//获取天数和秒数
        double js = jsStr.toDouble();
        int epoch_juDay = date.toJulianDay()+(int)js-1; //计算历元时刻的儒略日
        QDate epochDate = QDate::fromJulianDay(epoch_juDay); //将儒略日转换为日期
        //qDebug()<<"js= "<<jsStr<<js<<epochDate;
        double secondSum = (js-(int)js)*24*3600;//获取积秒总数

        int hour=secondSum/3600;
        int minute = (secondSum-hour*3600)/60;
        int second = (secondSum-hour*3600-minute*60);
        int ms = (secondSum -(int)secondSum)*1000;
        QTime epochTime(hour,minute,second,ms);
        //qDebug()<<"ms="<<QString::number(secondSum,'f',3)<<hour<<minute<<second<<ms<<epochTime;
        QDateTime a;
        a.setDate(epochDate);
        a.setTime(epochTime);
        //qDebug()<<a;
        return a;

    }

    QDateTime a;
    return a;

}

double func::getTleMJD(QString tleLine1)
{
    double MJD =0;
    QString str = tleLine1;
    if(str.length()>=69)
    {
        QString yearStr = "20"+str.mid(18,2);
        int year = yearStr.toInt();
        QDate date(year,1,1);  //取当年的1月1日作为参考点，计算其儒略日，
        //qDebug()<<date<<yearStr<<str.size();
        QString jsStr = str.mid(20,12);//获取天数和秒数
        double js = jsStr.toDouble();
        MJD = date.toJulianDay() ;
        MJD = MJD+ js -1;
       // qDebug()<<"MJD="<<QString::number(MJD,'f',8)<<js;

    }

    return MJD;



}

qint64    func::getDateTimeMs(QDateTime startTime,QDateTime endTime)
{
    qint64  a;
    qint64  msStart = startTime.toMSecsSinceEpoch();
    qint64  msEnd = endTime.toMSecsSinceEpoch();
    a = msEnd - msStart;
    return a;

}
