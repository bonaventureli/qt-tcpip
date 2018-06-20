#ifndef CLIENT_H
#define CLIENT_H

#include<QDebug>

#include <QWidget>
#include<QApplication>
#include<QDataStream>
#include<QTcpSocket>
#include<QHostAddress>
#include<QStatusBar>
#include<QString>
#include<QLabel>
#include<QLineEdit>
#include<QSlider>
#include<QPushButton>

class Client : public QWidget
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = 0);

signals:

private slots:
    void connectToServer();
    void connectedToServer();
    void disConnectedFromServer();
    void sendData(int val);

private:
    QDataStream m_stream;
    QTcpSocket *m_socket;
    QLabel *m_statusLabel;
    QLineEdit *m_ipAdr;
    QLineEdit *m_portNr;
    QSlider *m_slider;
    QLabel *sliderVal;
    QPushButton *m_connectButton;
    QStatusBar *m_statusBar;


    QHostAddress m_ip;
    quint16 m_port;


    void setupWidget();
    void setupClient();
    void makeLineEdits();
    void makeConnectButton(bool m_connect);


};

#endif // CLIENT_H
