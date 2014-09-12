#ifndef COMMCLIENT_H
#define COMMCLIENT_H

#include <QtNetwork/QtNetwork>
#include <QtCore/QByteArray>
#include <QObject>
#include <QXmlStreamReader>
#include <ros/timer.h>
#include "ISLH_msgs/robotPositions.h"

class RosThread;

class CommClient : public QObject
{
    Q_OBJECT
public:
    explicit CommClient(QObject *parent = 0);
   ~CommClient(void);

    // Connection request to the given address and port
    void connectToHost(QString hostAddress, quint16 port);

    RosThread* rosthread;

private:
    QTcpSocket* socket;

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
};

#endif // COMMCLIENT_H
