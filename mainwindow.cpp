#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    elink = new Elink();

    QString email_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/CPS_CLOUD_CONFIG.ini";
    QFile email_file(email_path);
    if(email_file.open(QFile::ReadOnly))
    {
        QTextStream out(&email_file);
        ui->lineEdit->setText(out.readLine());
    }
    email_file.close();

    setWindowFlags(this->windowFlags() &~  Qt::WindowMinMaxButtonsHint &~ Qt::WindowCloseButtonHint);
    setWindowFlags(Qt::FramelessWindowHint);
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/root/image/yichong.png"));
    m_systemTray->setToolTip("cps Programer service");
    m_systemTray->show();

    m_menu = new QMenu(this);

    m_action1 = new QAction(m_menu);
    m_action4 = new QAction(m_menu);
    m_action2 = new QAction(m_menu);
    m_action3 = new QAction(m_menu);

    m_action1->setText("打开Elink");
    m_action4->setText("隐藏Elink");
    m_action2->setText("帮助");
    m_action3->setText("退出");

    m_menu->addAction(m_action1);
    m_menu->addAction(m_action4);
    m_menu->addAction(m_action2);
    m_menu->addAction(m_action3);

    m_systemTray->setContextMenu(m_menu);

    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));//timeoutslot()为自定义槽

    connect(m_action1,&QAction::triggered,this,&MainWindow::action1_triggred);
    connect(m_action2,&QAction::triggered,this,&MainWindow::action2_triggred);
    connect(m_action3,&QAction::triggered,this,&MainWindow::action3_triggred);
    connect(m_action4,&QAction::triggered,this,&MainWindow::action4_triggred);
    connect(m_systemTray,&QSystemTrayIcon::activated,this,&MainWindow::activeTray);



    m_udp_clent = new UDPclent();
    m_udp_clent->InitSocket();

    timer->start(1000);



    //sample elink check
    qDebug()<<elink->check_device();



 //sample elink get data
 char read_buf[EP1_IN_SIZE];
 elink->get_eeprom_data(read_buf,32);
 for(int i = 0; i < 32; i++)
 {
     qDebug()<<QString("%1").arg(read_buf[i]&0xff,2,16);
 }

}

MainWindow::~MainWindow()
{
    delete ui;
}

static bool is_show = false;
void MainWindow::activeTray(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Context:
        qDebug()<<"c";
        break;
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
        if(is_show)
        {
            this->hide();
            is_show = false;
        }else
        {
            this->show();
            this->setFocus();
            is_show = true;
        }
        break;
    }
}

void MainWindow::action1_triggred()
{
    this->show();
    this->setFocus();
    is_show = true;
}

void MainWindow::action2_triggred()
{
   m_systemTray->showMessage("Service",
                             "this service help you program CPS IC",
                             QSystemTrayIcon::MessageIcon::Information,
                             7000);
}

void MainWindow::action3_triggred()
{
    this->close();
}

void MainWindow::action4_triggred()
{
   this->hide();
   is_show = false;
}

void MainWindow::on_pushButton_clicked()
{
   this->hide();
   is_show = false;
}

void MainWindow::timeoutslot()
{
    m_udp_clent->email=ui->lineEdit->text();

    if(elink->check_device())
    {

        if(m_udp_clent->status=="0")
        {
           m_udp_clent->status="2";
        }

        m_udp_clent->Send();

        if(m_udp_clent->Read())
        {
            QString str_buff=m_udp_clent->Buffer.data();
            QStringList lst=str_buff.split(';');

            QString status_str = lst.at(1);
            QString info_str = lst.at(2);
            info_str.simplified();
            status_str.simplified();
            m_udp_clent->info=info_str.mid(2,32);
            m_udp_clent->status=status_str.mid(2,1);

            QByteArray info_ba =info_str.toLatin1();
            char *write_buf=info_ba.data();

            qDebug()<<m_udp_clent->info;
            qDebug()<< m_udp_clent->status;


            if(m_udp_clent->status=="2")
            {
                char ret = 0;
                ret =elink->put_eeprom_data(write_buf,32);
                if(ret == -1)
                {
                    qDebug()<<"erase faile";
                    m_udp_clent->status="3";
                }
                else if(ret == -2)
                {
                    qDebug()<<"write faile";
                    m_udp_clent->status="2";

                }
                else
                {
                    qDebug()<<"write succeed";
                    m_udp_clent->status="2";
                }
            }

       }


    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text() == "")
    {
       QMessageBox::warning(this,"email error!!","make sure input valid email",QMessageBox::Yes);

    }else
    {
       //QMessageBox::about(this,"waiting connect","it will connect server");
        QString email_path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+tr("/CPS_CLOUD_CONFIG.ini");
        QFile email_file(email_path);
        if(email_file.open(QFile::WriteOnly| QFile::QFile::Text))
        {
            QTextStream out(&email_file);
            out<<ui->lineEdit->text();
            out<<'\n';
        }
        email_file.close();

    }
}
