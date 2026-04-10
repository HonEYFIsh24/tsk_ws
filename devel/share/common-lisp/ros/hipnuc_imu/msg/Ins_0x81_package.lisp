; Auto-generated. Do not edit!


(cl:in-package hipnuc_imu-msg)


;//! \htmlinclude Ins_0x81_package.msg.html

(cl:defclass <Ins_0x81_package> (roslisp-msg-protocol:ros-message)
  ((frame_rate
    :reader frame_rate
    :initarg :frame_rate
    :type cl:integer
    :initform 0)
   (tag
    :reader tag
    :initarg :tag
    :type cl:fixnum
    :initform 0)
   (status
    :reader status
    :initarg :status
    :type cl:fixnum
    :initform 0)
   (ins_status
    :reader ins_status
    :initarg :ins_status
    :type cl:fixnum
    :initform 0)
   (gpst_wn
    :reader gpst_wn
    :initarg :gpst_wn
    :type cl:fixnum
    :initform 0)
   (gpst_tow
    :reader gpst_tow
    :initarg :gpst_tow
    :type cl:integer
    :initform 0)
   (gyr_b_x
    :reader gyr_b_x
    :initarg :gyr_b_x
    :type cl:fixnum
    :initform 0)
   (gyr_b_y
    :reader gyr_b_y
    :initarg :gyr_b_y
    :type cl:fixnum
    :initform 0)
   (gyr_b_z
    :reader gyr_b_z
    :initarg :gyr_b_z
    :type cl:fixnum
    :initform 0)
   (acc_b_x
    :reader acc_b_x
    :initarg :acc_b_x
    :type cl:fixnum
    :initform 0)
   (acc_b_y
    :reader acc_b_y
    :initarg :acc_b_y
    :type cl:fixnum
    :initform 0)
   (acc_b_z
    :reader acc_b_z
    :initarg :acc_b_z
    :type cl:fixnum
    :initform 0)
   (mag_b_x
    :reader mag_b_x
    :initarg :mag_b_x
    :type cl:fixnum
    :initform 0)
   (mag_b_y
    :reader mag_b_y
    :initarg :mag_b_y
    :type cl:fixnum
    :initform 0)
   (mag_b_z
    :reader mag_b_z
    :initarg :mag_b_z
    :type cl:fixnum
    :initform 0)
   (air_pressure
    :reader air_pressure
    :initarg :air_pressure
    :type cl:fixnum
    :initform 0)
   (reserved_1
    :reader reserved_1
    :initarg :reserved_1
    :type cl:fixnum
    :initform 0)
   (reserved_2
    :reader reserved_2
    :initarg :reserved_2
    :type cl:fixnum
    :initform 0)
   (temperature
    :reader temperature
    :initarg :temperature
    :type cl:fixnum
    :initform 0)
   (utc_year
    :reader utc_year
    :initarg :utc_year
    :type cl:fixnum
    :initform 0)
   (utc_mouth
    :reader utc_mouth
    :initarg :utc_mouth
    :type cl:fixnum
    :initform 0)
   (utc_day
    :reader utc_day
    :initarg :utc_day
    :type cl:fixnum
    :initform 0)
   (utc_hour
    :reader utc_hour
    :initarg :utc_hour
    :type cl:fixnum
    :initform 0)
   (utc_min
    :reader utc_min
    :initarg :utc_min
    :type cl:fixnum
    :initform 0)
   (utc_sec
    :reader utc_sec
    :initarg :utc_sec
    :type cl:fixnum
    :initform 0)
   (roll
    :reader roll
    :initarg :roll
    :type cl:fixnum
    :initform 0)
   (pitch
    :reader pitch
    :initarg :pitch
    :type cl:fixnum
    :initform 0)
   (yaw
    :reader yaw
    :initarg :yaw
    :type cl:fixnum
    :initform 0)
   (quat_w
    :reader quat_w
    :initarg :quat_w
    :type cl:fixnum
    :initform 0)
   (quat_x
    :reader quat_x
    :initarg :quat_x
    :type cl:fixnum
    :initform 0)
   (quat_y
    :reader quat_y
    :initarg :quat_y
    :type cl:fixnum
    :initform 0)
   (quat_z
    :reader quat_z
    :initarg :quat_z
    :type cl:fixnum
    :initform 0)
   (ins_lon
    :reader ins_lon
    :initarg :ins_lon
    :type cl:integer
    :initform 0)
   (ins_lat
    :reader ins_lat
    :initarg :ins_lat
    :type cl:integer
    :initform 0)
   (ins_msl
    :reader ins_msl
    :initarg :ins_msl
    :type cl:integer
    :initform 0)
   (pdop
    :reader pdop
    :initarg :pdop
    :type cl:fixnum
    :initform 0)
   (hdop
    :reader hdop
    :initarg :hdop
    :type cl:fixnum
    :initform 0)
   (solq_pos
    :reader solq_pos
    :initarg :solq_pos
    :type cl:fixnum
    :initform 0)
   (nv_pos
    :reader nv_pos
    :initarg :nv_pos
    :type cl:fixnum
    :initform 0)
   (solq_heading
    :reader solq_heading
    :initarg :solq_heading
    :type cl:fixnum
    :initform 0)
   (nv_heading
    :reader nv_heading
    :initarg :nv_heading
    :type cl:fixnum
    :initform 0)
   (diff_age
    :reader diff_age
    :initarg :diff_age
    :type cl:fixnum
    :initform 0)
   (undulation
    :reader undulation
    :initarg :undulation
    :type cl:fixnum
    :initform 0)
   (reserved_3
    :reader reserved_3
    :initarg :reserved_3
    :type cl:fixnum
    :initform 0)
   (vel_enu_1
    :reader vel_enu_1
    :initarg :vel_enu_1
    :type cl:fixnum
    :initform 0)
   (vel_enu_2
    :reader vel_enu_2
    :initarg :vel_enu_2
    :type cl:fixnum
    :initform 0)
   (vel_enu_3
    :reader vel_enu_3
    :initarg :vel_enu_3
    :type cl:fixnum
    :initform 0)
   (acc_enu_1
    :reader acc_enu_1
    :initarg :acc_enu_1
    :type cl:fixnum
    :initform 0)
   (acc_enu_2
    :reader acc_enu_2
    :initarg :acc_enu_2
    :type cl:fixnum
    :initform 0)
   (acc_enu_3
    :reader acc_enu_3
    :initarg :acc_enu_3
    :type cl:fixnum
    :initform 0)
   (reserved_4
    :reader reserved_4
    :initarg :reserved_4
    :type cl:integer
    :initform 0)
   (reserved_5
    :reader reserved_5
    :initarg :reserved_5
    :type cl:integer
    :initform 0)
   (reserved_6
    :reader reserved_6
    :initarg :reserved_6
    :type cl:integer
    :initform 0)
   (reserved_7
    :reader reserved_7
    :initarg :reserved_7
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Ins_0x81_package (<Ins_0x81_package>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Ins_0x81_package>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Ins_0x81_package)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name hipnuc_imu-msg:<Ins_0x81_package> is deprecated: use hipnuc_imu-msg:Ins_0x81_package instead.")))

(cl:ensure-generic-function 'frame_rate-val :lambda-list '(m))
(cl:defmethod frame_rate-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:frame_rate-val is deprecated.  Use hipnuc_imu-msg:frame_rate instead.")
  (frame_rate m))

(cl:ensure-generic-function 'tag-val :lambda-list '(m))
(cl:defmethod tag-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:tag-val is deprecated.  Use hipnuc_imu-msg:tag instead.")
  (tag m))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:status-val is deprecated.  Use hipnuc_imu-msg:status instead.")
  (status m))

(cl:ensure-generic-function 'ins_status-val :lambda-list '(m))
(cl:defmethod ins_status-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:ins_status-val is deprecated.  Use hipnuc_imu-msg:ins_status instead.")
  (ins_status m))

(cl:ensure-generic-function 'gpst_wn-val :lambda-list '(m))
(cl:defmethod gpst_wn-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:gpst_wn-val is deprecated.  Use hipnuc_imu-msg:gpst_wn instead.")
  (gpst_wn m))

(cl:ensure-generic-function 'gpst_tow-val :lambda-list '(m))
(cl:defmethod gpst_tow-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:gpst_tow-val is deprecated.  Use hipnuc_imu-msg:gpst_tow instead.")
  (gpst_tow m))

(cl:ensure-generic-function 'gyr_b_x-val :lambda-list '(m))
(cl:defmethod gyr_b_x-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:gyr_b_x-val is deprecated.  Use hipnuc_imu-msg:gyr_b_x instead.")
  (gyr_b_x m))

(cl:ensure-generic-function 'gyr_b_y-val :lambda-list '(m))
(cl:defmethod gyr_b_y-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:gyr_b_y-val is deprecated.  Use hipnuc_imu-msg:gyr_b_y instead.")
  (gyr_b_y m))

(cl:ensure-generic-function 'gyr_b_z-val :lambda-list '(m))
(cl:defmethod gyr_b_z-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:gyr_b_z-val is deprecated.  Use hipnuc_imu-msg:gyr_b_z instead.")
  (gyr_b_z m))

(cl:ensure-generic-function 'acc_b_x-val :lambda-list '(m))
(cl:defmethod acc_b_x-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_b_x-val is deprecated.  Use hipnuc_imu-msg:acc_b_x instead.")
  (acc_b_x m))

(cl:ensure-generic-function 'acc_b_y-val :lambda-list '(m))
(cl:defmethod acc_b_y-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_b_y-val is deprecated.  Use hipnuc_imu-msg:acc_b_y instead.")
  (acc_b_y m))

(cl:ensure-generic-function 'acc_b_z-val :lambda-list '(m))
(cl:defmethod acc_b_z-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_b_z-val is deprecated.  Use hipnuc_imu-msg:acc_b_z instead.")
  (acc_b_z m))

(cl:ensure-generic-function 'mag_b_x-val :lambda-list '(m))
(cl:defmethod mag_b_x-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:mag_b_x-val is deprecated.  Use hipnuc_imu-msg:mag_b_x instead.")
  (mag_b_x m))

(cl:ensure-generic-function 'mag_b_y-val :lambda-list '(m))
(cl:defmethod mag_b_y-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:mag_b_y-val is deprecated.  Use hipnuc_imu-msg:mag_b_y instead.")
  (mag_b_y m))

(cl:ensure-generic-function 'mag_b_z-val :lambda-list '(m))
(cl:defmethod mag_b_z-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:mag_b_z-val is deprecated.  Use hipnuc_imu-msg:mag_b_z instead.")
  (mag_b_z m))

(cl:ensure-generic-function 'air_pressure-val :lambda-list '(m))
(cl:defmethod air_pressure-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:air_pressure-val is deprecated.  Use hipnuc_imu-msg:air_pressure instead.")
  (air_pressure m))

(cl:ensure-generic-function 'reserved_1-val :lambda-list '(m))
(cl:defmethod reserved_1-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_1-val is deprecated.  Use hipnuc_imu-msg:reserved_1 instead.")
  (reserved_1 m))

(cl:ensure-generic-function 'reserved_2-val :lambda-list '(m))
(cl:defmethod reserved_2-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_2-val is deprecated.  Use hipnuc_imu-msg:reserved_2 instead.")
  (reserved_2 m))

(cl:ensure-generic-function 'temperature-val :lambda-list '(m))
(cl:defmethod temperature-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:temperature-val is deprecated.  Use hipnuc_imu-msg:temperature instead.")
  (temperature m))

(cl:ensure-generic-function 'utc_year-val :lambda-list '(m))
(cl:defmethod utc_year-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_year-val is deprecated.  Use hipnuc_imu-msg:utc_year instead.")
  (utc_year m))

(cl:ensure-generic-function 'utc_mouth-val :lambda-list '(m))
(cl:defmethod utc_mouth-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_mouth-val is deprecated.  Use hipnuc_imu-msg:utc_mouth instead.")
  (utc_mouth m))

(cl:ensure-generic-function 'utc_day-val :lambda-list '(m))
(cl:defmethod utc_day-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_day-val is deprecated.  Use hipnuc_imu-msg:utc_day instead.")
  (utc_day m))

(cl:ensure-generic-function 'utc_hour-val :lambda-list '(m))
(cl:defmethod utc_hour-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_hour-val is deprecated.  Use hipnuc_imu-msg:utc_hour instead.")
  (utc_hour m))

(cl:ensure-generic-function 'utc_min-val :lambda-list '(m))
(cl:defmethod utc_min-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_min-val is deprecated.  Use hipnuc_imu-msg:utc_min instead.")
  (utc_min m))

(cl:ensure-generic-function 'utc_sec-val :lambda-list '(m))
(cl:defmethod utc_sec-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:utc_sec-val is deprecated.  Use hipnuc_imu-msg:utc_sec instead.")
  (utc_sec m))

(cl:ensure-generic-function 'roll-val :lambda-list '(m))
(cl:defmethod roll-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:roll-val is deprecated.  Use hipnuc_imu-msg:roll instead.")
  (roll m))

(cl:ensure-generic-function 'pitch-val :lambda-list '(m))
(cl:defmethod pitch-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:pitch-val is deprecated.  Use hipnuc_imu-msg:pitch instead.")
  (pitch m))

(cl:ensure-generic-function 'yaw-val :lambda-list '(m))
(cl:defmethod yaw-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:yaw-val is deprecated.  Use hipnuc_imu-msg:yaw instead.")
  (yaw m))

(cl:ensure-generic-function 'quat_w-val :lambda-list '(m))
(cl:defmethod quat_w-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:quat_w-val is deprecated.  Use hipnuc_imu-msg:quat_w instead.")
  (quat_w m))

(cl:ensure-generic-function 'quat_x-val :lambda-list '(m))
(cl:defmethod quat_x-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:quat_x-val is deprecated.  Use hipnuc_imu-msg:quat_x instead.")
  (quat_x m))

(cl:ensure-generic-function 'quat_y-val :lambda-list '(m))
(cl:defmethod quat_y-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:quat_y-val is deprecated.  Use hipnuc_imu-msg:quat_y instead.")
  (quat_y m))

(cl:ensure-generic-function 'quat_z-val :lambda-list '(m))
(cl:defmethod quat_z-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:quat_z-val is deprecated.  Use hipnuc_imu-msg:quat_z instead.")
  (quat_z m))

(cl:ensure-generic-function 'ins_lon-val :lambda-list '(m))
(cl:defmethod ins_lon-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:ins_lon-val is deprecated.  Use hipnuc_imu-msg:ins_lon instead.")
  (ins_lon m))

(cl:ensure-generic-function 'ins_lat-val :lambda-list '(m))
(cl:defmethod ins_lat-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:ins_lat-val is deprecated.  Use hipnuc_imu-msg:ins_lat instead.")
  (ins_lat m))

(cl:ensure-generic-function 'ins_msl-val :lambda-list '(m))
(cl:defmethod ins_msl-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:ins_msl-val is deprecated.  Use hipnuc_imu-msg:ins_msl instead.")
  (ins_msl m))

(cl:ensure-generic-function 'pdop-val :lambda-list '(m))
(cl:defmethod pdop-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:pdop-val is deprecated.  Use hipnuc_imu-msg:pdop instead.")
  (pdop m))

(cl:ensure-generic-function 'hdop-val :lambda-list '(m))
(cl:defmethod hdop-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:hdop-val is deprecated.  Use hipnuc_imu-msg:hdop instead.")
  (hdop m))

(cl:ensure-generic-function 'solq_pos-val :lambda-list '(m))
(cl:defmethod solq_pos-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:solq_pos-val is deprecated.  Use hipnuc_imu-msg:solq_pos instead.")
  (solq_pos m))

(cl:ensure-generic-function 'nv_pos-val :lambda-list '(m))
(cl:defmethod nv_pos-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:nv_pos-val is deprecated.  Use hipnuc_imu-msg:nv_pos instead.")
  (nv_pos m))

(cl:ensure-generic-function 'solq_heading-val :lambda-list '(m))
(cl:defmethod solq_heading-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:solq_heading-val is deprecated.  Use hipnuc_imu-msg:solq_heading instead.")
  (solq_heading m))

(cl:ensure-generic-function 'nv_heading-val :lambda-list '(m))
(cl:defmethod nv_heading-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:nv_heading-val is deprecated.  Use hipnuc_imu-msg:nv_heading instead.")
  (nv_heading m))

(cl:ensure-generic-function 'diff_age-val :lambda-list '(m))
(cl:defmethod diff_age-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:diff_age-val is deprecated.  Use hipnuc_imu-msg:diff_age instead.")
  (diff_age m))

(cl:ensure-generic-function 'undulation-val :lambda-list '(m))
(cl:defmethod undulation-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:undulation-val is deprecated.  Use hipnuc_imu-msg:undulation instead.")
  (undulation m))

(cl:ensure-generic-function 'reserved_3-val :lambda-list '(m))
(cl:defmethod reserved_3-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_3-val is deprecated.  Use hipnuc_imu-msg:reserved_3 instead.")
  (reserved_3 m))

(cl:ensure-generic-function 'vel_enu_1-val :lambda-list '(m))
(cl:defmethod vel_enu_1-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:vel_enu_1-val is deprecated.  Use hipnuc_imu-msg:vel_enu_1 instead.")
  (vel_enu_1 m))

(cl:ensure-generic-function 'vel_enu_2-val :lambda-list '(m))
(cl:defmethod vel_enu_2-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:vel_enu_2-val is deprecated.  Use hipnuc_imu-msg:vel_enu_2 instead.")
  (vel_enu_2 m))

(cl:ensure-generic-function 'vel_enu_3-val :lambda-list '(m))
(cl:defmethod vel_enu_3-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:vel_enu_3-val is deprecated.  Use hipnuc_imu-msg:vel_enu_3 instead.")
  (vel_enu_3 m))

(cl:ensure-generic-function 'acc_enu_1-val :lambda-list '(m))
(cl:defmethod acc_enu_1-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_enu_1-val is deprecated.  Use hipnuc_imu-msg:acc_enu_1 instead.")
  (acc_enu_1 m))

(cl:ensure-generic-function 'acc_enu_2-val :lambda-list '(m))
(cl:defmethod acc_enu_2-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_enu_2-val is deprecated.  Use hipnuc_imu-msg:acc_enu_2 instead.")
  (acc_enu_2 m))

(cl:ensure-generic-function 'acc_enu_3-val :lambda-list '(m))
(cl:defmethod acc_enu_3-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:acc_enu_3-val is deprecated.  Use hipnuc_imu-msg:acc_enu_3 instead.")
  (acc_enu_3 m))

(cl:ensure-generic-function 'reserved_4-val :lambda-list '(m))
(cl:defmethod reserved_4-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_4-val is deprecated.  Use hipnuc_imu-msg:reserved_4 instead.")
  (reserved_4 m))

(cl:ensure-generic-function 'reserved_5-val :lambda-list '(m))
(cl:defmethod reserved_5-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_5-val is deprecated.  Use hipnuc_imu-msg:reserved_5 instead.")
  (reserved_5 m))

(cl:ensure-generic-function 'reserved_6-val :lambda-list '(m))
(cl:defmethod reserved_6-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_6-val is deprecated.  Use hipnuc_imu-msg:reserved_6 instead.")
  (reserved_6 m))

(cl:ensure-generic-function 'reserved_7-val :lambda-list '(m))
(cl:defmethod reserved_7-val ((m <Ins_0x81_package>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader hipnuc_imu-msg:reserved_7-val is deprecated.  Use hipnuc_imu-msg:reserved_7 instead.")
  (reserved_7 m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Ins_0x81_package>) ostream)
  "Serializes a message object of type '<Ins_0x81_package>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_rate)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_rate)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'frame_rate)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'frame_rate)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'tag)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'status)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ins_status)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gpst_wn)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'gpst_wn)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gpst_tow)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'gpst_tow)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'gpst_tow)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'gpst_tow)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'gyr_b_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'gyr_b_y)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'gyr_b_z)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_b_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_b_y)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_b_z)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'mag_b_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'mag_b_y)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'mag_b_z)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'air_pressure)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_1)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_2)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'temperature)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_year)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_mouth)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_day)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_hour)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_min)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_sec)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'utc_sec)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'roll)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'pitch)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'yaw)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'yaw)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'quat_w)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'quat_x)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'quat_y)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'quat_z)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ins_lon)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ins_lat)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'ins_msl)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pdop)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hdop)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'solq_pos)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'nv_pos)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'solq_heading)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'nv_heading)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'diff_age)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'undulation)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_3)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'vel_enu_1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'vel_enu_2)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'vel_enu_3)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_enu_1)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_enu_2)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'acc_enu_3)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_4)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_4)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_4)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_4)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_5)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_5)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_5)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_5)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_6)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_6)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_6)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_6)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_7)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_7)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Ins_0x81_package>) istream)
  "Deserializes a message object of type '<Ins_0x81_package>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_rate)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_rate)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'frame_rate)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'frame_rate)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'tag)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'status)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'status)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'ins_status)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gpst_wn)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'gpst_wn)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'gpst_tow)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'gpst_tow)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'gpst_tow)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'gpst_tow)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'gyr_b_x) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'gyr_b_y) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'gyr_b_z) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_b_x) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_b_y) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_b_z) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mag_b_x) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mag_b_y) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mag_b_z) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'air_pressure) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_1)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_2)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'temperature) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_year)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_mouth)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_day)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_hour)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_min)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'utc_sec)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'utc_sec)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'roll) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'pitch) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'yaw)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'yaw)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'quat_w) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'quat_x) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'quat_y) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'quat_z) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ins_lon) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ins_lat) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ins_msl) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pdop)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'hdop)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'solq_pos)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'nv_pos)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'solq_heading)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'nv_heading)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'diff_age)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'undulation) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_3)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'vel_enu_1) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'vel_enu_2) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'vel_enu_3) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_enu_1) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_enu_2) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'acc_enu_3) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_4)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_4)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_4)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_4)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_5)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_5)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_5)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_5)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_6)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_6)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'reserved_6)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'reserved_6)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'reserved_7)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'reserved_7)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Ins_0x81_package>)))
  "Returns string type for a message object of type '<Ins_0x81_package>"
  "hipnuc_imu/Ins_0x81_package")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Ins_0x81_package)))
  "Returns string type for a message object of type 'Ins_0x81_package"
  "hipnuc_imu/Ins_0x81_package")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Ins_0x81_package>)))
  "Returns md5sum for a message object of type '<Ins_0x81_package>"
  "4551f87534527c807c5576a40dab6bfe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Ins_0x81_package)))
  "Returns md5sum for a message object of type 'Ins_0x81_package"
  "4551f87534527c807c5576a40dab6bfe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Ins_0x81_package>)))
  "Returns full string definition for message of type '<Ins_0x81_package>"
  (cl:format cl:nil "uint32 frame_rate~%uint8  tag~%uint16 status~%uint8 ins_status~%uint16 gpst_wn   ~%uint32 gpst_tow~%int16 gyr_b_x~%int16 gyr_b_y~%int16 gyr_b_z~%int16 acc_b_x~%int16 acc_b_y~%int16 acc_b_z~%int16 mag_b_x~%int16 mag_b_y~%int16 mag_b_z~%int16 air_pressure~%uint8 reserved_1~%uint8 reserved_2~%int8 temperature~%uint8 utc_year~%uint8 utc_mouth~%uint8 utc_day~%uint8 utc_hour~%uint8 utc_min~%uint16 utc_sec~%int16 roll~%int16 pitch~%uint16 yaw~%int16 quat_w~%int16 quat_x~%int16 quat_y~%int16 quat_z~%int32 ins_lon~%int32 ins_lat~%int32 ins_msl~%uint8 pdop~%uint8 hdop~%uint8 solq_pos~%uint8 nv_pos~%uint8 solq_heading~%uint8 nv_heading~%uint8 diff_age~%int16 undulation~%uint8 reserved_3~%int16 vel_enu_1~%int16 vel_enu_2~%int16 vel_enu_3~%int16 acc_enu_1~%int16 acc_enu_2~%int16 acc_enu_3~%uint32 reserved_4~%uint32 reserved_5~%uint32 reserved_6~%uint16 reserved_7~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Ins_0x81_package)))
  "Returns full string definition for message of type 'Ins_0x81_package"
  (cl:format cl:nil "uint32 frame_rate~%uint8  tag~%uint16 status~%uint8 ins_status~%uint16 gpst_wn   ~%uint32 gpst_tow~%int16 gyr_b_x~%int16 gyr_b_y~%int16 gyr_b_z~%int16 acc_b_x~%int16 acc_b_y~%int16 acc_b_z~%int16 mag_b_x~%int16 mag_b_y~%int16 mag_b_z~%int16 air_pressure~%uint8 reserved_1~%uint8 reserved_2~%int8 temperature~%uint8 utc_year~%uint8 utc_mouth~%uint8 utc_day~%uint8 utc_hour~%uint8 utc_min~%uint16 utc_sec~%int16 roll~%int16 pitch~%uint16 yaw~%int16 quat_w~%int16 quat_x~%int16 quat_y~%int16 quat_z~%int32 ins_lon~%int32 ins_lat~%int32 ins_msl~%uint8 pdop~%uint8 hdop~%uint8 solq_pos~%uint8 nv_pos~%uint8 solq_heading~%uint8 nv_heading~%uint8 diff_age~%int16 undulation~%uint8 reserved_3~%int16 vel_enu_1~%int16 vel_enu_2~%int16 vel_enu_3~%int16 acc_enu_1~%int16 acc_enu_2~%int16 acc_enu_3~%uint32 reserved_4~%uint32 reserved_5~%uint32 reserved_6~%uint16 reserved_7~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Ins_0x81_package>))
  (cl:+ 0
     4
     1
     2
     1
     2
     4
     2
     2
     2
     2
     2
     2
     2
     2
     2
     2
     1
     1
     1
     1
     1
     1
     1
     1
     2
     2
     2
     2
     2
     2
     2
     2
     4
     4
     4
     1
     1
     1
     1
     1
     1
     1
     2
     1
     2
     2
     2
     2
     2
     2
     4
     4
     4
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Ins_0x81_package>))
  "Converts a ROS message object to a list"
  (cl:list 'Ins_0x81_package
    (cl:cons ':frame_rate (frame_rate msg))
    (cl:cons ':tag (tag msg))
    (cl:cons ':status (status msg))
    (cl:cons ':ins_status (ins_status msg))
    (cl:cons ':gpst_wn (gpst_wn msg))
    (cl:cons ':gpst_tow (gpst_tow msg))
    (cl:cons ':gyr_b_x (gyr_b_x msg))
    (cl:cons ':gyr_b_y (gyr_b_y msg))
    (cl:cons ':gyr_b_z (gyr_b_z msg))
    (cl:cons ':acc_b_x (acc_b_x msg))
    (cl:cons ':acc_b_y (acc_b_y msg))
    (cl:cons ':acc_b_z (acc_b_z msg))
    (cl:cons ':mag_b_x (mag_b_x msg))
    (cl:cons ':mag_b_y (mag_b_y msg))
    (cl:cons ':mag_b_z (mag_b_z msg))
    (cl:cons ':air_pressure (air_pressure msg))
    (cl:cons ':reserved_1 (reserved_1 msg))
    (cl:cons ':reserved_2 (reserved_2 msg))
    (cl:cons ':temperature (temperature msg))
    (cl:cons ':utc_year (utc_year msg))
    (cl:cons ':utc_mouth (utc_mouth msg))
    (cl:cons ':utc_day (utc_day msg))
    (cl:cons ':utc_hour (utc_hour msg))
    (cl:cons ':utc_min (utc_min msg))
    (cl:cons ':utc_sec (utc_sec msg))
    (cl:cons ':roll (roll msg))
    (cl:cons ':pitch (pitch msg))
    (cl:cons ':yaw (yaw msg))
    (cl:cons ':quat_w (quat_w msg))
    (cl:cons ':quat_x (quat_x msg))
    (cl:cons ':quat_y (quat_y msg))
    (cl:cons ':quat_z (quat_z msg))
    (cl:cons ':ins_lon (ins_lon msg))
    (cl:cons ':ins_lat (ins_lat msg))
    (cl:cons ':ins_msl (ins_msl msg))
    (cl:cons ':pdop (pdop msg))
    (cl:cons ':hdop (hdop msg))
    (cl:cons ':solq_pos (solq_pos msg))
    (cl:cons ':nv_pos (nv_pos msg))
    (cl:cons ':solq_heading (solq_heading msg))
    (cl:cons ':nv_heading (nv_heading msg))
    (cl:cons ':diff_age (diff_age msg))
    (cl:cons ':undulation (undulation msg))
    (cl:cons ':reserved_3 (reserved_3 msg))
    (cl:cons ':vel_enu_1 (vel_enu_1 msg))
    (cl:cons ':vel_enu_2 (vel_enu_2 msg))
    (cl:cons ':vel_enu_3 (vel_enu_3 msg))
    (cl:cons ':acc_enu_1 (acc_enu_1 msg))
    (cl:cons ':acc_enu_2 (acc_enu_2 msg))
    (cl:cons ':acc_enu_3 (acc_enu_3 msg))
    (cl:cons ':reserved_4 (reserved_4 msg))
    (cl:cons ':reserved_5 (reserved_5 msg))
    (cl:cons ':reserved_6 (reserved_6 msg))
    (cl:cons ':reserved_7 (reserved_7 msg))
))
