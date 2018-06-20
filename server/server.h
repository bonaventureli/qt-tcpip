#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include<QApplication>
#include<QDataStream>
#include<QTcpServer>
#include<QTcpSocket>
#include<QNetworkInterface>
#include<QString>
#include<QList>
#include<QLabel>
#include<QMessageBox>
#include<QLCDNumber>
#include<QPushButton>

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = 0);

signals:

private slots:
    void slotNewConnection();
    void slotDisplayVal();
private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
    QDataStream m_stream;

    QLabel *m_statusLabel;
    QLCDNumber *LCDDisplay;


    void setupServer();
    void setupWidget();


};

#endif // SERVER_H
