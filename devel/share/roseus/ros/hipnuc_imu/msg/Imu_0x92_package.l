;; Auto-generated. Do not edit!


(when (boundp 'hipnuc_imu::Imu_0x92_package)
  (if (not (find-package "HIPNUC_IMU"))
    (make-package "HIPNUC_IMU"))
  (shadow 'Imu_0x92_package (find-package "HIPNUC_IMU")))
(unless (find-package "HIPNUC_IMU::IMU_0X92_PACKAGE")
  (make-package "HIPNUC_IMU::IMU_0X92_PACKAGE"))

(in-package "ROS")
;;//! \htmlinclude Imu_0x92_package.msg.html


(defclass hipnuc_imu::Imu_0x92_package
  :super ros::object
  :slots (_frame_rate _tag _temperature _air_pressure _system_time _acc_x _acc_y _acc_z _gyr_x _gyr_y _gyr_z _mag_x _mag_y _mag_z _eul_r _eul_p _eul_y _quat_w _quat_x _quat_y _quat_z ))

(defmethod hipnuc_imu::Imu_0x92_package
  (:init
   (&key
    ((:frame_rate __frame_rate) 0)
    ((:tag __tag) 0)
    ((:temperature __temperature) 0)
    ((:air_pressure __air_pressure) 0.0)
    ((:system_time __system_time) 0)
    ((:acc_x __acc_x) 0.0)
    ((:acc_y __acc_y) 0.0)
    ((:acc_z __acc_z) 0.0)
    ((:gyr_x __gyr_x) 0.0)
    ((:gyr_y __gyr_y) 0.0)
    ((:gyr_z __gyr_z) 0.0)
    ((:mag_x __mag_x) 0.0)
    ((:mag_y __mag_y) 0.0)
    ((:mag_z __mag_z) 0.0)
    ((:eul_r __eul_r) 0.0)
    ((:eul_p __eul_p) 0.0)
    ((:eul_y __eul_y) 0.0)
    ((:quat_w __quat_w) 0.0)
    ((:quat_x __quat_x) 0.0)
    ((:quat_y __quat_y) 0.0)
    ((:quat_z __quat_z) 0.0)
    )
   (send-super :init)
   (setq _frame_rate (round __frame_rate))
   (setq _tag (round __tag))
   (setq _temperature (round __temperature))
   (setq _air_pressure (float __air_pressure))
   (setq _system_time (round __system_time))
   (setq _acc_x (float __acc_x))
   (setq _acc_y (float __acc_y))
   (setq _acc_z (float __acc_z))
   (setq _gyr_x (float __gyr_x))
   (setq _gyr_y (float __gyr_y))
   (setq _gyr_z (float __gyr_z))
   (setq _mag_x (float __mag_x))
   (setq _mag_y (float __mag_y))
   (setq _mag_z (float __mag_z))
   (setq _eul_r (float __eul_r))
   (setq _eul_p (float __eul_p))
   (setq _eul_y (float __eul_y))
   (setq _quat_w (float __quat_w))
   (setq _quat_x (float __quat_x))
   (setq _quat_y (float __quat_y))
   (setq _quat_z (float __quat_z))
   self)
  (:frame_rate
   (&optional __frame_rate)
   (if __frame_rate (setq _frame_rate __frame_rate)) _frame_rate)
  (:tag
   (&optional __tag)
   (if __tag (setq _tag __tag)) _tag)
  (:temperature
   (&optional __temperature)
   (if __temperature (setq _temperature __temperature)) _temperature)
  (:air_pressure
   (&optional __air_pressure)
   (if __air_pressure (setq _air_pressure __air_pressure)) _air_pressure)
  (:system_time
   (&optional __system_time)
   (if __system_time (setq _system_time __system_time)) _system_time)
  (:acc_x
   (&optional __acc_x)
   (if __acc_x (setq _acc_x __acc_x)) _acc_x)
  (:acc_y
   (&optional __acc_y)
   (if __acc_y (setq _acc_y __acc_y)) _acc_y)
  (:acc_z
   (&optional __acc_z)
   (if __acc_z (setq _acc_z __acc_z)) _acc_z)
  (:gyr_x
   (&optional __gyr_x)
   (if __gyr_x (setq _gyr_x __gyr_x)) _gyr_x)
  (:gyr_y
   (&optional __gyr_y)
   (if __gyr_y (setq _gyr_y __gyr_y)) _gyr_y)
  (:gyr_z
   (&optional __gyr_z)
   (if __gyr_z (setq _gyr_z __gyr_z)) _gyr_z)
  (:mag_x
   (&optional __mag_x)
   (if __mag_x (setq _mag_x __mag_x)) _mag_x)
  (:mag_y
   (&optional __mag_y)
   (if __mag_y (setq _mag_y __mag_y)) _mag_y)
  (:mag_z
   (&optional __mag_z)
   (if __mag_z (setq _mag_z __mag_z)) _mag_z)
  (:eul_r
   (&optional __eul_r)
   (if __eul_r (setq _eul_r __eul_r)) _eul_r)
  (:eul_p
   (&optional __eul_p)
   (if __eul_p (setq _eul_p __eul_p)) _eul_p)
  (:eul_y
   (&optional __eul_y)
   (if __eul_y (setq _eul_y __eul_y)) _eul_y)
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
  (:serialization-length
   ()
   (+
    ;; uint32 _frame_rate
    4
    ;; uint8 _tag
    1
    ;; int8 _temperature
    1
    ;; float32 _air_pressure
    4
    ;; uint32 _system_time
    4
    ;; float32 _acc_x
    4
    ;; float32 _acc_y
    4
    ;; float32 _acc_z
    4
    ;; float32 _gyr_x
    4
    ;; float32 _gyr_y
    4
    ;; float32 _gyr_z
    4
    ;; float32 _mag_x
    4
    ;; float32 _mag_y
    4
    ;; float32 _mag_z
    4
    ;; float32 _eul_r
    4
    ;; float32 _eul_p
    4
    ;; float32 _eul_y
    4
    ;; float32 _quat_w
    4
    ;; float32 _quat_x
    4
    ;; float32 _quat_y
    4
    ;; float32 _quat_z
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _frame_rate
       (write-long _frame_rate s)
     ;; uint8 _tag
       (write-byte _tag s)
     ;; int8 _temperature
       (write-byte _temperature s)
     ;; float32 _air_pressure
       (sys::poke _air_pressure (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; uint32 _system_time
       (write-long _system_time s)
     ;; float32 _acc_x
       (sys::poke _acc_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _acc_y
       (sys::poke _acc_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _acc_z
       (sys::poke _acc_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _gyr_x
       (sys::poke _gyr_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _gyr_y
       (sys::poke _gyr_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _gyr_z
       (sys::poke _gyr_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _mag_x
       (sys::poke _mag_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _mag_y
       (sys::poke _mag_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _mag_z
       (sys::poke _mag_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _eul_r
       (sys::poke _eul_r (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _eul_p
       (sys::poke _eul_p (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _eul_y
       (sys::poke _eul_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _quat_w
       (sys::poke _quat_w (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _quat_x
       (sys::poke _quat_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _quat_y
       (sys::poke _quat_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _quat_z
       (sys::poke _quat_z (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _frame_rate
     (setq _frame_rate (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint8 _tag
     (setq _tag (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; int8 _temperature
     (setq _temperature (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _temperature 127) (setq _temperature (- _temperature 256)))
   ;; float32 _air_pressure
     (setq _air_pressure (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; uint32 _system_time
     (setq _system_time (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _acc_x
     (setq _acc_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _acc_y
     (setq _acc_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _acc_z
     (setq _acc_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _gyr_x
     (setq _gyr_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _gyr_y
     (setq _gyr_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _gyr_z
     (setq _gyr_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _mag_x
     (setq _mag_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _mag_y
     (setq _mag_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _mag_z
     (setq _mag_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _eul_r
     (setq _eul_r (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _eul_p
     (setq _eul_p (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _eul_y
     (setq _eul_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _quat_w
     (setq _quat_w (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _quat_x
     (setq _quat_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _quat_y
     (setq _quat_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _quat_z
     (setq _quat_z (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get hipnuc_imu::Imu_0x92_package :md5sum-) "7fa204fc91ba06d4f14d0b3976c76670")
(setf (get hipnuc_imu::Imu_0x92_package :datatype-) "hipnuc_imu/Imu_0x92_package")
(setf (get hipnuc_imu::Imu_0x92_package :definition-)
      "uint32 frame_rate
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
")



(provide :hipnuc_imu/Imu_0x92_package "7fa204fc91ba06d4f14d0b3976c76670")


