;; Auto-generated. Do not edit!


(when (boundp 'deptrum_camera::GetInt32TwoParam)
  (if (not (find-package "DEPTRUM_CAMERA"))
    (make-package "DEPTRUM_CAMERA"))
  (shadow 'GetInt32TwoParam (find-package "DEPTRUM_CAMERA")))
(unless (find-package "DEPTRUM_CAMERA::GETINT32TWOPARAM")
  (make-package "DEPTRUM_CAMERA::GETINT32TWOPARAM"))
(unless (find-package "DEPTRUM_CAMERA::GETINT32TWOPARAMREQUEST")
  (make-package "DEPTRUM_CAMERA::GETINT32TWOPARAMREQUEST"))
(unless (find-package "DEPTRUM_CAMERA::GETINT32TWOPARAMRESPONSE")
  (make-package "DEPTRUM_CAMERA::GETINT32TWOPARAMRESPONSE"))

(in-package "ROS")





(defclass deptrum_camera::GetInt32TwoParamRequest
  :super ros::object
  :slots ())

(defmethod deptrum_camera::GetInt32TwoParamRequest
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

(defclass deptrum_camera::GetInt32TwoParamResponse
  :super ros::object
  :slots (_data _data_two _success _message ))

(defmethod deptrum_camera::GetInt32TwoParamResponse
  (:init
   (&key
    ((:data __data) 0)
    ((:data_two __data_two) 0)
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _data (round __data))
   (setq _data_two (round __data_two))
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:data_two
   (&optional __data_two)
   (if __data_two (setq _data_two __data_two)) _data_two)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; int32 _data
    4
    ;; int32 _data_two
    4
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32 _data
       (write-long _data s)
     ;; int32 _data_two
       (write-long _data_two s)
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32 _data
     (setq _data (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _data_two
     (setq _data_two (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass deptrum_camera::GetInt32TwoParam
  :super ros::object
  :slots ())

(setf (get deptrum_camera::GetInt32TwoParam :md5sum-) "de440921d13d18cca98fe886c3de143e")
(setf (get deptrum_camera::GetInt32TwoParam :datatype-) "deptrum_camera/GetInt32TwoParam")
(setf (get deptrum_camera::GetInt32TwoParam :request) deptrum_camera::GetInt32TwoParamRequest)
(setf (get deptrum_camera::GetInt32TwoParam :response) deptrum_camera::GetInt32TwoParamResponse)

(defmethod deptrum_camera::GetInt32TwoParamRequest
  (:response () (instance deptrum_camera::GetInt32TwoParamResponse :init)))

(setf (get deptrum_camera::GetInt32TwoParamRequest :md5sum-) "de440921d13d18cca98fe886c3de143e")
(setf (get deptrum_camera::GetInt32TwoParamRequest :datatype-) "deptrum_camera/GetInt32TwoParamRequest")
(setf (get deptrum_camera::GetInt32TwoParamRequest :definition-)
      "---
int32 data
int32 data_two
bool success
string message

")

(setf (get deptrum_camera::GetInt32TwoParamResponse :md5sum-) "de440921d13d18cca98fe886c3de143e")
(setf (get deptrum_camera::GetInt32TwoParamResponse :datatype-) "deptrum_camera/GetInt32TwoParamResponse")
(setf (get deptrum_camera::GetInt32TwoParamResponse :definition-)
      "---
int32 data
int32 data_two
bool success
string message

")



(provide :deptrum_camera/GetInt32TwoParam "de440921d13d18cca98fe886c3de143e")


