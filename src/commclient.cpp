#include "commclient.h"
#include <QtNetwork/QHostInfo>
#include <QtGui/QMessageBox>
#include <QtCore/QString>
#include <string>
#include <sstream>
#include <qjson/parser.h>
#include <QDir>
#include <QFile>

#include "rosThread.h"

CommClient::CommClient(QObject* parent) :
    QObject(parent)
{
    QString path = QDir::homePath();
    path.append("/ISL_workspace/src/configISL.json");

    robotID = 0;

    if( !readConfigFile(path) ){
        qDebug()<< "Read Config File Failed!!!";
    }
    else
    {
       qDebug() << "robotID is: "<< robotID;
    }


    socket = new QTcpSocket(this);

    socket->setReadBufferSize(0);

    connect(socket,SIGNAL(readyRead()),this,SLOT(receiveData()));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displaySocketError(QAbstractSocket::SocketError)));

    qDebug() << "Client IP is: " << IP;

    socket->connectToHost(IP,4012);

    if(socket->waitForConnected(5000)){
        qDebug()<<"Connected";
    }
    else
    {
        qDebug()<<"Error timeout";
    }
}

void CommClient::receiveData(){
    // Read the buffer;
    recDataBA = socket->readAll();

    // Convert to QString
    recData = QString::fromAscii(recDataBA);
	
	// If more than one message came than get only the last one
    QStringList _list = recData.split("AA;",QString::SkipEmptyParts);
	recData = "AA;" + _list.at(_list.count()-1);
	
    // Split the data (Comma seperated format)
    QStringList list = recData.split(";",QString::SkipEmptyParts);

    // Incoming data parts
    qDebug()<<"Number of incoming data parts"<<list.size();
    qDebug()<<list;

    ISLH_msgs::robotPositions robotPoseArray;

    if(list.at(0) == "AA" && list.size()==(1+numOfRobots) )
    {
        for(int i = 1; i < list.size();i++){

            qDebug()<<list[i]<<" "<<i;

            QStringList valsList = list[i].split(",",QString::SkipEmptyParts);
            qDebug()<< valsList;
            geometry_msgs::Pose2D robotPose;
            robotPose.x = valsList.at(0).toFloat();
            robotPose.y = valsList.at(1).toFloat();
            float direction = valsList.at(2).toFloat(); // Actually this is yawData
            robotPoseArray.positions.push_back(robotPose);
            robotPoseArray.directions.push_back(direction);
        }

        //Note: 0. item in robotPoseArray is 1. Robot, 1. item .....
        this->rosthread->localizationPosePublisher.publish(robotPoseArray);
    }


    // Clear the buffers
    recData.clear();
    recDataBA.clear();
}



CommClient::~CommClient()
{
}

// Displays socket error
void CommClient::displaySocketError(QAbstractSocket::SocketError socketError){
    qDebug()<<"Socket Error!!!";

    return;
}


int CommClient::readConfigFile(QString filename)
{
    QFile file(filename);

    if(!file.exists()) return false;

    if(!file.open(QFile::ReadOnly)) return false;

    QJson::Parser parser;

    bool ok;

    QVariantMap result = parser.parse(&file,&ok).toMap();

    if(!ok){

        file.close();
        qDebug()<<"Fatal reading error";

        return false;
    }
    else
    {

        robotID = result["robotID"].toInt();
        qDebug()<<result["robotID"].toString();

        IP = result["IP"].toString();
        qDebug()<<result["IP"].toString();


        numOfRobots = result["numrobots"].toInt();
        qDebug()<<result["numrobots"].toString();

    }
    file.close();
    return true;
}
