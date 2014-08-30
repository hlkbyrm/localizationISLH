#include <tf/tf.h>
#include "commclient.h"
#include <QtNetwork/QHostInfo>
#include <QtGui/QMessageBox>
#include <QtCore/QString>
#include <string>
#include <sstream>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Pose.h>
#include <qjson/parser.h>
#include <QDir>
#include <QFile>

#include "rosThread.h"

//using namespace std;


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

    //type = clientType;

    //add = socket->peerAddress();

    //QString IP = "10.42.0.102";//socket->peerAddress().toString(); // get IP



    //connect(socket, SIGNAL(disconnected()), this, SLOT(getSocketDisconnected()));


    //connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

    connect(socket,SIGNAL(readyRead()),this,SLOT(receiveData()));

    // When neighbor info is received, notify the parent
//    connect(this,SIGNAL(neighborInfo(navigationISL::robotInfo)),this->parent(),SLOT(receiveRobotInfo(navigationISL::robotInfo)));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displaySocketError(QAbstractSocket::SocketError)));


  //  clientSocketError = QAbstractSocket::UnknownSocketError; // initially no error


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

    geometry_msgs::PoseArray robotPoseArray;


    if(list.at(0) == "AA" && list.size()==(2+numOfRobots) )
    {
        geometry_msgs::Pose robotPose;
        robotPose.position.x = list.at(1).toDouble(); // this is epoch time of last received data
        robotPoseArray.poses.push_back(robotPose);

        for(int i = 2; i < list.size();i++){

            qDebug()<<list[i]<<" "<<i;

            QStringList valsList = list[i].split(",",QString::SkipEmptyParts);
            qDebug()<< valsList;

            //if (valsList.at(0).toInt()==robotID){
                //printf("x=%f y=%f q=%f", valsList.at(0).toFloat(), valsList.at(1).toFloat(), valsList.at(2).toFloat());

                geometry_msgs::Pose robotPose;
                robotPose.position.x = valsList.at(0).toFloat();
                robotPose.position.y = valsList.at(1).toFloat();
                robotPose.position.z = valsList.at(2).toFloat(); // Actually this is yawData
                robotPose.orientation = tf::createQuaternionMsgFromYaw(valsList.at(2).toFloat());
                robotPoseArray.poses.push_back(robotPose);
            //}
        }


        this->rosthread->localizationPosePublisher.publish(robotPoseArray);

    }


    // Clear the buffers
    recData.clear();
    recDataBA.clear();

    /*
    // If list contains anything, process it
    if(list.size()>0)
    {
        // If the control byte is correct
        if(list.at(0) == "AA")
        {
            // Read the task
            int task = list.at(1).toInt();

            int dataSize = list.at(2).toInt();

            int meaningfulSize = dataSize + list.at(1).size() + list.at(0).size() + list.at(2).size() + COMMA_OFFSET;

            if(myRecData.size() > meaningfulSize)
            {
                myRecData.remove(meaningfulSize,myRecData.size() - meaningfulSize);

                list.clear();

                list = myRecData.split(",");
            }


            // Clear the buffers
            myRecData.clear();
            myRecDataBA.clear();

            // The end part contains the whole data
            myRecData = list.at(list.size()-1);

            // Handle data
            this->handleTask(task,1);

            myRecData.clear();

        }
    }
*/


}



CommClient::~CommClient()
{


}

/*
void CommClient::getSocketDisconnected()
{


}

*/

// Displays socket error in a MessageBox
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
