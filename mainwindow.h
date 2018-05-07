#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QtNetwork/QUdpSocket>
#include <QHostAddress>
#include <QThread>
#include "udpclent.h"
#include <QTimer>
#include "elink.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void activeTray(QSystemTrayIcon::ActivationReason reason);
    void action1_triggred(void);
    void action2_triggred(void);
    void action3_triggred(void);
    void action4_triggred(void);
    void timeoutslot(void);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Elink *elink;

    QSystemTrayIcon *m_systemTray;
    QMenu *m_menu;
    QAction *m_action1;
    QAction *m_action2;
    QAction *m_action3;
    QAction *m_action4;
    QTimer  *timer;

    UDPclent *m_udp_clent;

};

#endif // MAINWINDOW_H
