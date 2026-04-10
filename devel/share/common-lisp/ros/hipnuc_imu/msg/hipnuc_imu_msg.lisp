; Auto-generated. Do not edit!


(cl:in-package hipnuc_imu-msg)


;//! \htmlinclude hipnuc_imu_msg.msg.html

(cl:defclass <hipnuc_imu_msg> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (hi91_data
    :reader hi91_data
    :initarg :hi91_data
    :type hipnuc_imu-msg:Imu_0x91_package
    :initform (cl:make-instance 'hipnuc_imu-msg:Imu_0x91_package))
   (hi92_data
    :reader hi92_data
    :initarg :hi92_data
    :type hipnuc_imu-msg:Imu_0x92_package
    :initform (cl:make-instance 'hipnuc_imu-msg:Imu_0x92_package))
   (hi81_data
    :reader hi81_data
    :initarg :hi81_data
    :type hipnuc_imu-msg:Ins_0x81_package
    :initform (cl:make-instance 'hipnuc_imu-msg:Ins_0x81_package)))
)

(cl:defclass hipnuc_imu_msg (<hipnuc_imu_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <hipnuc_imu_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'hipnuc_imu_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hipnuc_imu-msg:<hipnuc_imu_msg> is deprecated: use hipnuc_imu-msg:hipnuc_imu_msg instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <hipnuc_imu_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:header-val is deprecated.  Use hipnuc_imu-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'hi91_data-val :lambda-list '(m))
(cl:defmethod hi91_data-val ((m <hipnuc_imu_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:hi91_data-val is deprecated.  Use hipnuc_imu-msg:hi91_data instead.")
  (hi91_data m))

(cl:ensure-generic-function 'hi92_data-val :lambda-list '(m))
(cl:defmethod hi92_data-val ((m <hipnuc_imu_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:hi92_data-val is deprecated.  Use hipnuc_imu-msg:hi92_data instead.")
  (hi92_data m))

(cl:ensure-generic-function 'hi81_data-val :lambda-list '(m))
(cl:defmethod hi81_data-val ((m <hipnuc_imu_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:hi81_data-val is deprecated.  Use hipnuc_imu-msg:hi81_data instead.")
  (hi81_data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <hipnuc_imu_msg>) ostream)
  "Serializes a message object of type '<hipnuc_imu_msg>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'hi91_data) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'hi92_data) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'hi81_data) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <hipnuc_imu_msg>) istream)
  "Deserializes a message object of type '<hipnuc_imu_msg>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'hi91_data) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'hi92_data) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'hi81_data) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<hipnuc_imu_msg>)))
  "Returns string type for a message object of type '<hipnuc_imu_msg>"
  "hipnuc_imu/hipnuc_imu_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'hipnuc_imu_msg)))
  "Returns string type for a message object of type 'hipnuc_imu_msg"
  "hipnuc_imu/hipnuc_imu_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<hipnuc_imu_msg>)))
  "Returns md5sum for a message object of type '<hipnuc_imu_msg>"
  "fb9f1cc5f720d9cd4bf3b0cab56ae7c9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'hipnuc_imu_msg)))
  "Returns md5sum for a message object of type 'hipnuc_imu_msg"
  "fb9f1cc5f720d9cd4bf3b0cab56ae7c9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<hipnuc_imu_msg>)))
  "Returns full string definition for message of type '<hipnuc_imu_msg>"
  (cl:format cl:nil "Header header~%Imu_0x91_package hi91_data~%Imu_0x92_package hi92_data~%Ins_0x81_package hi81_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: hipnuc_imu/Imu_0x91_package~%uint32 frame_rate~%uint8 tag~%int8 temperature~%float32 air_pressure~%uint32 system_time~%float32 acc_x~%float32 acc_y~%float32 acc_z~%float32 gyr_x~%float32 gyr_y~%float32 gyr_z~%float32 mag_x~%float32 mag_y~%float32 mag_z~%float32 eul_r~%float32 eul_p~%float32 eul_y~%float32 quat_w~%float32 quat_x~%float32 quat_y~%float32 quat_z~%================================================================================~%MSG: hipnuc_imu/Imu_0x92_package~%uint32 frame_rate~%uint8 tag~%int8 temperature~%float32 air_pressure~%uint32 system_time~%float32 acc_x~%float32 acc_y~%float32 acc_z~%float32 gyr_x~%float32 gyr_y~%float32 gyr_z~%float32 mag_x~%float32 mag_y~%float32 mag_z~%float32 eul_r~%float32 eul_p~%float32 eul_y~%float32 quat_w~%float32 quat_x~%float32 quat_y~%float32 quat_z~%================================================================================~%MSG: hipnuc_imu/Ins_0x81_package~%uint32 frame_rate~%uint8  tag~%uint16 status~%uint8 ins_status~%uint16 gpst_wn   ~%uint32 gpst_tow~%int16 gyr_b_x~%int16 gyr_b_y~%int16 gyr_b_z~%int16 acc_b_x~%int16 acc_b_y~%int16 acc_b_z~%int16 mag_b_x~%int16 mag_b_y~%int16 mag_b_z~%int16 air_pressure~%uint8 reserved_1~%uint8 reserved_2~%int8 temperature~%uint8 utc_year~%uint8 utc_mouth~%uint8 utc_day~%uint8 utc_hour~%uint8 utc_min~%uint16 utc_sec~%int16 roll~%int16 pitch~%uint16 yaw~%int16 quat_w~%int16 quat_x~%int16 quat_y~%int16 quat_z~%int32 ins_lon~%int32 ins_lat~%int32 ins_msl~%uint8 pdop~%uint8 hdop~%uint8 solq_pos~%uint8 nv_pos~%uint8 solq_heading~%uint8 nv_heading~%uint8 diff_age~%int16 undulation~%uint8 reserved_3~%int16 vel_enu_1~%int16 vel_enu_2~%int16 vel_enu_3~%int16 acc_enu_1~%int16 acc_enu_2~%int16 acc_enu_3~%uint32 reserved_4~%uint32 reserved_5~%uint32 reserved_6~%uint16 reserved_7~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'hipnuc_imu_msg)))
  "Returns full string definition for message of type 'hipnuc_imu_msg"
  (cl:format cl:nil "Header header~%Imu_0x91_package hi91_data~%Imu_0x92_package hi92_data~%Ins_0x81_package hi81_data~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: hipnuc_imu/Imu_0x91_package~%uint32 frame_rate~%uint8 tag~%int8 temperature~%float32 air_pressure~%uint32 system_time~%float32 acc_x~%float32 acc_y~%float32 acc_z~%float32 gyr_x~%float32 gyr_y~%float32 gyr_z~%float32 mag_x~%float32 mag_y~%float32 mag_z~%float32 eul_r~%float32 eul_p~%float32 eul_y~%float32 quat_w~%float32 quat_x~%float32 quat_y~%float32 quat_z~%================================================================================~%MSG: hipnuc_imu/Imu_0x92_package~%uint32 frame_rate~%uint8 tag~%int8 temperature~%float32 air_pressure~%uint32 system_time~%float32 acc_x~%float32 acc_y~%float32 acc_z~%float32 gyr_x~%float32 gyr_y~%float32 gyr_z~%float32 mag_x~%float32 mag_y~%float32 mag_z~%float32 eul_r~%float32 eul_p~%float32 eul_y~%float32 quat_w~%float32 quat_x~%float32 quat_y~%float32 quat_z~%================================================================================~%MSG: hipnuc_imu/Ins_0x81_package~%uint32 frame_rate~%uint8  tag~%uint16 status~%uint8 ins_status~%uint16 gpst_wn   ~%uint32 gpst_tow~%int16 gyr_b_x~%int16 gyr_b_y~%int16 gyr_b_z~%int16 acc_b_x~%int16 acc_b_y~%int16 acc_b_z~%int16 mag_b_x~%int16 mag_b_y~%int16 mag_b_z~%int16 air_pressure~%uint8 reserved_1~%uint8 reserved_2~%int8 temperature~%uint8 utc_year~%uint8 utc_mouth~%uint8 utc_day~%uint8 utc_hour~%uint8 utc_min~%uint16 utc_sec~%int16 roll~%int16 pitch~%uint16 yaw~%int16 quat_w~%int16 quat_x~%int16 quat_y~%int16 quat_z~%int32 ins_lon~%int32 ins_lat~%int32 ins_msl~%uint8 pdop~%uint8 hdop~%uint8 solq_pos~%uint8 nv_pos~%uint8 solq_heading~%uint8 nv_heading~%uint8 diff_age~%int16 undulation~%uint8 reserved_3~%int16 vel_enu_1~%int16 vel_enu_2~%int16 vel_enu_3~%int16 acc_enu_1~%int16 acc_enu_2~%int16 acc_enu_3~%uint32 reserved_4~%uint32 reserved_5~%uint32 reserved_6~%uint16 reserved_7~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <hipnuc_imu_msg>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'hi91_data))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'hi92_data))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'hi81_data))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <hipnuc_imu_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'hipnuc_imu_msg
    (cl:cons ':header (header msg))
    (cl:cons ':hi91_data (hi91_data msg))
    (cl:cons ':hi92_data (hi92_data msg))
    (cl:cons ':hi81_data (hi81_data msg))
))
