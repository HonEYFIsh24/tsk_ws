;; Auto-generated. Do not edit!


(when (boundp 'deptrum_camera::GetBool)
  (if (not (find-package "DEPTRUM_CAMERA"))
    (make-package "DEPTRUM_CAMERA"))
  (shadow 'GetBool (find-package "DEPTRUM_CAMERA")))
(unless (find-package "DEPTRUM_CAMERA::GETBOOL")
  (make-package "DEPTRUM_CAMERA::GETBOOL"))
(unless (find-package "DEPTRUM_CAMERA::GETBOOLREQUEST")
  (make-package "DEPTRUM_CAMERA::GETBOOLREQUEST"))
(unless (find-package "DEPTRUM_CAMERA::GETBOOLRESPONSE")
  (make-package "DEPTRUM_CAMERA::GETBOOLRESPONSE"))

(in-package "ROS")





(defclass deptrum_camera::GetBoolRequest
  :super ros::object
  :slots ())

(defmethod deptrum_camera::GetBoolRequest
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass deptrum_camera::GetBoolResponse
  :super ros::object
  :slots (_data _success _message ))

(defmethod deptrum_camera::GetBoolResponse
  (:init
   (&key
    ((:data __data) nil)
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _data __data)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:data
   (&optional (__data :null))
   (if (not (eq __data :null)) (setq _data __data)) _data)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _data
    1
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _data
       (if _data (write-byte -1 s) (write-byte 0 s))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _data
     (setq _data (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass deptrum_camera::GetBool
  :super ros::object
  :slots ())

(setf (get deptrum_camera::GetBool :md5sum-) "43f76a855b16e988566e147b692f2fab")
(setf (get deptrum_camera::GetBool :datatype-) "deptrum_camera/GetBool")
(setf (get deptrum_camera::GetBool :request) deptrum_camera::GetBoolRequest)
(setf (get deptrum_camera::GetBool :response) deptrum_camera::GetBoolResponse)

(defmethod deptrum_camera::GetBoolRequest
  (:response () (instance deptrum_camera::GetBoolResponse :init)))

(setf (get deptrum_camera::GetBoolRequest :md5sum-) "43f76a855b16e988566e147b692f2fab")
(setf (get deptrum_camera::GetBoolRequest :datatype-) "deptrum_camera/GetBoolRequest")
(setf (get deptrum_camera::GetBoolRequest :definition-)
      "---
bool data
bool success
string message

")

(setf (get deptrum_camera::GetBoolResponse :md5sum-) "43f76a855b16e988566e147b692f2fab")
(setf (get deptrum_camera::GetBoolResponse :datatype-) "deptrum_camera/GetBoolResponse")
(setf (get deptrum_camera::GetBoolResponse :definition-)
      "---
bool data
bool success
string message

")



(provide :deptrum_camera/GetBool "43f76a855b16e988566e147b692f2fab")


