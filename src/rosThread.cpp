#include "rosThread.h"
//#include <navigationISL/neighborInfo.h>
//#include "commclient.h"

#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Pose.h>


RosThread::RosThread()
{
    shutdown = false;

}
RosThread::RosThread(CommClient *currentcommclient)
{
    commclient = currentcommclient;

}

void RosThread::work(){

    if(!ros::ok()){

        emit rosStartFailed();

        return;
    }

     emit rosStarted();

     localizationPosePublisher = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("amcl_pose", 1000);


     //robotInfoSubscriber = n.subscribe("navigationISL/robotInfo",5,&CommunicationManager::handleNavigationISLInfo,this->manager);

     //coordinatorUpdateSubscriber = n.subscribe("navigationISL/coordinatorUpdate",5,&CommunicationManager::handleCoordinatorUpdate,this->manager);

     // Sends the received neighbor info to the navigationNode
     //neighborInfoPublisher = n.advertise<navigationISL::neighborInfo>("communicationISL/neighborInfo",3);
  //  this->amclSub = n.subscribe("amcl_pose",2,&RosThread::amclPoseCallback,this);

     // Publishes the received coordinator update from a robot to the coordinator node
     //coordinatorUpdatePublisher = n.advertise<navigationISL::neighborInfo>("communicationISL/coordinatorUpdate",1);

     //networkUpdateSubscriber = n.subscribe("coordinatorISL/networkInfo",1,&CommunicationManager::handleNetworkUpdateFromCoordinator,this->manager);

     //hotspotHandlerMessageInPublisher = n.advertise<navigationISL::helpMessage>("communicationISL/hotspothandlerMessageIn",5);

     //hotspotHandlerMessageOutSubscriber = n.subscribe("hotspothandlerISL/outMessage",5,&CommunicationManager::handleHotspotHandlerMessageOut,this->manager);

    ros::Rate loop(30);

    while(ros::ok()){

        //coordinatorUpdatePublisher.publish(inf);

          //  NavigationController::robotContoller(vel, numOfRobots, bin, bt, b_rs, ro, kkLimits);

             //   ros::spinOnce();

             //   loop.sleep();

        ros::spinOnce();
        loop.sleep();

    }

    emit rosFinished();


}
void RosThread::shutdownROS()
{
    ros::shutdown();
   // shutdown = true;


}


