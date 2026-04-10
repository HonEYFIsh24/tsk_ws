#!/usr/bin/env python3

import rospy
import numpy as np
import cv2
from sensor_msgs.msg import CompressedImage, PointCloud2
from pypcd4 import PointCloud

def encode(cld: np.ndarray, W: int, H: int, f: float, max_depth: float) -> np.ndarray:
    assert cld.ndim == 2 and cld.shape[1] == 3
    cx = int(W // 2.)
    cy = int(H // 2.)
    us = f * cld[:, 0] / (cld[:, 2] + 1e-6) + cx
    vs = f * cld[:, 1] / (cld[:, 2] + 1e-6) + cy
    pixels = np.stack([us, vs, cld[:, 2]], axis=1)

    img = np.zeros((H, W), dtype=np.uint16)
    valid = (us >= 0) & (us < W) & (vs >= 0) & (vs < H)

    u_int = pixels[valid, 0].astype(np.int32)
    v_int = pixels[valid, 1].astype(np.int32)
    depths = pixels[valid, 2]
    depths = np.clip(depths, 0., max_depth)
    img[v_int, u_int] = (depths * 1000).astype(np.uint16)

    return cv2.imencode('.png', img)[1]

def pointcloud_callback(msg: PointCloud2):
    # 将 ROS PointCloud2 消息转换为 numpy 数组
    pc = PointCloud.from_msg(msg)
    cld = pc[("x", "y", "z")].numpy()

    # 编码
    encoded_img = encode(cld, W, H, f, max_depth)

    # 发布压缩图像
    compressed_msg = CompressedImage()
    compressed_msg.header = msg.header
    compressed_msg.format = "png"
    compressed_msg.data = encoded_img.tobytes()
    image_pub.publish(compressed_msg)

if __name__ == "__main__":
    rospy.init_node('encode_node')
    
    # 读取参数, 订阅 topic, 图像尺寸等
    pointcloud_topic = rospy.get_param('~pointcloud_topic', '/camera/depth/points')
    image_topic = f"{pointcloud_topic}/compressed"
    W = rospy.get_param('~image_width', 240)
    H = rospy.get_param('~image_height', 180)
    f = rospy.get_param('~focal_length', 107.0)
    max_depth = rospy.get_param('~max_depth', 6.0)

    # 发布器
    image_pub = rospy.Publisher(image_topic, CompressedImage, queue_size=1)

    # 订阅器
    rospy.Subscriber(pointcloud_topic, PointCloud2, pointcloud_callback, queue_size=1)

    rospy.loginfo(f"Subscribed to {pointcloud_topic}, publishing to {image_topic}")
    rospy.spin()


