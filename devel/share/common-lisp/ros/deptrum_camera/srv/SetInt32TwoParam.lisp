; Auto-generated. Do not edit!


(cl:in-package deptrum_camera-srv)


;//! \htmlinclude SetInt32TwoParam-request.msg.html

(cl:defclass <SetInt32TwoParam-request> (roslisp-msg-protocol:ros-message)
  ((data
    :reader data
    :initarg :data
    :type cl:integer
    :initform 0)
   (data_two
    :reader data_two
    :initarg :data_two
    :type cl:integer
    :initform 0))
)

(cl:defclass SetInt32TwoParam-request (<SetInt32TwoParam-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetInt32TwoParam-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetInt32TwoParam-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name deptrum_camera-srv:<SetInt32TwoParam-request> is deprecated: use deptrum_camera-srv:SetInt32TwoParam-request instead.")))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <SetInt32TwoParam-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:data-val is deprecated.  Use deptrum_camera-srv:data instead.")
  (data m))

(cl:ensure-generic-function 'data_two-val :lambda-list '(m))
(cl:defmethod data_two-val ((m <SetInt32TwoParam-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:data_two-val is deprecated.  Use deptrum_camera-srv:data_two instead.")
  (data_two m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetInt32TwoParam-request>) ostream)
  "Serializes a message object of type '<SetInt32TwoParam-request>"
  (cl:let* ((signed (cl:slot-value msg 'data)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'data_two)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetInt32TwoParam-request>) istream)
  "Deserializes a message object of type '<SetInt32TwoParam-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'data) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'data_two) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetInt32TwoParam-request>)))
  "Returns string type for a service object of type '<SetInt32TwoParam-request>"
  "deptrum_camera/SetInt32TwoParamRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetInt32TwoParam-request)))
  "Returns string type for a service object of type 'SetInt32TwoParam-request"
  "deptrum_camera/SetInt32TwoParamRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetInt32TwoParam-request>)))
  "Returns md5sum for a message object of type '<SetInt32TwoParam-request>"
  "bc6de20a6c902412675c277e90267ae2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetInt32TwoParam-request)))
  "Returns md5sum for a message object of type 'SetInt32TwoParam-request"
  "bc6de20a6c902412675c277e90267ae2")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetInt32TwoParam-request>)))
  "Returns full string definition for message of type '<SetInt32TwoParam-request>"
  (cl:format cl:nil "int32 data~%int32 data_two~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetInt32TwoParam-request)))
  "Returns full string definition for message of type 'SetInt32TwoParam-request"
  (cl:format cl:nil "int32 data~%int32 data_two~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetInt32TwoParam-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetInt32TwoParam-request>))
  "Converts a ROS message object to a list"
  (cl:list 'SetInt32TwoParam-request
    (cl:cons ':data (data msg))
    (cl:cons ':data_two (data_two msg))
))
;//! \htmlinclude SetInt32TwoParam-response.msg.html

(cl:defclass <SetInt32TwoParam-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (message
    :reader message
    :initarg :message
    :type cl:string
    :initform ""))
)

(cl:defclass SetInt32TwoParam-response (<SetInt32TwoParam-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetInt32TwoParam-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetInt32TwoParam-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name deptrum_camera-srv:<SetInt32TwoParam-response> is deprecated: use deptrum_camera-srv:SetInt32TwoParam-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <SetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:success-val is deprecated.  Use deptrum_camera-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <SetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:message-val is deprecated.  Use deptrum_camera-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetInt32TwoParam-response>) ostream)
  "Serializes a message object of type '<SetInt32TwoParam-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetInt32TwoParam-response>) istream)
  "Deserializes a message object of type '<SetInt32TwoParam-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetInt32TwoParam-response>)))
  "Returns string type for a service object of type '<SetInt32TwoParam-response>"
  "deptrum_camera/SetInt32TwoParamResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetInt32TwoParam-response)))
  "Returns string type for a service object of type 'SetInt32TwoParam-response"
  "deptrum_camera/SetInt32TwoParamResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetInt32TwoParam-response>)))
  "Returns md5sum for a message object of type '<SetInt32TwoParam-response>"
  "bc6de20a6c902412675c277e90267ae2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetInt32TwoParam-response)))
  "Returns md5sum for a message object of type 'SetInt32TwoParam-response"
  "bc6de20a6c902412675c277e90267ae2")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetInt32TwoParam-response>)))
  "Returns full string definition for message of type '<SetInt32TwoParam-response>"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetInt32TwoParam-response)))
  "Returns full string definition for message of type 'SetInt32TwoParam-response"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetInt32TwoParam-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetInt32TwoParam-response>))
  "Converts a ROS message object to a list"
  (cl:list 'SetInt32TwoParam-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'SetInt32TwoParam)))
  'SetInt32TwoParam-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'SetInt32TwoParam)))
  'SetInt32TwoParam-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetInt32TwoParam)))
  "Returns string type for a service object of type '<SetInt32TwoParam>"
  "deptrum_camera/SetInt32TwoParam")