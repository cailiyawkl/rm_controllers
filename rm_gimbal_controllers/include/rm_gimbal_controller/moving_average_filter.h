//
// Created by chenzheng on 2021/5/8.
//

#ifndef RM_GIMBAL_CONTROLLERS_MOVING_AVERAGE_FILTER_H
#define RM_GIMBAL_CONTROLLERS_MOVING_AVERAGE_FILTER_H

#include <realtime_tools/realtime_publisher.h>
#include <realtime_tools/realtime_buffer.h>
#include <geometry_msgs/TransformStamped.h>
#include <rm_common/ros_utilities.h>
#include <rm_common/filters/filters.h>
#include <rm_common/hardware_interface/robot_state_interface.h>
#include <rm_msgs/MovingAverageData.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>

namespace moving_average_filter {

class MovingAverageFilterTrack {
 public:
  explicit MovingAverageFilterTrack(ros::NodeHandle &nh,
                                    int id,
                                    hardware_interface::RobotStateHandle robot_state_handle);
  void input(const geometry_msgs::TransformStamped &map2detection, const std::string pitch_frame);
  geometry_msgs::TransformStamped getTransform() const { return output_map2detection_; }
  geometry_msgs::Vector3 getVel() const { return output_vel_; }
  geometry_msgs::Point getPos() const { return output_pos_; }
  geometry_msgs::Point getPosObservation() const { return output_pos_observation_; }
  geometry_msgs::Point getCenter() const { return output_center_; }
  geometry_msgs::Point getCenterObservation() const { return output_center_observation_; }
  double getGyroVel() const { return output_gyro_vel_; }
  bool isGyro() const { return is_gyro_; }
  double getDelta() const { return delta_; }
  ~MovingAverageFilterTrack() = default;
 private:
  ros::Time enter_gyro_time_;

  MovingAverageFilter<double> *ma_filter_pos_x_;
  MovingAverageFilter<double> *ma_filter_pos_y_;
  MovingAverageFilter<double> *ma_filter_pos_z_;
  MovingAverageFilter<double> *ma_filter_vel_x_;
  MovingAverageFilter<double> *ma_filter_vel_y_;
  MovingAverageFilter<double> *ma_filter_vel_z_;
  MovingAverageFilter<double> *ma_filter_center_x_;
  MovingAverageFilter<double> *ma_filter_center_y_;
  MovingAverageFilter<double> *ma_filter_center_z_;
  MovingAverageFilter<double> *ma_filter_gyro_vel_;

  std::shared_ptr<realtime_tools::RealtimePublisher<rm_msgs::MovingAverageData>> realtime_pub_;
  hardware_interface::RobotStateHandle robot_state_handle_;

  bool is_debug_{};
  bool is_gyro_{};
  int switch_count_{};
  int pos_data_num_{}, vel_data_num_{}, center_data_num_{}, gyro_data_num_{};
  double delta_{};
  double output_gyro_vel_{};
  double center_offset_z_{};

  geometry_msgs::TransformStamped last_map2detection_{};
  geometry_msgs::TransformStamped output_map2detection_{};
  geometry_msgs::TransformStamped last_observation2detection_;
  geometry_msgs::Vector3 output_vel_{};
  geometry_msgs::Point output_pos_{};
  geometry_msgs::Point output_pos_observation_{};
  geometry_msgs::Point output_center_{};
  geometry_msgs::Point output_center_observation_{};
  geometry_msgs::Point last_output_pos_{};
};
}

#endif