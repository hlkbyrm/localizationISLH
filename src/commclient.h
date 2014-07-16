#ifndef COMMCLIENT_H
#define COMMCLIENT_H

#include <QtNetwork/QtNetwork>
#include <QtCore/QByteArray>
//#include "robot.h"
#include <QObject>
//===================================================================================================================================#include <navigationISL/robotInfo.h>
//#include <navigationISL/networkInfo.h>
//#include <navigationISL/helpMessage.h>
#include <QXmlStreamReader>
#include <ros/timer.h>

class RosThread;

class CommClient : public QObject
{
    Q_OBJECT
public:
    explicit CommClient(QObject *parent = 0);

    // Constructor for incoming Client
    //CommClient(QObject* parent = 0);

   ~CommClient(void);

    // Connection request to the given address and port
    void connectToHost(QString hostAddress, quint16 port);


//    bool readConfigFile(QString filename);

//    bool initializeNetwork();

    RosThread* rosthread;

private:

//    tcpComm TcpComm;

    QTcpSocket* socket;

//    Client tempClient;
//
//    bool firstNetworkReceived;

    QString hostName;

    QAbstractSocket::SocketError clientSocketError;

    // Received data
    QString recData;

    QByteArray recDataBA;

    int robotID;
    QString IP;

    int numOfRobots;

    int readConfigFile(QString filename);

signals:
    
public slots:

    void receiveData();

private slots:

    void displaySocketError(QAbstractSocket::SocketError socketError);

 //   void getSocketDisconnected();


//    void connecttt();

//    void handleSocketConnect();

//    void handleSocketError(QAbstractSocket::SocketError error);
    
//    void handleNewCommRequest(QTcpSocket* socket);

//    void getClientDisconnected(int type);

//    void connectToRobots();

//    void handleNetworkInfo(QStringList list);


};

#endif // COMMCLIENT_H
