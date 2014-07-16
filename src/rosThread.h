//#include "navigationController.h"
#include <QThread>
#include <QObject>
#include <ros/ros.h>
//===================================================================================================================================#include "navigationISL/robotInfo.h"
//#include <navigationISL/networkInfo.h>
//#include <navigationISL/helpMessage.h>
//#include "commclient.h"
#include <QTimer>

//#define numOfRobots 5
class CommClient;

class RosThread:public QObject
{
    Q_OBJECT

public:

    RosThread();

    RosThread(CommClient* currentcommclient);

   // RosThread(int argc, char **argv, std::string nodeName);

public:

     void shutdownROS();

     friend class CommClient;

     //friend class Robot;

private:

     CommClient* commclient;

     bool shutdown;

     ros::NodeHandle n;

     ros::Publisher localizationPosePublisher;
     //ros::Subscriber robotInfoSubscriber;

     //ros::Publisher neighborInfoPublisher;

     // Publish the received coordinator Update to the coordinator
     //ros::Publisher coordinatorUpdatePublisher;

     //ros::Publisher hotspotHandlerMessageInPublisher;

     // Subscribe for coordinatorUpdate from Navigation
     //ros::Subscriber coordinatorUpdateSubscriber;

     // Subscribe for a network update from coordinator
     //ros::Subscriber networkUpdateSubscriber;

     //ros::Subscriber hotspotHandlerMessageOutSubscriber;




public slots:
     void work();

signals:
   void rosFinished();
   void  rosStarted();
   void  rosStartFailed();

};
