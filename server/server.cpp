#include "server.h"


Server::Server(QWidget *parent) : QWidget(parent)
  ,m_server(new QTcpServer(this))
  ,m_socket(new QTcpSocket(this))
{
    setupWidget();
    setupServer();
    connect(m_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
}

void Server::slotNewConnection()
{
    //set up a socket (connection) with the contacting client
    m_socket = m_server->nextPendingConnection();

    m_stream.setDevice(m_socket);
    m_stream.setVersion(QDataStream::Qt_4_0);//make sure to use v4.0

    connect(m_socket,SIGNAL(readyRead()),this,SLOT(slotDisplayVal()));

}

void Server::slotDisplayVal()
{
    QString data;
    m_stream>>data;
    LCDDisplay->display(data.toInt());
}

void Server::setupServer()
{

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();//get all network addresses
    // use the first non-localhost IPv4 address
    int i = 0;
    for (i;i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())//not local and a valid ipv4
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;//use the first we find
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    if(!m_server->listen())//listen for incoming connections on the specified address and randomly assign port
    {
        QMessageBox::warning(this,"Server failure","no connections"+m_server->errorString());
        m_server->close();
        return;
    }

    //if all goes well the server should listen on port x and notify user

    m_statusLabel->setText("Server connection: \nIPv4: "+ipAddress+"\nPort: "+QString::number(m_server->serverPort()));


}

void Server::setupWidget()
{
    setFixedSize(512,512);
    m_statusLabel = new QLabel("Starting server",this);
    m_statusLabel->setGeometry(100,10,500,100);

    LCDDisplay = new QLCDNumber(this);
    LCDDisplay->setGeometry((256-150),120,300,150);

    QPushButton *closeButton = new QPushButton(this);
    closeButton->setText("Close");
    closeButton->setGeometry((256-(130/2)),120+150+20,130,80);
    connect(closeButton,SIGNAL(clicked(bool)),QApplication::instance(),SLOT(quit()));
}
