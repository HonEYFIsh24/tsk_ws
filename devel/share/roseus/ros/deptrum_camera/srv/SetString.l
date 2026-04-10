;; Auto-generated. Do not edit!


(when (boundp 'deptrum_camera::SetString)
  (if (not (find-package "DEPTRUM_CAMERA"))
    (make-package "DEPTRUM_CAMERA"))
  (shadow 'SetString (find-package "DEPTRUM_CAMERA")))
(unless (find-package "DEPTRUM_CAMERA::SETSTRING")
  (make-package "DEPTRUM_CAMERA::SETSTRING"))
(unless (find-package "DEPTRUM_CAMERA::SETSTRINGREQUEST")
  (make-package "DEPTRUM_CAMERA::SETSTRINGREQUEST"))
(unless (find-package "DEPTRUM_CAMERA::SETSTRINGRESPONSE")
  (make-package "DEPTRUM_CAMERA::SETSTRINGRESPONSE"))

(in-package "ROS")





(defclass deptrum_camera::SetStringRequest
  :super ros::object
  :slots (_data ))

(defmethod deptrum_camera::SetStringRequest
  (:init
   (&key
    ((:data __data) "")
    )
   (send-super :init)
   (setq _data (string __data))
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; string _data
    4 (length _data)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _data
       (write-long (length _data) s) (princ _data s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _data
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _data (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass deptrum_camera::SetStringResponse
  :super ros::object
  :slots (_success _message ))

(defmethod deptrum_camera::SetStringResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:success
   (&optional (__success :null))
   (if (not (eq __success :null)) (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass deptrum_camera::SetString
  :super ros::object
  :slots ())

(setf (get deptrum_camera::SetString :md5sum-) "546971982e3fbbd5a41e60fb6432e357")
(setf (get deptrum_camera::SetString :datatype-) "deptrum_camera/SetString")
(setf (get deptrum_camera::SetString :request) deptrum_camera::SetStringRequest)
(setf (get deptrum_camera::SetString :response) deptrum_camera::SetStringResponse)

(defmethod deptrum_camera::SetStringRequest
  (:response () (instance deptrum_camera::SetStringResponse :init)))

(setf (get deptrum_camera::SetStringRequest :md5sum-) "546971982e3fbbd5a41e60fb6432e357")
(setf (get deptrum_camera::SetStringRequest :datatype-) "deptrum_camera/SetStringRequest")
(setf (get deptrum_camera::SetStringRequest :definition-)
      "string data
---
bool success
string message

")

(setf (get deptrum_camera::SetStringResponse :md5sum-) "546971982e3fbbd5a41e60fb6432e357")
(setf (get deptrum_camera::SetStringResponse :datatype-) "deptrum_camera/SetStringResponse")
(setf (get deptrum_camera::SetStringResponse :definition-)
      "string data
---
bool success
string message

")



(provide :deptrum_camera/SetString "546971982e3fbbd5a41e60fb6432e357")


