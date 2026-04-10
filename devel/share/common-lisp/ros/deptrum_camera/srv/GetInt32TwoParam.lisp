; Auto-generated. Do not edit!


(cl:in-package deptrum_camera-srv)


;//! \htmlinclude GetInt32TwoParam-request.msg.html

(cl:defclass <GetInt32TwoParam-request> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass GetInt32TwoParam-request (<GetInt32TwoParam-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GetInt32TwoParam-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GetInt32TwoParam-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name deptrum_camera-srv:<GetInt32TwoParam-request> is deprecated: use deptrum_camera-srv:GetInt32TwoParam-request instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GetInt32TwoParam-request>) ostream)
  "Serializes a message object of type '<GetInt32TwoParam-request>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GetInt32TwoParam-request>) istream)
  "Deserializes a message object of type '<GetInt32TwoParam-request>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GetInt32TwoParam-request>)))
  "Returns string type for a service object of type '<GetInt32TwoParam-request>"
  "deptrum_camera/GetInt32TwoParamRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GetInt32TwoParam-request)))
  "Returns string type for a service object of type 'GetInt32TwoParam-request"
  "deptrum_camera/GetInt32TwoParamRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GetInt32TwoParam-request>)))
  "Returns md5sum for a message object of type '<GetInt32TwoParam-request>"
  "de440921d13d18cca98fe886c3de143e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GetInt32TwoParam-request)))
  "Returns md5sum for a message object of type 'GetInt32TwoParam-request"
  "de440921d13d18cca98fe886c3de143e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GetInt32TwoParam-request>)))
  "Returns full string definition for message of type '<GetInt32TwoParam-request>"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GetInt32TwoParam-request)))
  "Returns full string definition for message of type 'GetInt32TwoParam-request"
  (cl:format cl:nil "~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GetInt32TwoParam-request>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GetInt32TwoParam-request>))
  "Converts a ROS message object to a list"
  (cl:list 'GetInt32TwoParam-request
))
;//! \htmlinclude GetInt32TwoParam-response.msg.html

(cl:defclass <GetInt32TwoParam-response> (roslisp-msg-protocol:ros-message)
  ((data
    :reader data
    :initarg :data
    :type cl:integer
    :initform 0)
   (data_two
    :reader data_two
    :initarg :data_two
    :type cl:integer
    :initform 0)
   (success
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

(cl:defclass GetInt32TwoParam-response (<GetInt32TwoParam-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GetInt32TwoParam-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GetInt32TwoParam-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name deptrum_camera-srv:<GetInt32TwoParam-response> is deprecated: use deptrum_camera-srv:GetInt32TwoParam-response instead.")))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <GetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:data-val is deprecated.  Use deptrum_camera-srv:data instead.")
  (data m))

(cl:ensure-generic-function 'data_two-val :lambda-list '(m))
(cl:defmethod data_two-val ((m <GetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:data_two-val is deprecated.  Use deptrum_camera-srv:data_two instead.")
  (data_two m))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <GetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:success-val is deprecated.  Use deptrum_camera-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <GetInt32TwoParam-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader deptrum_camera-srv:message-val is deprecated.  Use deptrum_camera-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GetInt32TwoParam-response>) ostream)
  "Serializes a message object of type '<GetInt32TwoParam-response>"
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
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GetInt32TwoParam-response>) istream)
  "Deserializes a message object of type '<GetInt32TwoParam-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GetInt32TwoParam-response>)))
  "Returns string type for a service object of type '<GetInt32TwoParam-response>"
  "deptrum_camera/GetInt32TwoParamResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GetInt32TwoParam-response)))
  "Returns string type for a service object of type 'GetInt32TwoParam-response"
  "deptrum_camera/GetInt32TwoParamResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GetInt32TwoParam-response>)))
  "Returns md5sum for a message object of type '<GetInt32TwoParam-response>"
  "de440921d13d18cca98fe886c3de143e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GetInt32TwoParam-response)))
  "Returns md5sum for a message object of type 'GetInt32TwoParam-response"
  "de440921d13d18cca98fe886c3de143e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GetInt32TwoParam-response>)))
  "Returns full string definition for message of type '<GetInt32TwoParam-response>"
  (cl:format cl:nil "int32 data~%int32 data_two~%bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GetInt32TwoParam-response)))
  "Returns full string definition for message of type 'GetInt32TwoParam-response"
  (cl:format cl:nil "int32 data~%int32 data_two~%bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GetInt32TwoParam-response>))
  (cl:+ 0
     4
     4
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GetInt32TwoParam-response>))
  "Converts a ROS message object to a list"
  (cl:list 'GetInt32TwoParam-response
    (cl:cons ':data (data msg))
    (cl:cons ':data_two (data_two msg))
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'GetInt32TwoParam)))
  'GetInt32TwoParam-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'GetInt32TwoParam)))
  'GetInt32TwoParam-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GetInt32TwoParam)))
  "Returns string type for a service object of type '<GetInt32TwoParam>"
  "deptrum_camera/GetInt32TwoParam")