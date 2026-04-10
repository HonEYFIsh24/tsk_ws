// Auto-generated. Do not edit!

// (in-package hipnuc_imu.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class Ins_0x81_package {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.frame_rate = null;
      this.tag = null;
      this.status = null;
      this.ins_status = null;
      this.gpst_wn = null;
      this.gpst_tow = null;
      this.gyr_b_x = null;
      this.gyr_b_y = null;
      this.gyr_b_z = null;
      this.acc_b_x = null;
      this.acc_b_y = null;
      this.acc_b_z = null;
      this.mag_b_x = null;
      this.mag_b_y = null;
      this.mag_b_z = null;
      this.air_pressure = null;
      this.reserved_1 = null;
      this.reserved_2 = null;
      this.temperature = null;
      this.utc_year = null;
      this.utc_mouth = null;
      this.utc_day = null;
      this.utc_hour = null;
      this.utc_min = null;
      this.utc_sec = null;
      this.roll = null;
      this.pitch = null;
      this.yaw = null;
      this.quat_w = null;
      this.quat_x = null;
      this.quat_y = null;
      this.quat_z = null;
      this.ins_lon = null;
      this.ins_lat = null;
      this.ins_msl = null;
      this.pdop = null;
      this.hdop = null;
      this.solq_pos = null;
      this.nv_pos = null;
      this.solq_heading = null;
      this.nv_heading = null;
      this.diff_age = null;
      this.undulation = null;
      this.reserved_3 = null;
      this.vel_enu_1 = null;
      this.vel_enu_2 = null;
      this.vel_enu_3 = null;
      this.acc_enu_1 = null;
      this.acc_enu_2 = null;
      this.acc_enu_3 = null;
      this.reserved_4 = null;
      this.reserved_5 = null;
      this.reserved_6 = null;
      this.reserved_7 = null;
    }
    else {
      if (initObj.hasOwnProperty('frame_rate')) {
        this.frame_rate = initObj.frame_rate
      }
      else {
        this.frame_rate = 0;
      }
      if (initObj.hasOwnProperty('tag')) {
        this.tag = initObj.tag
      }
      else {
        this.tag = 0;
      }
      if (initObj.hasOwnProperty('status')) {
        this.status = initObj.status
      }
      else {
        this.status = 0;
      }
      if (initObj.hasOwnProperty('ins_status')) {
        this.ins_status = initObj.ins_status
      }
      else {
        this.ins_status = 0;
      }
      if (initObj.hasOwnProperty('gpst_wn')) {
        this.gpst_wn = initObj.gpst_wn
      }
      else {
        this.gpst_wn = 0;
      }
      if (initObj.hasOwnProperty('gpst_tow')) {
        this.gpst_tow = initObj.gpst_tow
      }
      else {
        this.gpst_tow = 0;
      }
      if (initObj.hasOwnProperty('gyr_b_x')) {
        this.gyr_b_x = initObj.gyr_b_x
      }
      else {
        this.gyr_b_x = 0;
      }
      if (initObj.hasOwnProperty('gyr_b_y')) {
        this.gyr_b_y = initObj.gyr_b_y
      }
      else {
        this.gyr_b_y = 0;
      }
      if (initObj.hasOwnProperty('gyr_b_z')) {
        this.gyr_b_z = initObj.gyr_b_z
      }
      else {
        this.gyr_b_z = 0;
      }
      if (initObj.hasOwnProperty('acc_b_x')) {
        this.acc_b_x = initObj.acc_b_x
      }
      else {
        this.acc_b_x = 0;
      }
      if (initObj.hasOwnProperty('acc_b_y')) {
        this.acc_b_y = initObj.acc_b_y
      }
      else {
        this.acc_b_y = 0;
      }
      if (initObj.hasOwnProperty('acc_b_z')) {
        this.acc_b_z = initObj.acc_b_z
      }
      else {
        this.acc_b_z = 0;
      }
      if (initObj.hasOwnProperty('mag_b_x')) {
        this.mag_b_x = initObj.mag_b_x
      }
      else {
        this.mag_b_x = 0;
      }
      if (initObj.hasOwnProperty('mag_b_y')) {
        this.mag_b_y = initObj.mag_b_y
      }
      else {
        this.mag_b_y = 0;
      }
      if (initObj.hasOwnProperty('mag_b_z')) {
        this.mag_b_z = initObj.mag_b_z
      }
      else {
        this.mag_b_z = 0;
      }
      if (initObj.hasOwnProperty('air_pressure')) {
        this.air_pressure = initObj.air_pressure
      }
      else {
        this.air_pressure = 0;
      }
      if (initObj.hasOwnProperty('reserved_1')) {
        this.reserved_1 = initObj.reserved_1
      }
      else {
        this.reserved_1 = 0;
      }
      if (initObj.hasOwnProperty('reserved_2')) {
        this.reserved_2 = initObj.reserved_2
      }
      else {
        this.reserved_2 = 0;
      }
      if (initObj.hasOwnProperty('temperature')) {
        this.temperature = initObj.temperature
      }
      else {
        this.temperature = 0;
      }
      if (initObj.hasOwnProperty('utc_year')) {
        this.utc_year = initObj.utc_year
      }
      else {
        this.utc_year = 0;
      }
      if (initObj.hasOwnProperty('utc_mouth')) {
        this.utc_mouth = initObj.utc_mouth
      }
      else {
        this.utc_mouth = 0;
      }
      if (initObj.hasOwnProperty('utc_day')) {
        this.utc_day = initObj.utc_day
      }
      else {
        this.utc_day = 0;
      }
      if (initObj.hasOwnProperty('utc_hour')) {
        this.utc_hour = initObj.utc_hour
      }
      else {
        this.utc_hour = 0;
      }
      if (initObj.hasOwnProperty('utc_min')) {
        this.utc_min = initObj.utc_min
      }
      else {
        this.utc_min = 0;
      }
      if (initObj.hasOwnProperty('utc_sec')) {
        this.utc_sec = initObj.utc_sec
      }
      else {
        this.utc_sec = 0;
      }
      if (initObj.hasOwnProperty('roll')) {
        this.roll = initObj.roll
      }
      else {
        this.roll = 0;
      }
      if (initObj.hasOwnProperty('pitch')) {
        this.pitch = initObj.pitch
      }
      else {
        this.pitch = 0;
      }
      if (initObj.hasOwnProperty('yaw')) {
        this.yaw = initObj.yaw
      }
      else {
        this.yaw = 0;
      }
      if (initObj.hasOwnProperty('quat_w')) {
        this.quat_w = initObj.quat_w
      }
      else {
        this.quat_w = 0;
      }
      if (initObj.hasOwnProperty('quat_x')) {
        this.quat_x = initObj.quat_x
      }
      else {
        this.quat_x = 0;
      }
      if (initObj.hasOwnProperty('quat_y')) {
        this.quat_y = initObj.quat_y
      }
      else {
        this.quat_y = 0;
      }
      if (initObj.hasOwnProperty('quat_z')) {
        this.quat_z = initObj.quat_z
      }
      else {
        this.quat_z = 0;
      }
      if (initObj.hasOwnProperty('ins_lon')) {
        this.ins_lon = initObj.ins_lon
      }
      else {
        this.ins_lon = 0;
      }
      if (initObj.hasOwnProperty('ins_lat')) {
        this.ins_lat = initObj.ins_lat
      }
      else {
        this.ins_lat = 0;
      }
      if (initObj.hasOwnProperty('ins_msl')) {
        this.ins_msl = initObj.ins_msl
      }
      else {
        this.ins_msl = 0;
      }
      if (initObj.hasOwnProperty('pdop')) {
        this.pdop = initObj.pdop
      }
      else {
        this.pdop = 0;
      }
      if (initObj.hasOwnProperty('hdop')) {
        this.hdop = initObj.hdop
      }
      else {
        this.hdop = 0;
      }
      if (initObj.hasOwnProperty('solq_pos')) {
        this.solq_pos = initObj.solq_pos
      }
      else {
        this.solq_pos = 0;
      }
      if (initObj.hasOwnProperty('nv_pos')) {
        this.nv_pos = initObj.nv_pos
      }
      else {
        this.nv_pos = 0;
      }
      if (initObj.hasOwnProperty('solq_heading')) {
        this.solq_heading = initObj.solq_heading
      }
      else {
        this.solq_heading = 0;
      }
      if (initObj.hasOwnProperty('nv_heading')) {
        this.nv_heading = initObj.nv_heading
      }
      else {
        this.nv_heading = 0;
      }
      if (initObj.hasOwnProperty('diff_age')) {
        this.diff_age = initObj.diff_age
      }
      else {
        this.diff_age = 0;
      }
      if (initObj.hasOwnProperty('undulation')) {
        this.undulation = initObj.undulation
      }
      else {
        this.undulation = 0;
      }
      if (initObj.hasOwnProperty('reserved_3')) {
        this.reserved_3 = initObj.reserved_3
      }
      else {
        this.reserved_3 = 0;
      }
      if (initObj.hasOwnProperty('vel_enu_1')) {
        this.vel_enu_1 = initObj.vel_enu_1
      }
      else {
        this.vel_enu_1 = 0;
      }
      if (initObj.hasOwnProperty('vel_enu_2')) {
        this.vel_enu_2 = initObj.vel_enu_2
      }
      else {
        this.vel_enu_2 = 0;
      }
      if (initObj.hasOwnProperty('vel_enu_3')) {
        this.vel_enu_3 = initObj.vel_enu_3
      }
      else {
        this.vel_enu_3 = 0;
      }
      if (initObj.hasOwnProperty('acc_enu_1')) {
        this.acc_enu_1 = initObj.acc_enu_1
      }
      else {
        this.acc_enu_1 = 0;
      }
      if (initObj.hasOwnProperty('acc_enu_2')) {
        this.acc_enu_2 = initObj.acc_enu_2
      }
      else {
        this.acc_enu_2 = 0;
      }
      if (initObj.hasOwnProperty('acc_enu_3')) {
        this.acc_enu_3 = initObj.acc_enu_3
      }
      else {
        this.acc_enu_3 = 0;
      }
      if (initObj.hasOwnProperty('reserved_4')) {
        this.reserved_4 = initObj.reserved_4
      }
      else {
        this.reserved_4 = 0;
      }
      if (initObj.hasOwnProperty('reserved_5')) {
        this.reserved_5 = initObj.reserved_5
      }
      else {
        this.reserved_5 = 0;
      }
      if (initObj.hasOwnProperty('reserved_6')) {
        this.reserved_6 = initObj.reserved_6
      }
      else {
        this.reserved_6 = 0;
      }
      if (initObj.hasOwnProperty('reserved_7')) {
        this.reserved_7 = initObj.reserved_7
      }
      else {
        this.reserved_7 = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Ins_0x81_package
    // Serialize message field [frame_rate]
    bufferOffset = _serializer.uint32(obj.frame_rate, buffer, bufferOffset);
    // Serialize message field [tag]
    bufferOffset = _serializer.uint8(obj.tag, buffer, bufferOffset);
    // Serialize message field [status]
    bufferOffset = _serializer.uint16(obj.status, buffer, bufferOffset);
    // Serialize message field [ins_status]
    bufferOffset = _serializer.uint8(obj.ins_status, buffer, bufferOffset);
    // Serialize message field [gpst_wn]
    bufferOffset = _serializer.uint16(obj.gpst_wn, buffer, bufferOffset);
    // Serialize message field [gpst_tow]
    bufferOffset = _serializer.uint32(obj.gpst_tow, buffer, bufferOffset);
    // Serialize message field [gyr_b_x]
    bufferOffset = _serializer.int16(obj.gyr_b_x, buffer, bufferOffset);
    // Serialize message field [gyr_b_y]
    bufferOffset = _serializer.int16(obj.gyr_b_y, buffer, bufferOffset);
    // Serialize message field [gyr_b_z]
    bufferOffset = _serializer.int16(obj.gyr_b_z, buffer, bufferOffset);
    // Serialize message field [acc_b_x]
    bufferOffset = _serializer.int16(obj.acc_b_x, buffer, bufferOffset);
    // Serialize message field [acc_b_y]
    bufferOffset = _serializer.int16(obj.acc_b_y, buffer, bufferOffset);
    // Serialize message field [acc_b_z]
    bufferOffset = _serializer.int16(obj.acc_b_z, buffer, bufferOffset);
    // Serialize message field [mag_b_x]
    bufferOffset = _serializer.int16(obj.mag_b_x, buffer, bufferOffset);
    // Serialize message field [mag_b_y]
    bufferOffset = _serializer.int16(obj.mag_b_y, buffer, bufferOffset);
    // Serialize message field [mag_b_z]
    bufferOffset = _serializer.int16(obj.mag_b_z, buffer, bufferOffset);
    // Serialize message field [air_pressure]
    bufferOffset = _serializer.int16(obj.air_pressure, buffer, bufferOffset);
    // Serialize message field [reserved_1]
    bufferOffset = _serializer.uint8(obj.reserved_1, buffer, bufferOffset);
    // Serialize message field [reserved_2]
    bufferOffset = _serializer.uint8(obj.reserved_2, buffer, bufferOffset);
    // Serialize message field [temperature]
    bufferOffset = _serializer.int8(obj.temperature, buffer, bufferOffset);
    // Serialize message field [utc_year]
    bufferOffset = _serializer.uint8(obj.utc_year, buffer, bufferOffset);
    // Serialize message field [utc_mouth]
    bufferOffset = _serializer.uint8(obj.utc_mouth, buffer, bufferOffset);
    // Serialize message field [utc_day]
    bufferOffset = _serializer.uint8(obj.utc_day, buffer, bufferOffset);
    // Serialize message field [utc_hour]
    bufferOffset = _serializer.uint8(obj.utc_hour, buffer, bufferOffset);
    // Serialize message field [utc_min]
    bufferOffset = _serializer.uint8(obj.utc_min, buffer, bufferOffset);
    // Serialize message field [utc_sec]
    bufferOffset = _serializer.uint16(obj.utc_sec, buffer, bufferOffset);
    // Serialize message field [roll]
    bufferOffset = _serializer.int16(obj.roll, buffer, bufferOffset);
    // Serialize message field [pitch]
    bufferOffset = _serializer.int16(obj.pitch, buffer, bufferOffset);
    // Serialize message field [yaw]
    bufferOffset = _serializer.uint16(obj.yaw, buffer, bufferOffset);
    // Serialize message field [quat_w]
    bufferOffset = _serializer.int16(obj.quat_w, buffer, bufferOffset);
    // Serialize message field [quat_x]
    bufferOffset = _serializer.int16(obj.quat_x, buffer, bufferOffset);
    // Serialize message field [quat_y]
    bufferOffset = _serializer.int16(obj.quat_y, buffer, bufferOffset);
    // Serialize message field [quat_z]
    bufferOffset = _serializer.int16(obj.quat_z, buffer, bufferOffset);
    // Serialize message field [ins_lon]
    bufferOffset = _serializer.int32(obj.ins_lon, buffer, bufferOffset);
    // Serialize message field [ins_lat]
    bufferOffset = _serializer.int32(obj.ins_lat, buffer, bufferOffset);
    // Serialize message field [ins_msl]
    bufferOffset = _serializer.int32(obj.ins_msl, buffer, bufferOffset);
    // Serialize message field [pdop]
    bufferOffset = _serializer.uint8(obj.pdop, buffer, bufferOffset);
    // Serialize message field [hdop]
    bufferOffset = _serializer.uint8(obj.hdop, buffer, bufferOffset);
    // Serialize message field [solq_pos]
    bufferOffset = _serializer.uint8(obj.solq_pos, buffer, bufferOffset);
    // Serialize message field [nv_pos]
    bufferOffset = _serializer.uint8(obj.nv_pos, buffer, bufferOffset);
    // Serialize message field [solq_heading]
    bufferOffset = _serializer.uint8(obj.solq_heading, buffer, bufferOffset);
    // Serialize message field [nv_heading]
    bufferOffset = _serializer.uint8(obj.nv_heading, buffer, bufferOffset);
    // Serialize message field [diff_age]
    bufferOffset = _serializer.uint8(obj.diff_age, buffer, bufferOffset);
    // Serialize message field [undulation]
    bufferOffset = _serializer.int16(obj.undulation, buffer, bufferOffset);
    // Serialize message field [reserved_3]
    bufferOffset = _serializer.uint8(obj.reserved_3, buffer, bufferOffset);
    // Serialize message field [vel_enu_1]
    bufferOffset = _serializer.int16(obj.vel_enu_1, buffer, bufferOffset);
    // Serialize message field [vel_enu_2]
    bufferOffset = _serializer.int16(obj.vel_enu_2, buffer, bufferOffset);
    // Serialize message field [vel_enu_3]
    bufferOffset = _serializer.int16(obj.vel_enu_3, buffer, bufferOffset);
    // Serialize message field [acc_enu_1]
    bufferOffset = _serializer.int16(obj.acc_enu_1, buffer, bufferOffset);
    // Serialize message field [acc_enu_2]
    bufferOffset = _serializer.int16(obj.acc_enu_2, buffer, bufferOffset);
    // Serialize message field [acc_enu_3]
    bufferOffset = _serializer.int16(obj.acc_enu_3, buffer, bufferOffset);
    // Serialize message field [reserved_4]
    bufferOffset = _serializer.uint32(obj.reserved_4, buffer, bufferOffset);
    // Serialize message field [reserved_5]
    bufferOffset = _serializer.uint32(obj.reserved_5, buffer, bufferOffset);
    // Serialize message field [reserved_6]
    bufferOffset = _serializer.uint32(obj.reserved_6, buffer, bufferOffset);
    // Serialize message field [reserved_7]
    bufferOffset = _serializer.uint16(obj.reserved_7, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Ins_0x81_package
    let len;
    let data = new Ins_0x81_package(null);
    // Deserialize message field [frame_rate]
    data.frame_rate = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [tag]
    data.tag = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [status]
    data.status = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [ins_status]
    data.ins_status = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [gpst_wn]
    data.gpst_wn = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [gpst_tow]
    data.gpst_tow = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [gyr_b_x]
    data.gyr_b_x = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [gyr_b_y]
    data.gyr_b_y = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [gyr_b_z]
    data.gyr_b_z = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_b_x]
    data.acc_b_x = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_b_y]
    data.acc_b_y = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_b_z]
    data.acc_b_z = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [mag_b_x]
    data.mag_b_x = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [mag_b_y]
    data.mag_b_y = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [mag_b_z]
    data.mag_b_z = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [air_pressure]
    data.air_pressure = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [reserved_1]
    data.reserved_1 = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [reserved_2]
    data.reserved_2 = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [temperature]
    data.temperature = _deserializer.int8(buffer, bufferOffset);
    // Deserialize message field [utc_year]
    data.utc_year = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [utc_mouth]
    data.utc_mouth = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [utc_day]
    data.utc_day = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [utc_hour]
    data.utc_hour = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [utc_min]
    data.utc_min = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [utc_sec]
    data.utc_sec = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [roll]
    data.roll = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [pitch]
    data.pitch = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [yaw]
    data.yaw = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [quat_w]
    data.quat_w = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [quat_x]
    data.quat_x = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [quat_y]
    data.quat_y = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [quat_z]
    data.quat_z = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [ins_lon]
    data.ins_lon = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [ins_lat]
    data.ins_lat = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [ins_msl]
    data.ins_msl = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [pdop]
    data.pdop = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [hdop]
    data.hdop = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [solq_pos]
    data.solq_pos = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [nv_pos]
    data.nv_pos = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [solq_heading]
    data.solq_heading = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [nv_heading]
    data.nv_heading = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [diff_age]
    data.diff_age = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [undulation]
    data.undulation = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [reserved_3]
    data.reserved_3 = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [vel_enu_1]
    data.vel_enu_1 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [vel_enu_2]
    data.vel_enu_2 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [vel_enu_3]
    data.vel_enu_3 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_enu_1]
    data.acc_enu_1 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_enu_2]
    data.acc_enu_2 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [acc_enu_3]
    data.acc_enu_3 = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [reserved_4]
    data.reserved_4 = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [reserved_5]
    data.reserved_5 = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [reserved_6]
    data.reserved_6 = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [reserved_7]
    data.reserved_7 = _deserializer.uint16(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 106;
  }

  static datatype() {
    // Returns string type for a message object
    return 'hipnuc_imu/Ins_0x81_package';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4551f87534527c807c5576a40dab6bfe';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
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
    const resolved = new Ins_0x81_package(null);
    if (msg.frame_rate !== undefined) {
      resolved.frame_rate = msg.frame_rate;
    }
    else {
      resolved.frame_rate = 0
    }

    if (msg.tag !== undefined) {
      resolved.tag = msg.tag;
    }
    else {
      resolved.tag = 0
    }

    if (msg.status !== undefined) {
      resolved.status = msg.status;
    }
    else {
      resolved.status = 0
    }

    if (msg.ins_status !== undefined) {
      resolved.ins_status = msg.ins_status;
    }
    else {
      resolved.ins_status = 0
    }

    if (msg.gpst_wn !== undefined) {
      resolved.gpst_wn = msg.gpst_wn;
    }
    else {
      resolved.gpst_wn = 0
    }

    if (msg.gpst_tow !== undefined) {
      resolved.gpst_tow = msg.gpst_tow;
    }
    else {
      resolved.gpst_tow = 0
    }

    if (msg.gyr_b_x !== undefined) {
      resolved.gyr_b_x = msg.gyr_b_x;
    }
    else {
      resolved.gyr_b_x = 0
    }

    if (msg.gyr_b_y !== undefined) {
      resolved.gyr_b_y = msg.gyr_b_y;
    }
    else {
      resolved.gyr_b_y = 0
    }

    if (msg.gyr_b_z !== undefined) {
      resolved.gyr_b_z = msg.gyr_b_z;
    }
    else {
      resolved.gyr_b_z = 0
    }

    if (msg.acc_b_x !== undefined) {
      resolved.acc_b_x = msg.acc_b_x;
    }
    else {
      resolved.acc_b_x = 0
    }

    if (msg.acc_b_y !== undefined) {
      resolved.acc_b_y = msg.acc_b_y;
    }
    else {
      resolved.acc_b_y = 0
    }

    if (msg.acc_b_z !== undefined) {
      resolved.acc_b_z = msg.acc_b_z;
    }
    else {
      resolved.acc_b_z = 0
    }

    if (msg.mag_b_x !== undefined) {
      resolved.mag_b_x = msg.mag_b_x;
    }
    else {
      resolved.mag_b_x = 0
    }

    if (msg.mag_b_y !== undefined) {
      resolved.mag_b_y = msg.mag_b_y;
    }
    else {
      resolved.mag_b_y = 0
    }

    if (msg.mag_b_z !== undefined) {
      resolved.mag_b_z = msg.mag_b_z;
    }
    else {
      resolved.mag_b_z = 0
    }

    if (msg.air_pressure !== undefined) {
      resolved.air_pressure = msg.air_pressure;
    }
    else {
      resolved.air_pressure = 0
    }

    if (msg.reserved_1 !== undefined) {
      resolved.reserved_1 = msg.reserved_1;
    }
    else {
      resolved.reserved_1 = 0
    }

    if (msg.reserved_2 !== undefined) {
      resolved.reserved_2 = msg.reserved_2;
    }
    else {
      resolved.reserved_2 = 0
    }

    if (msg.temperature !== undefined) {
      resolved.temperature = msg.temperature;
    }
    else {
      resolved.temperature = 0
    }

    if (msg.utc_year !== undefined) {
      resolved.utc_year = msg.utc_year;
    }
    else {
      resolved.utc_year = 0
    }

    if (msg.utc_mouth !== undefined) {
      resolved.utc_mouth = msg.utc_mouth;
    }
    else {
      resolved.utc_mouth = 0
    }

    if (msg.utc_day !== undefined) {
      resolved.utc_day = msg.utc_day;
    }
    else {
      resolved.utc_day = 0
    }

    if (msg.utc_hour !== undefined) {
      resolved.utc_hour = msg.utc_hour;
    }
    else {
      resolved.utc_hour = 0
    }

    if (msg.utc_min !== undefined) {
      resolved.utc_min = msg.utc_min;
    }
    else {
      resolved.utc_min = 0
    }

    if (msg.utc_sec !== undefined) {
      resolved.utc_sec = msg.utc_sec;
    }
    else {
      resolved.utc_sec = 0
    }

    if (msg.roll !== undefined) {
      resolved.roll = msg.roll;
    }
    else {
      resolved.roll = 0
    }

    if (msg.pitch !== undefined) {
      resolved.pitch = msg.pitch;
    }
    else {
      resolved.pitch = 0
    }

    if (msg.yaw !== undefined) {
      resolved.yaw = msg.yaw;
    }
    else {
      resolved.yaw = 0
    }

    if (msg.quat_w !== undefined) {
      resolved.quat_w = msg.quat_w;
    }
    else {
      resolved.quat_w = 0
    }

    if (msg.quat_x !== undefined) {
      resolved.quat_x = msg.quat_x;
    }
    else {
      resolved.quat_x = 0
    }

    if (msg.quat_y !== undefined) {
      resolved.quat_y = msg.quat_y;
    }
    else {
      resolved.quat_y = 0
    }

    if (msg.quat_z !== undefined) {
      resolved.quat_z = msg.quat_z;
    }
    else {
      resolved.quat_z = 0
    }

    if (msg.ins_lon !== undefined) {
      resolved.ins_lon = msg.ins_lon;
    }
    else {
      resolved.ins_lon = 0
    }

    if (msg.ins_lat !== undefined) {
      resolved.ins_lat = msg.ins_lat;
    }
    else {
      resolved.ins_lat = 0
    }

    if (msg.ins_msl !== undefined) {
      resolved.ins_msl = msg.ins_msl;
    }
    else {
      resolved.ins_msl = 0
    }

    if (msg.pdop !== undefined) {
      resolved.pdop = msg.pdop;
    }
    else {
      resolved.pdop = 0
    }

    if (msg.hdop !== undefined) {
      resolved.hdop = msg.hdop;
    }
    else {
      resolved.hdop = 0
    }

    if (msg.solq_pos !== undefined) {
      resolved.solq_pos = msg.solq_pos;
    }
    else {
      resolved.solq_pos = 0
    }

    if (msg.nv_pos !== undefined) {
      resolved.nv_pos = msg.nv_pos;
    }
    else {
      resolved.nv_pos = 0
    }

    if (msg.solq_heading !== undefined) {
      resolved.solq_heading = msg.solq_heading;
    }
    else {
      resolved.solq_heading = 0
    }

    if (msg.nv_heading !== undefined) {
      resolved.nv_heading = msg.nv_heading;
    }
    else {
      resolved.nv_heading = 0
    }

    if (msg.diff_age !== undefined) {
      resolved.diff_age = msg.diff_age;
    }
    else {
      resolved.diff_age = 0
    }

    if (msg.undulation !== undefined) {
      resolved.undulation = msg.undulation;
    }
    else {
      resolved.undulation = 0
    }

    if (msg.reserved_3 !== undefined) {
      resolved.reserved_3 = msg.reserved_3;
    }
    else {
      resolved.reserved_3 = 0
    }

    if (msg.vel_enu_1 !== undefined) {
      resolved.vel_enu_1 = msg.vel_enu_1;
    }
    else {
      resolved.vel_enu_1 = 0
    }

    if (msg.vel_enu_2 !== undefined) {
      resolved.vel_enu_2 = msg.vel_enu_2;
    }
    else {
      resolved.vel_enu_2 = 0
    }

    if (msg.vel_enu_3 !== undefined) {
      resolved.vel_enu_3 = msg.vel_enu_3;
    }
    else {
      resolved.vel_enu_3 = 0
    }

    if (msg.acc_enu_1 !== undefined) {
      resolved.acc_enu_1 = msg.acc_enu_1;
    }
    else {
      resolved.acc_enu_1 = 0
    }

    if (msg.acc_enu_2 !== undefined) {
      resolved.acc_enu_2 = msg.acc_enu_2;
    }
    else {
      resolved.acc_enu_2 = 0
    }

    if (msg.acc_enu_3 !== undefined) {
      resolved.acc_enu_3 = msg.acc_enu_3;
    }
    else {
      resolved.acc_enu_3 = 0
    }

    if (msg.reserved_4 !== undefined) {
      resolved.reserved_4 = msg.reserved_4;
    }
    else {
      resolved.reserved_4 = 0
    }

    if (msg.reserved_5 !== undefined) {
      resolved.reserved_5 = msg.reserved_5;
    }
    else {
      resolved.reserved_5 = 0
    }

    if (msg.reserved_6 !== undefined) {
      resolved.reserved_6 = msg.reserved_6;
    }
    else {
      resolved.reserved_6 = 0
    }

    if (msg.reserved_7 !== undefined) {
      resolved.reserved_7 = msg.reserved_7;
    }
    else {
      resolved.reserved_7 = 0
    }

    return resolved;
    }
};

module.exports = Ins_0x81_package;
