#include "rosThread.h"
//#include <navigationISL/neighborInfo.h>
//#include "commclient.h"

#include <geometry_msgs/PoseArray.h>
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

     localizationPosePublisher = n.advertise<geometry_msgs::PoseArray>("localizationISLH/pose_list", 1000);

    ros::Rate loop(30);

    while(ros::ok()){


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


