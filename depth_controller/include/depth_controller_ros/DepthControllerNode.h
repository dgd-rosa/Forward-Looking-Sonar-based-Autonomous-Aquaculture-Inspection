/*
Developers: #DSORTeam -> @tecnico.ulisboa.pt Instituto Superior Tecnico 
*/
 #ifndef CATKIN_WS_DEPTHCONTROLLERNODE_H
 #define CATKIN_WS_DEPTHCONTROLLERNODE_H

 //some generically useful stuff to include...
 #include <std_msgs/String.h>
 #include <vector>
 #include <algorithm>
 #include <ros/ros.h>
  #include "DepthControllerAlgorithm.h"

 #include <farol_gimmicks_library/FarolGimmicks.h>

/* Include the messages used by publishers */
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int8.h>
#include <auv_msgs/NavigationStatus.h>
#include <auv_msgs/BodyForceRequest.h>

#include "depth_controller/ChangeDepthInnerGains.h"
#include "depth_controller/ChangeDepthFFGains.h"
#include "depth_controller/ChangeKffLamb.h"

/* -------------------------------------------------------------------------*/
/**
 * @brief  ADD HERE A SMALL DESCRIPTION OF THE NODE'S OBJECTIVE
 */
/* -------------------------------------------------------------------------*/
 class DepthControllerNode {
 public:
   
   /* -------------------------------------------------------------------------*/
   /**
    * @brief Constructor
    *
    * @Param nodehandle
    * @Param nodehandle_private
    */
   /* -------------------------------------------------------------------------*/
 	DepthControllerNode(ros::NodeHandle* nodehandle, ros::NodeHandle *nodehandle_private);

  /* -------------------------------------------------------------------------*/
  /**
   * @brief  Destructor
   */
  /* -------------------------------------------------------------------------*/
 	~DepthControllerNode();

  

  // @.@ Public methods



 private:
 	ros::NodeHandle nh_;          ///< ROS nodehandler
 	ros::NodeHandle nh_private_;  ///< ROS private nodehandler

 	// @.@ Timer
 	ros::Timer timer_;           ///< ROS Timer

  
  /**
   * 
   * @brief Surge Controller Attribute
   */
  std::unique_ptr<PID_Controller> depth_controller_;
  

  // @.@ Subsctibers
  ros::Subscriber state_sub_;
  ros::Subscriber depth_sub_;

 	// @.@ Publishers

  ros::Publisher forces_torques_pub_;

  ros::Publisher integral_error_pub_;
  
  
  // @.@ Parameters from Yaml
  double p_node_frequency_;   ///< node frequency

 	// @.@ Problem variables
  double surge_;
  double sway_;
  double heave_;
  double roll_rate_;
  double pitch_rate_;
  double yaw_rate_;
  double depth_;

  double depth_ref_;
  double last_ref_;

  ros::Time last_cmd_;
  bool debug;
 	
  ros::Duration timeout_ref_;   // inverval of time where a reference is valid
  double ref_value_;     // reference value of the variable being controlled
  double max_ref_value_; // maximum value of the reference being controlled
  double min_ref_value_; // minimum value of the reference being controlled
  ros::Time ref_time_;   // timestamp of the reference

  // @.@ Encapsulation the gory details of initializing subscribers, publishers and services
 	


  /* -------------------------------------------------------------------------*/
  /**
   * @brief Load parameters from parameter server 
   */
  /* -------------------------------------------------------------------------*/
 	void loadParams();


  /* -------------------------------------------------------------------------*/
  /**
   * @brief Initialize ROS node Subscribers
   */
  /* -------------------------------------------------------------------------*/
  void initializeSubscribers();


  /* -------------------------------------------------------------------------*/
  /**
   * @brief  Initialize ROS node Publishers
   */
  /* -------------------------------------------------------------------------*/
 	void initializePublishers();

 
  /* -------------------------------------------------------------------------*/
  /**
   * @brief  Initialize ROS node Services
   */
  /* -------------------------------------------------------------------------*/
 	void initializeServices();

  /* -------------------------------------------------------------------------*/
  /**
   * @brief Initialize ROS node Timer  
   */
  /* -------------------------------------------------------------------------*/
 	void initializeTimer();


 	
  // @.@ Callbacks declaration

  void stateCallback(const auv_msgs::NavigationStatus &msg);
  void depthCallback(const std_msgs::Float64 &msg);
  

  /* -------------------------------------------------------------------------*/
  /**
   * @brief  Timer iteration callback
   *
   * @Param event
   */
  /* -------------------------------------------------------------------------*/
  void timerIterCallback(const ros::TimerEvent& event);



  // @.@ Services declaration
  ros::ServiceServer change_gains_srv_;
  ros::ServiceServer change_ff_gains_srv_;
  ros::ServiceServer change_kff_lamb_srv_;
  bool changeGainsService(
    depth_controller::ChangeDepthInnerGains::Request &req,
    depth_controller::ChangeDepthInnerGains::Response &res);

  bool changeFFGainsService(
    depth_controller::ChangeDepthFFGains::Request &req,
    depth_controller::ChangeDepthFFGains::Response &res);

  bool changeKff_lambService(
    depth_controller::ChangeKffLamb::Request &req,
    depth_controller::ChangeKffLamb::Response &res);


  // @.@ Member helper functions

protected:
  ros::Publisher debug_pub_;
  farol_msgs::mPidDebug debug_msg_;

  /**
   * @brief  Check if the reference is new
   *
   * @return  True if valid, false otherwise
   */
  virtual bool validRef();


};
#endif //CATKIN_WS_CONTROLNODE_H
