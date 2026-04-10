;; Auto-generated. Do not edit!


(when (boundp 'hipnuc_imu::hipnuc_imu_msg)
  (if (not (find-package "HIPNUC_IMU"))
    (make-package "HIPNUC_IMU"))
  (shadow 'hipnuc_imu_msg (find-package "HIPNUC_IMU")))
(unless (find-package "HIPNUC_IMU::HIPNUC_IMU_MSG")
  (make-package "HIPNUC_IMU::HIPNUC_IMU_MSG"))

(in-package "ROS")
;;//! \htmlinclude hipnuc_imu_msg.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass hipnuc_imu::hipnuc_imu_msg
  :super ros::object
  :slots (_header _hi91_data _hi92_data _hi81_data ))

(defmethod hipnuc_imu::hipnuc_imu_msg
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:hi91_data __hi91_data) (instance hipnuc_imu::Imu_0x91_package :init))
    ((:hi92_data __hi92_data) (instance hipnuc_imu::Imu_0x92_package :init))
    ((:hi81_data __hi81_data) (instance hipnuc_imu::Ins_0x81_package :init))
    )
   (send-super :init)
   (setq _header __header)
   (setq _hi91_data __hi91_data)
   (setq _hi92_data __hi92_data)
   (setq _hi81_data __hi81_data)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:hi91_data
   (&rest __hi91_data)
   (if (keywordp (car __hi91_data))
       (send* _hi91_data __hi91_data)
     (progn
       (if __hi91_data (setq _hi91_data (car __hi91_data)))
       _hi91_data)))
  (:hi92_data
   (&rest __hi92_data)
   (if (keywordp (car __hi92_data))
       (send* _hi92_data __hi92_data)
     (progn
       (if __hi92_data (setq _hi92_data (car __hi92_data)))
       _hi92_data)))
  (:hi81_data
   (&rest __hi81_data)
   (if (keywordp (car __hi81_data))
       (send* _hi81_data __hi81_data)
     (progn
       (if __hi81_data (setq _hi81_data (car __hi81_data)))
       _hi81_data)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; hipnuc_imu/Imu_0x91_package _hi91_data
    (send _hi91_data :serialization-length)
    ;; hipnuc_imu/Imu_0x92_package _hi92_data
    (send _hi92_data :serialization-length)
    ;; hipnuc_imu/Ins_0x81_package _hi81_data
    (send _hi81_data :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; hipnuc_imu/Imu_0x91_package _hi91_data
       (send _hi91_data :serialize s)
     ;; hipnuc_imu/Imu_0x92_package _hi92_data
       (send _hi92_data :serialize s)
     ;; hipnuc_imu/Ins_0x81_package _hi81_data
       (send _hi81_data :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; hipnuc_imu/Imu_0x91_package _hi91_data
     (send _hi91_data :deserialize buf ptr-) (incf ptr- (send _hi91_data :serialization-length))
   ;; hipnuc_imu/Imu_0x92_package _hi92_data
     (send _hi92_data :deserialize buf ptr-) (incf ptr- (send _hi92_data :serialization-length))
   ;; hipnuc_imu/Ins_0x81_package _hi81_data
     (send _hi81_data :deserialize buf ptr-) (incf ptr- (send _hi81_data :serialization-length))
   ;;
   self)
  )

(setf (get hipnuc_imu::hipnuc_imu_msg :md5sum-) "fb9f1cc5f720d9cd4bf3b0cab56ae7c9")
(setf (get hipnuc_imu::hipnuc_imu_msg :datatype-) "hipnuc_imu/hipnuc_imu_msg")
(setf (get hipnuc_imu::hipnuc_imu_msg :definition-)
      "Header header
Imu_0x91_package hi91_data
Imu_0x92_package hi92_data
Ins_0x81_package hi81_data

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: hipnuc_imu/Imu_0x91_package
uint32 frame_rate
uint8 tag
int8 temperature
float32 air_pressure
uint32 system_time
float32 acc_x
float32 acc_y
float32 acc_z
float32 gyr_x
float32 gyr_y
float32 gyr_z
float32 mag_x
float32 mag_y
float32 mag_z
float32 eul_r
float32 eul_p
float32 eul_y
float32 quat_w
float32 quat_x
float32 quat_y
float32 quat_z
================================================================================
MSG: hipnuc_imu/Imu_0x92_package
uint32 frame_rate
uint8 tag
int8 temperature
float32 air_pressure
uint32 system_time
float32 acc_x
float32 acc_y
float32 acc_z
float32 gyr_x
float32 gyr_y
float32 gyr_z
float32 mag_x
float32 mag_y
float32 mag_z
float32 eul_r
float32 eul_p
float32 eul_y
float32 quat_w
float32 quat_x
float32 quat_y
float32 quat_z
================================================================================
MSG: hipnuc_imu/Ins_0x81_package
uint32 frame_rate
uint8  tag
uint16 status
uint8 ins_status
uint16 gpst_wn   
uint32 gpst_tow
int16 gyr_b_x
int16 gyr_b_y
int16 gyr_b_z
int16 acc_b_x
int16 acc_b_y
int16 acc_b_z
int16 mag_b_x
int16 mag_b_y
int16 mag_b_z
int16 air_pressure
uint8 reserved_1
uint8 reserved_2
int8 temperature
uint8 utc_year
uint8 utc_mouth
uint8 utc_day
uint8 utc_hour
uint8 utc_min
uint16 utc_sec
int16 roll
int16 pitch
uint16 yaw
int16 quat_w
int16 quat_x
int16 quat_y
int16 quat_z
int32 ins_lon
int32 ins_lat
int32 ins_msl
uint8 pdop
uint8 hdop
uint8 solq_pos
uint8 nv_pos
uint8 solq_heading
uint8 nv_heading
uint8 diff_age
int16 undulation
uint8 reserved_3
int16 vel_enu_1
int16 vel_enu_2
int16 vel_enu_3
int16 acc_enu_1
int16 acc_enu_2
int16 acc_enu_3
uint32 reserved_4
uint32 reserved_5
uint32 reserved_6
uint16 reserved_7
")



(provide :hipnuc_imu/hipnuc_imu_msg "fb9f1cc5f720d9cd4bf3b0cab56ae7c9")


