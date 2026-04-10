;; Auto-generated. Do not edit!


(when (boundp 'hipnuc_imu::Ins_0x81_package)
  (if (not (find-package "HIPNUC_IMU"))
    (make-package "HIPNUC_IMU"))
  (shadow 'Ins_0x81_package (find-package "HIPNUC_IMU")))
(unless (find-package "HIPNUC_IMU::INS_0X81_PACKAGE")
  (make-package "HIPNUC_IMU::INS_0X81_PACKAGE"))

(in-package "ROS")
;;//! \htmlinclude Ins_0x81_package.msg.html


(defclass hipnuc_imu::Ins_0x81_package
  :super ros::object
  :slots (_frame_rate _tag _status _ins_status _gpst_wn _gpst_tow _gyr_b_x _gyr_b_y _gyr_b_z _acc_b_x _acc_b_y _acc_b_z _mag_b_x _mag_b_y _mag_b_z _air_pressure _reserved_1 _reserved_2 _temperature _utc_year _utc_mouth _utc_day _utc_hour _utc_min _utc_sec _roll _pitch _yaw _quat_w _quat_x _quat_y _quat_z _ins_lon _ins_lat _ins_msl _pdop _hdop _solq_pos _nv_pos _solq_heading _nv_heading _diff_age _undulation _reserved_3 _vel_enu_1 _vel_enu_2 _vel_enu_3 _acc_enu_1 _acc_enu_2 _acc_enu_3 _reserved_4 _reserved_5 _reserved_6 _reserved_7 ))

(defmethod hipnuc_imu::Ins_0x81_package
  (:init
   (&key
    ((:frame_rate __frame_rate) 0)
    ((:tag __tag) 0)
    ((:status __status) 0)
    ((:ins_status __ins_status) 0)
    ((:gpst_wn __gpst_wn) 0)
    ((:gpst_tow __gpst_tow) 0)
    ((:gyr_b_x __gyr_b_x) 0)
    ((:gyr_b_y __gyr_b_y) 0)
    ((:gyr_b_z __gyr_b_z) 0)
    ((:acc_b_x __acc_b_x) 0)
    ((:acc_b_y __acc_b_y) 0)
    ((:acc_b_z __acc_b_z) 0)
    ((:mag_b_x __mag_b_x) 0)
    ((:mag_b_y __mag_b_y) 0)
    ((:mag_b_z __mag_b_z) 0)
    ((:air_pressure __air_pressure) 0)
    ((:reserved_1 __reserved_1) 0)
    ((:reserved_2 __reserved_2) 0)
    ((:temperature __temperature) 0)
    ((:utc_year __utc_year) 0)
    ((:utc_mouth __utc_mouth) 0)
    ((:utc_day __utc_day) 0)
    ((:utc_hour __utc_hour) 0)
    ((:utc_min __utc_min) 0)
    ((:utc_sec __utc_sec) 0)
    ((:roll __roll) 0)
    ((:pitch __pitch) 0)
    ((:yaw __yaw) 0)
    ((:quat_w __quat_w) 0)
    ((:quat_x __quat_x) 0)
    ((:quat_y __quat_y) 0)
    ((:quat_z __quat_z) 0)
    ((:ins_lon __ins_lon) 0)
    ((:ins_lat __ins_lat) 0)
    ((:ins_msl __ins_msl) 0)
    ((:pdop __pdop) 0)
    ((:hdop __hdop) 0)
    ((:solq_pos __solq_pos) 0)
    ((:nv_pos __nv_pos) 0)
    ((:solq_heading __solq_heading) 0)
    ((:nv_heading __nv_heading) 0)
    ((:diff_age __diff_age) 0)
    ((:undulation __undulation) 0)
    ((:reserved_3 __reserved_3) 0)
    ((:vel_enu_1 __vel_enu_1) 0)
    ((:vel_enu_2 __vel_enu_2) 0)
    ((:vel_enu_3 __vel_enu_3) 0)
    ((:acc_enu_1 __acc_enu_1) 0)
    ((:acc_enu_2 __acc_enu_2) 0)
    ((:acc_enu_3 __acc_enu_3) 0)
    ((:reserved_4 __reserved_4) 0)
    ((:reserved_5 __reserved_5) 0)
    ((:reserved_6 __reserved_6) 0)
    ((:reserved_7 __reserved_7) 0)
    )
   (send-super :init)
   (setq _frame_rate (round __frame_rate))
   (setq _tag (round __tag))
   (setq _status (round __status))
   (setq _ins_status (round __ins_status))
   (setq _gpst_wn (round __gpst_wn))
   (setq _gpst_tow (round __gpst_tow))
   (setq _gyr_b_x (round __gyr_b_x))
   (setq _gyr_b_y (round __gyr_b_y))
   (setq _gyr_b_z (round __gyr_b_z))
   (setq _acc_b_x (round __acc_b_x))
   (setq _acc_b_y (round __acc_b_y))
   (setq _acc_b_z (round __acc_b_z))
   (setq _mag_b_x (round __mag_b_x))
   (setq _mag_b_y (round __mag_b_y))
   (setq _mag_b_z (round __mag_b_z))
   (setq _air_pressure (round __air_pressure))
   (setq _reserved_1 (round __reserved_1))
   (setq _reserved_2 (round __reserved_2))
   (setq _temperature (round __temperature))
   (setq _utc_year (round __utc_year))
   (setq _utc_mouth (round __utc_mouth))
   (setq _utc_day (round __utc_day))
   (setq _utc_hour (round __utc_hour))
   (setq _utc_min (round __utc_min))
   (setq _utc_sec (round __utc_sec))
   (setq _roll (round __roll))
   (setq _pitch (round __pitch))
   (setq _yaw (round __yaw))
   (setq _quat_w (round __quat_w))
   (setq _quat_x (round __quat_x))
   (setq _quat_y (round __quat_y))
   (setq _quat_z (round __quat_z))
   (setq _ins_lon (round __ins_lon))
   (setq _ins_lat (round __ins_lat))
   (setq _ins_msl (round __ins_msl))
   (setq _pdop (round __pdop))
   (setq _hdop (round __hdop))
   (setq _solq_pos (round __solq_pos))
   (setq _nv_pos (round __nv_pos))
   (setq _solq_heading (round __solq_heading))
   (setq _nv_heading (round __nv_heading))
   (setq _diff_age (round __diff_age))
   (setq _undulation (round __undulation))
   (setq _reserved_3 (round __reserved_3))
   (setq _vel_enu_1 (round __vel_enu_1))
   (setq _vel_enu_2 (round __vel_enu_2))
   (setq _vel_enu_3 (round __vel_enu_3))
   (setq _acc_enu_1 (round __acc_enu_1))
   (setq _acc_enu_2 (round __acc_enu_2))
   (setq _acc_enu_3 (round __acc_enu_3))
   (setq _reserved_4 (round __reserved_4))
   (setq _reserved_5 (round __reserved_5))
   (setq _reserved_6 (round __reserved_6))
   (setq _reserved_7 (round __reserved_7))
   self)
  (:frame_rate
   (&optional __frame_rate)
   (if __frame_rate (setq _frame_rate __frame_rate)) _frame_rate)
  (:tag
   (&optional __tag)
   (if __tag (setq _tag __tag)) _tag)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:ins_status
   (&optional __ins_status)
   (if __ins_status (setq _ins_status __ins_status)) _ins_status)
  (:gpst_wn
   (&optional __gpst_wn)
   (if __gpst_wn (setq _gpst_wn __gpst_wn)) _gpst_wn)
  (:gpst_tow
   (&optional __gpst_tow)
   (if __gpst_tow (setq _gpst_tow __gpst_tow)) _gpst_tow)
  (:gyr_b_x
   (&optional __gyr_b_x)
   (if __gyr_b_x (setq _gyr_b_x __gyr_b_x)) _gyr_b_x)
  (:gyr_b_y
   (&optional __gyr_b_y)
   (if __gyr_b_y (setq _gyr_b_y __gyr_b_y)) _gyr_b_y)
  (:gyr_b_z
   (&optional __gyr_b_z)
   (if __gyr_b_z (setq _gyr_b_z __gyr_b_z)) _gyr_b_z)
  (:acc_b_x
   (&optional __acc_b_x)
   (if __acc_b_x (setq _acc_b_x __acc_b_x)) _acc_b_x)
  (:acc_b_y
   (&optional __acc_b_y)
   (if __acc_b_y (setq _acc_b_y __acc_b_y)) _acc_b_y)
  (:acc_b_z
   (&optional __acc_b_z)
   (if __acc_b_z (setq _acc_b_z __acc_b_z)) _acc_b_z)
  (:mag_b_x
   (&optional __mag_b_x)
   (if __mag_b_x (setq _mag_b_x __mag_b_x)) _mag_b_x)
  (:mag_b_y
   (&optional __mag_b_y)
   (if __mag_b_y (setq _mag_b_y __mag_b_y)) _mag_b_y)
  (:mag_b_z
   (&optional __mag_b_z)
   (if __mag_b_z (setq _mag_b_z __mag_b_z)) _mag_b_z)
  (:air_pressure
   (&optional __air_pressure)
   (if __air_pressure (setq _air_pressure __air_pressure)) _air_pressure)
  (:reserved_1
   (&optional __reserved_1)
   (if __reserved_1 (setq _reserved_1 __reserved_1)) _reserved_1)
  (:reserved_2
   (&optional __reserved_2)
   (if __reserved_2 (setq _reserved_2 __reserved_2)) _reserved_2)
  (:temperature
   (&optional __temperature)
   (if __temperature (setq _temperature __temperature)) _temperature)
  (:utc_year
   (&optional __utc_year)
   (if __utc_year (setq _utc_year __utc_year)) _utc_year)
  (:utc_mouth
   (&optional __utc_mouth)
   (if __utc_mouth (setq _utc_mouth __utc_mouth)) _utc_mouth)
  (:utc_day
   (&optional __utc_day)
   (if __utc_day (setq _utc_day __utc_day)) _utc_day)
  (:utc_hour
   (&optional __utc_hour)
   (if __utc_hour (setq _utc_hour __utc_hour)) _utc_hour)
  (:utc_min
   (&optional __utc_min)
   (if __utc_min (setq _utc_min __utc_min)) _utc_min)
  (:utc_sec
   (&optional __utc_sec)
   (if __utc_sec (setq _utc_sec __utc_sec)) _utc_sec)
  (:roll
   (&optional __roll)
   (if __roll (setq _roll __roll)) _roll)
  (:pitch
   (&optional __pitch)
   (if __pitch (setq _pitch __pitch)) _pitch)
  (:yaw
   (&optional __yaw)
   (if __yaw (setq _yaw __yaw)) _yaw)
  (:quat_w
   (&optional __quat_w)
   (if __quat_w (setq _quat_w __quat_w)) _quat_w)
  (:quat_x
   (&optional __quat_x)
   (if __quat_x (setq _quat_x __quat_x)) _quat_x)
  (:quat_y
   (&optional __quat_y)
   (if __quat_y (setq _quat_y __quat_y)) _quat_y)
  (:quat_z
   (&optional __quat_z)
   (if __quat_z (setq _quat_z __quat_z)) _quat_z)
  (:ins_lon
   (&optional __ins_lon)
   (if __ins_lon (setq _ins_lon __ins_lon)) _ins_lon)
  (:ins_lat
   (&optional __ins_lat)
   (if __ins_lat (setq _ins_lat __ins_lat)) _ins_lat)
  (:ins_msl
   (&optional __ins_msl)
   (if __ins_msl (setq _ins_msl __ins_msl)) _ins_msl)
  (:pdop
   (&optional __pdop)
   (if __pdop (setq _pdop __pdop)) _pdop)
  (:hdop
   (&optional __hdop)
   (if __hdop (setq _hdop __hdop)) _hdop)
  (:solq_pos
   (&optional __solq_pos)
   (if __solq_pos (setq _solq_pos __solq_pos)) _solq_pos)
  (:nv_pos
   (&optional __nv_pos)
   (if __nv_pos (setq _nv_pos __nv_pos)) _nv_pos)
  (:solq_heading
   (&optional __solq_heading)
   (if __solq_heading (setq _solq_heading __solq_heading)) _solq_heading)
  (:nv_heading
   (&optional __nv_heading)
   (if __nv_heading (setq _nv_heading __nv_heading)) _nv_heading)
  (:diff_age
   (&optional __diff_age)
   (if __diff_age (setq _diff_age __diff_age)) _diff_age)
  (:undulation
   (&optional __undulation)
   (if __undulation (setq _undulation __undulation)) _undulation)
  (:reserved_3
   (&optional __reserved_3)
   (if __reserved_3 (setq _reserved_3 __reserved_3)) _reserved_3)
  (:vel_enu_1
   (&optional __vel_enu_1)
   (if __vel_enu_1 (setq _vel_enu_1 __vel_enu_1)) _vel_enu_1)
  (:vel_enu_2
   (&optional __vel_enu_2)
   (if __vel_enu_2 (setq _vel_enu_2 __vel_enu_2)) _vel_enu_2)
  (:vel_enu_3
   (&optional __vel_enu_3)
   (if __vel_enu_3 (setq _vel_enu_3 __vel_enu_3)) _vel_enu_3)
  (:acc_enu_1
   (&optional __acc_enu_1)
   (if __acc_enu_1 (setq _acc_enu_1 __acc_enu_1)) _acc_enu_1)
  (:acc_enu_2
   (&optional __acc_enu_2)
   (if __acc_enu_2 (setq _acc_enu_2 __acc_enu_2)) _acc_enu_2)
  (:acc_enu_3
   (&optional __acc_enu_3)
   (if __acc_enu_3 (setq _acc_enu_3 __acc_enu_3)) _acc_enu_3)
  (:reserved_4
   (&optional __reserved_4)
   (if __reserved_4 (setq _reserved_4 __reserved_4)) _reserved_4)
  (:reserved_5
   (&optional __reserved_5)
   (if __reserved_5 (setq _reserved_5 __reserved_5)) _reserved_5)
  (:reserved_6
   (&optional __reserved_6)
   (if __reserved_6 (setq _reserved_6 __reserved_6)) _reserved_6)
  (:reserved_7
   (&optional __reserved_7)
   (if __reserved_7 (setq _reserved_7 __reserved_7)) _reserved_7)
  (:serialization-length
   ()
   (+
    ;; uint32 _frame_rate
    4
    ;; uint8 _tag
    1
    ;; uint16 _status
    2
    ;; uint8 _ins_status
    1
    ;; uint16 _gpst_wn
    2
    ;; uint32 _gpst_tow
    4
    ;; int16 _gyr_b_x
    2
    ;; int16 _gyr_b_y
    2
    ;; int16 _gyr_b_z
    2
    ;; int16 _acc_b_x
    2
    ;; int16 _acc_b_y
    2
    ;; int16 _acc_b_z
    2
    ;; int16 _mag_b_x
    2
    ;; int16 _mag_b_y
    2
    ;; int16 _mag_b_z
    2
    ;; int16 _air_pressure
    2
    ;; uint8 _reserved_1
    1
    ;; uint8 _reserved_2
    1
    ;; int8 _temperature
    1
    ;; uint8 _utc_year
    1
    ;; uint8 _utc_mouth
    1
    ;; uint8 _utc_day
    1
    ;; uint8 _utc_hour
    1
    ;; uint8 _utc_min
    1
    ;; uint16 _utc_sec
    2
    ;; int16 _roll
    2
    ;; int16 _pitch
    2
    ;; uint16 _yaw
    2
    ;; int16 _quat_w
    2
    ;; int16 _quat_x
    2
    ;; int16 _quat_y
    2
    ;; int16 _quat_z
    2
    ;; int32 _ins_lon
    4
    ;; int32 _ins_lat
    4
    ;; int32 _ins_msl
    4
    ;; uint8 _pdop
    1
    ;; uint8 _hdop
    1
    ;; uint8 _solq_pos
    1
    ;; uint8 _nv_pos
    1
    ;; uint8 _solq_heading
    1
    ;; uint8 _nv_heading
    1
    ;; uint8 _diff_age
    1
    ;; int16 _undulation
    2
    ;; uint8 _reserved_3
    1
    ;; int16 _vel_enu_1
    2
    ;; int16 _vel_enu_2
    2
    ;; int16 _vel_enu_3
    2
    ;; int16 _acc_enu_1
    2
    ;; int16 _acc_enu_2
    2
    ;; int16 _acc_enu_3
    2
    ;; uint32 _reserved_4
    4
    ;; uint32 _reserved_5
    4
    ;; uint32 _reserved_6
    4
    ;; uint16 _reserved_7
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _frame_rate
       (write-long _frame_rate s)
     ;; uint8 _tag
       (write-byte _tag s)
     ;; uint16 _status
       (write-word _status s)
     ;; uint8 _ins_status
       (write-byte _ins_status s)
     ;; uint16 _gpst_wn
       (write-word _gpst_wn s)
     ;; uint32 _gpst_tow
       (write-long _gpst_tow s)
     ;; int16 _gyr_b_x
       (write-word _gyr_b_x s)
     ;; int16 _gyr_b_y
       (write-word _gyr_b_y s)
     ;; int16 _gyr_b_z
       (write-word _gyr_b_z s)
     ;; int16 _acc_b_x
       (write-word _acc_b_x s)
     ;; int16 _acc_b_y
       (write-word _acc_b_y s)
     ;; int16 _acc_b_z
       (write-word _acc_b_z s)
     ;; int16 _mag_b_x
       (write-word _mag_b_x s)
     ;; int16 _mag_b_y
       (write-word _mag_b_y s)
     ;; int16 _mag_b_z
       (write-word _mag_b_z s)
     ;; int16 _air_pressure
       (write-word _air_pressure s)
     ;; uint8 _reserved_1
       (write-byte _reserved_1 s)
     ;; uint8 _reserved_2
       (write-byte _reserved_2 s)
     ;; int8 _temperature
       (write-byte _temperature s)
     ;; uint8 _utc_year
       (write-byte _utc_year s)
     ;; uint8 _utc_mouth
       (write-byte _utc_mouth s)
     ;; uint8 _utc_day
       (write-byte _utc_day s)
     ;; uint8 _utc_hour
       (write-byte _utc_hour s)
     ;; uint8 _utc_min
       (write-byte _utc_min s)
     ;; uint16 _utc_sec
       (write-word _utc_sec s)
     ;; int16 _roll
       (write-word _roll s)
     ;; int16 _pitch
       (write-word _pitch s)
     ;; uint16 _yaw
       (write-word _yaw s)
     ;; int16 _quat_w
       (write-word _quat_w s)
     ;; int16 _quat_x
       (write-word _quat_x s)
     ;; int16 _quat_y
       (write-word _quat_y s)
     ;; int16 _quat_z
       (write-word _quat_z s)
     ;; int32 _ins_lon
       (write-long _ins_lon s)
     ;; int32 _ins_lat
       (write-long _ins_lat s)
     ;; int32 _ins_msl
       (write-long _ins_msl s)
     ;; uint8 _pdop
       (write-byte _pdop s)
     ;; uint8 _hdop
       (write-byte _hdop s)
     ;; uint8 _solq_pos
       (write-byte _solq_pos s)
     ;; uint8 _nv_pos
       (write-byte _nv_pos s)
     ;; uint8 _solq_heading
       (write-byte _solq_heading s)
     ;; uint8 _nv_heading
       (write-byte _nv_heading s)
     ;; uint8 _diff_age
       (write-byte _diff_age s)
     ;; int16 _undulation
       (write-word _undulation s)
     ;; uint8 _reserved_3
       (write-byte _reserved_3 s)
     ;; int16 _vel_enu_1
       (write-word _vel_enu_1 s)
     ;; int16 _vel_enu_2
       (write-word _vel_enu_2 s)
     ;; int16 _vel_enu_3
       (write-word _vel_enu_3 s)
     ;; int16 _acc_enu_1
       (write-word _acc_enu_1 s)
     ;; int16 _acc_enu_2
       (write-word _acc_enu_2 s)
     ;; int16 _acc_enu_3
       (write-word _acc_enu_3 s)
     ;; uint32 _reserved_4
       (write-long _reserved_4 s)
     ;; uint32 _reserved_5
       (write-long _reserved_5 s)
     ;; uint32 _reserved_6
       (write-long _reserved_6 s)
     ;; uint16 _reserved_7
       (write-word _reserved_7 s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _frame_rate
     (setq _frame_rate (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _tag
     (setq _tag (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint16 _status
     (setq _status (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint8 _ins_status
     (setq _ins_status (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint16 _gpst_wn
     (setq _gpst_wn (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint32 _gpst_tow
     (setq _gpst_tow (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int16 _gyr_b_x
     (setq _gyr_b_x (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _gyr_b_y
     (setq _gyr_b_y (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _gyr_b_z
     (setq _gyr_b_z (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_b_x
     (setq _acc_b_x (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_b_y
     (setq _acc_b_y (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_b_z
     (setq _acc_b_z (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _mag_b_x
     (setq _mag_b_x (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _mag_b_y
     (setq _mag_b_y (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _mag_b_z
     (setq _mag_b_z (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _air_pressure
     (setq _air_pressure (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint8 _reserved_1
     (setq _reserved_1 (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _reserved_2
     (setq _reserved_2 (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; int8 _temperature
     (setq _temperature (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _temperature 127) (setq _temperature (- _temperature 256)))
   ;; uint8 _utc_year
     (setq _utc_year (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _utc_mouth
     (setq _utc_mouth (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _utc_day
     (setq _utc_day (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _utc_hour
     (setq _utc_hour (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _utc_min
     (setq _utc_min (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint16 _utc_sec
     (setq _utc_sec (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _roll
     (setq _roll (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _pitch
     (setq _pitch (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint16 _yaw
     (setq _yaw (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _quat_w
     (setq _quat_w (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _quat_x
     (setq _quat_x (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _quat_y
     (setq _quat_y (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _quat_z
     (setq _quat_z (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int32 _ins_lon
     (setq _ins_lon (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _ins_lat
     (setq _ins_lat (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _ins_msl
     (setq _ins_msl (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _pdop
     (setq _pdop (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _hdop
     (setq _hdop (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _solq_pos
     (setq _solq_pos (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _nv_pos
     (setq _nv_pos (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _solq_heading
     (setq _solq_heading (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _nv_heading
     (setq _nv_heading (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _diff_age
     (setq _diff_age (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; int16 _undulation
     (setq _undulation (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint8 _reserved_3
     (setq _reserved_3 (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; int16 _vel_enu_1
     (setq _vel_enu_1 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _vel_enu_2
     (setq _vel_enu_2 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _vel_enu_3
     (setq _vel_enu_3 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_enu_1
     (setq _acc_enu_1 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_enu_2
     (setq _acc_enu_2 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _acc_enu_3
     (setq _acc_enu_3 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint32 _reserved_4
     (setq _reserved_4 (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _reserved_5
     (setq _reserved_5 (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _reserved_6
     (setq _reserved_6 (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint16 _reserved_7
     (setq _reserved_7 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get hipnuc_imu::Ins_0x81_package :md5sum-) "4551f87534527c807c5576a40dab6bfe")
(setf (get hipnuc_imu::Ins_0x81_package :datatype-) "hipnuc_imu/Ins_0x81_package")
(setf (get hipnuc_imu::Ins_0x81_package :definition-)
      "uint32 frame_rate
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



(provide :hipnuc_imu/Ins_0x81_package "4551f87534527c807c5576a40dab6bfe")


