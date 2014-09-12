#include <QThread>
#include <QObject>
#include <ros/ros.h>
#include "ISLH_msgs/robotPositions.h"
#include <QTimer>

class CommClient;

class RosThread:public QObject
{
    Q_OBJECT

public:

    RosThread();

    RosThread(CommClient* currentcommclient);

public:

     void shutdownROS();

     friend class CommClient;

private:

     CommClient* commclient;

     bool shutdown;

     ros::NodeHandle n;

     ros::Publisher localizationPosePublisher;

public slots:
     void work();

signals:
   void rosFinished();
   void  rosStarted();
   void  rosStartFailed();

};
