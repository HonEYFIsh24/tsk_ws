// Auto-generated. Do not edit!

// (in-package hipnuc_imu.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let Imu_0x91_package = require('./Imu_0x91_package.js');
let Imu_0x92_package = require('./Imu_0x92_package.js');
let Ins_0x81_package = require('./Ins_0x81_package.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class hipnuc_imu_msg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.hi91_data = null;
      this.hi92_data = null;
      this.hi81_data = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('hi91_data')) {
        this.hi91_data = initObj.hi91_data
      }
      else {
        this.hi91_data = new Imu_0x91_package();
      }
      if (initObj.hasOwnProperty('hi92_data')) {
        this.hi92_data = initObj.hi92_data
      }
      else {
        this.hi92_data = new Imu_0x92_package();
      }
      if (initObj.hasOwnProperty('hi81_data')) {
        this.hi81_data = initObj.hi81_data
      }
      else {
        this.hi81_data = new Ins_0x81_package();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type hipnuc_imu_msg
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [hi91_data]
    bufferOffset = Imu_0x91_package.serialize(obj.hi91_data, buffer, bufferOffset);
    // Serialize message field [hi92_data]
    bufferOffset = Imu_0x92_package.serialize(obj.hi92_data, buffer, bufferOffset);
    // Serialize message field [hi81_data]
    bufferOffset = Ins_0x81_package.serialize(obj.hi81_data, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type hipnuc_imu_msg
    let len;
    let data = new hipnuc_imu_msg(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [hi91_data]
    data.hi91_data = Imu_0x91_package.deserialize(buffer, bufferOffset);
    // Deserialize message field [hi92_data]
    data.hi92_data = Imu_0x92_package.deserialize(buffer, bufferOffset);
    // Deserialize message field [hi81_data]
    data.hi81_data = Ins_0x81_package.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 262;
  }

  static datatype() {
    // Returns string type for a message object
    return 'hipnuc_imu/hipnuc_imu_msg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'fb9f1cc5f720d9cd4bf3b0cab56ae7c9';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    Imu_0x91_package hi91_data
    Imu_0x92_package hi92_data
    Ins_0x81_package hi81_data
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    ================================================================================
    MSG: hipnuc_imu/Imu_0x91_package
    uint32 frame_rate
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
    ================================================================================
    MSG: hipnuc_imu/Imu_0x92_package
    uint32 frame_rate
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
    ================================================================================
    MSG: hipnuc_imu/Ins_0x81_package
    uint32 frame_rate
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
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new hipnuc_imu_msg(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.hi91_data !== undefined) {
      resolved.hi91_data = Imu_0x91_package.Resolve(msg.hi91_data)
    }
    else {
      resolved.hi91_data = new Imu_0x91_package()
    }

    if (msg.hi92_data !== undefined) {
      resolved.hi92_data = Imu_0x92_package.Resolve(msg.hi92_data)
    }
    else {
      resolved.hi92_data = new Imu_0x92_package()
    }

    if (msg.hi81_data !== undefined) {
      resolved.hi81_data = Ins_0x81_package.Resolve(msg.hi81_data)
    }
    else {
      resolved.hi81_data = new Ins_0x81_package()
    }

    return resolved;
    }
};

module.exports = hipnuc_imu_msg;
