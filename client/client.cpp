#include "client.h"

Client::Client(QWidget *parent) : QWidget(parent)
  ,m_connectButton(new QPushButton(this))
  ,m_socket(new QTcpSocket(this))
  ,m_slider(new QSlider(this))
  ,sliderVal(new QLabel(this))
  ,m_ipAdr(new QLineEdit(this))
  ,m_portNr(new QLineEdit(this))
  ,m_statusBar(new QStatusBar(this))
{
    setupWidget();
    setupClient();
}

void Client::connectToServer()
{
    connect(m_socket,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(disConnectedFromServer()));

    m_ip.setAddress(m_ipAdr->text());

    m_port = m_portNr->text().toUInt();

    m_socket->connectToHost(m_ip,m_port);
    if(m_socket->waitForConnected(5000))//give it 5 sec to connect
    {
        m_statusLabel->setText("Connected");

    }
    else
    {
        m_statusLabel->setText("Could not connect  "+m_socket->errorString());

    }

}

void Client::connectedToServer()
{
    m_ipAdr->close();
    m_portNr->close();

    makeConnectButton(false);

    m_slider->setRange(0,1000);
    m_slider->setGeometry(100,140,320,100);

    connect(m_slider,SIGNAL(valueChanged(int)),this,SLOT(sendData(int)));
}

void Client::disConnectedFromServer()
{
    m_slider->close();
    m_ipAdr->show();
    m_portNr->show();

    makeConnectButton(true);

    m_statusLabel->setText("Enter host ip and port nr ");
    makeLineEdits();
}

void Client::sendData(int val)
{
    //---------------local display------------
    sliderVal->setGeometry(100,80,320,50);
    sliderVal->setAlignment(Qt::AlignCenter);
    sliderVal->setText(QString::number(val));

    //---------------to server----------------
    QString dataToSend = QString::number(val);
    m_stream<<dataToSend;


}

void Client::setupWidget()
{
    setFixedSize(512,512);

    makeConnectButton(true);

    //--------------------closeButton--------------------------------------
    QPushButton *closeButton = new QPushButton(this);
    closeButton->setText("Close");
    closeButton->setGeometry(100+150+20,300,150,80);
    connect(closeButton,SIGNAL(clicked(bool)),QApplication::instance(),SLOT(quit()));


    //--------------------statuslabel----------------------------------------
    m_statusLabel = new QLabel(this);
    m_statusLabel->setText("Enter host ip and port nr ");
    m_statusLabel->setGeometry(50,10,450,80);

    makeLineEdits();



}

void Client::setupClient()
{
    //communication from socket

    m_stream.setDevice(m_socket);
    m_stream.setVersion(QDataStream::Qt_4_0);//make sure to use v4.0


}

void Client::makeLineEdits()
{

    //--------------------lineedit ip----------------------------------------
    m_ipAdr->setText("127.0.0.1");
    m_ipAdr->setGeometry(50,100,300,80);

    //--------------------lineedit port--------------------------------------
    m_portNr->setPlaceholderText("Port nr");
    m_portNr->setGeometry(50,190,300,80);
}

void Client::makeConnectButton(bool m_connect)
{

    m_connectButton->setGeometry(100,300,150,80);
    if(m_connect)
    {
        sliderVal->hide();
        m_slider->hide();
        //--------------------connectButton--------------------------------------

        m_connectButton->setText("Connect");

        disconnect(m_connectButton,SIGNAL(clicked(bool)),m_socket,SLOT(deleteLater()));
        connect(m_connectButton,SIGNAL(clicked(bool)),this,SLOT(connectToServer()));
    }
    else
    {
        sliderVal->show();
        m_slider->show();

        m_connectButton->setText("Disconnect");

        disconnect(m_connectButton,SIGNAL(clicked(bool)),this,SLOT(connectToServer()));
        connect(m_connectButton,SIGNAL(clicked(bool)),m_socket,SLOT(deleteLater()));
    }
}
