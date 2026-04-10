#!/usr/bin/env python3

import rospy
import numpy as np
import cv2
from sensor_msgs.msg import CompressedImage, PointCloud2
from sensor_msgs import point_cloud2

def decode(png_bytes: bytes, W: int, H: int, f: float) -> np.ndarray:
    img = cv2.imdecode(np.frombuffer(png_bytes, np.uint8), cv2.IMREAD_UNCHANGED)
    assert img.shape == (H, W)

    cx = float(W // 2.)
    cy = float(H // 2.)

    v_coords, u_coords = np.indices((H, W))
    z = img.astype(np.float32) / 1000.0
    mask = z > 0

    u_valid = u_coords[mask]
    v_valid = v_coords[mask]
    z_valid = z[mask]

    x = (u_valid - cx) * z_valid / f
    y = (v_valid - cy) * z_valid / f

    return np.stack((x, y, z_valid), axis=1).astype(np.float32)

def image_callback(msg: CompressedImage):
    png_bytes = msg.data
    points = decode(png_bytes, W, H, f)

    header = msg.header
    pc2_msg = point_cloud2.create_cloud_xyz32(header, points.tolist())
    pointcloud_pub.publish(pc2_msg)
    

if __name__ == "__main__":
    rospy.init_node('encode_node')
    
    # 读取参数, 订阅 topic, 图像尺寸等
    image_topic = rospy.get_param('~image_topic', '/camera/depth/points/compressed')
    pointcloud_topic = rospy.get_param('~pointcloud_topic', '/camera/depth/points')
    W = rospy.get_param('~image_width', 240)
    H = rospy.get_param('~image_height', 180)
    f = rospy.get_param('~focal_length', 107.0)

    # 发布器和订阅器
    pointcloud_pub = rospy.Publisher(pointcloud_topic, PointCloud2, queue_size=1)
    image_sub = rospy.Subscriber(image_topic, CompressedImage, image_callback, queue_size=1)

    rospy.loginfo(f"Subscribed to {image_topic}, publishing to {pointcloud_topic}")
    rospy.spin()