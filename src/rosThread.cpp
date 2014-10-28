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

     localizationPosePublisher = n.advertise<ISLH_msgs::robotPositions>("localizationISLH/poseList", 1);

    ros::Rate loop(10);

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


