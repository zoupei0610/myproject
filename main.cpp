#include "mainwindow.h"
#include <QApplication>
#include <QSystemTrayIcon>
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // auto run when computer restart
    QSettings  reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    QString strAppPath=QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    reg.setValue("elink cloud",strAppPath);

    //read email from config file
    QString email_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/CPS_CLOUD_CONFIG.ini";
    QFile email_file(email_path);


    MainWindow w;
    w.show();
    //w.hide();
    if(email_file.open(QFile::ReadOnly))
    {
       w.hide();
    }
    return a.exec();
}
