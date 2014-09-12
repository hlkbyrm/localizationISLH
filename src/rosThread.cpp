#include "rosThread.h"

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

     localizationPosePublisher = n.advertise<ISLH_msgs::robotPositions>("localizationISLH/poseList", 1000);

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
}


