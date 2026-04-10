
(cl:in-package :asdf)

(defsystem "hipnuc_imu-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "Imu_0x91_package" :depends-on ("_package_Imu_0x91_package"))
    (:file "_package_Imu_0x91_package" :depends-on ("_package"))
    (:file "Imu_0x92_package" :depends-on ("_package_Imu_0x92_package"))
    (:file "_package_Imu_0x92_package" :depends-on ("_package"))
    (:file "Ins_0x81_package" :depends-on ("_package_Ins_0x81_package"))
    (:file "_package_Ins_0x81_package" :depends-on ("_package"))
    (:file "hipnuc_imu_msg" :depends-on ("_package_hipnuc_imu_msg"))
    (:file "_package_hipnuc_imu_msg" :depends-on ("_package"))
  ))