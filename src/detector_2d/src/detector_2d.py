#! /usr/bin/env python3

import os
import time

import rospy
import cv2
from std_msgs.msg import Header
from sensor_msgs.msg import Image
from sensor_msgs.msg import CompressedImage
import time
import numpy as np
import torch
from cv_bridge import CvBridge, CvBridgeError

from nanodet.data.batch_process import stack_batch_img
from nanodet.data.collate import naive_collate
from nanodet.data.transform import Pipeline
from nanodet.model.arch import build_model
from nanodet.util import Logger, cfg, load_config, load_model_weight
from nanodet.util.path import mkdir

class Predictor(object):
    def __init__(self, cfg, model_path, logger, device="cuda:0"):
        self.cfg = cfg
        self.device = device
        model = build_model(cfg.model)
        ckpt = torch.load(model_path, map_location=lambda storage, loc: storage)
        load_model_weight(model, ckpt, logger)
        if cfg.model.arch.backbone.name == "RepVGG":
            deploy_config = cfg.model
            deploy_config.arch.backbone.update({"deploy": True})
            deploy_model = build_model(deploy_config)
            from nanodet.model.backbone.repvgg import repvgg_det_model_convert

            model = repvgg_det_model_convert(model, deploy_model)
        self.model = model.to(device).eval()
        self.pipeline = Pipeline(cfg.data.val.pipeline, cfg.data.val.keep_ratio)

    def inference(self, img):
        img_info = {"id": 0}
        if isinstance(img, str):
            img_info["file_name"] = os.path.basename(img)
            img = cv2.imread(img)
        else:
            img_info["file_name"] = None

        height, width = img.shape[:2]
        img_info["height"] = height
        img_info["width"] = width
        meta = dict(img_info=img_info, raw_img=img, img=img)
        meta = self.pipeline(None, meta, self.cfg.data.val.input_size)
        meta["img"] = torch.from_numpy(meta["img"].transpose(2, 0, 1)).to(self.device)
        meta = naive_collate([meta])
        meta["img"] = stack_batch_img(meta["img"], divisible=32)
        
        with torch.no_grad():
            results = self.model.inference(meta)
        return meta, results

    def visualize(self, dets, meta, class_names, score_thres, wait=0):
        time1 = time.time()
        result_img = self.model.head.show_result(
            meta["raw_img"][0], dets, class_names, score_thres=score_thres, show=True
        )
        # print("viz time: {:.3f}s".format(time.time() - time1))
        return result_img

def computeIoU(bbox1,bbox2):
    x_overlap = max(0,min(bbox1[2],bbox2[2])-max(bbox1[0],bbox2[0]))
    y_overlap = max(0,min(bbox1[3],bbox2[3])-max(bbox1[1],bbox2[1]))
    intersection = x_overlap*y_overlap
    # print(intersection)
    union = (bbox2[2]-bbox2[0])*(bbox2[3]-bbox2[1])+(bbox1[2]-bbox1[0])*(bbox1[3]-bbox1[1])-intersection
    # print(union)
    IoU = intersection/union
    return IoU

local_rank = 0
torch.backends.cudnn.enabled = True
torch.backends.cudnn.benchmark = True
load_config(cfg, '/home/fxxk/ObjectDetection/nanodet/config/nanodet-plus-m_416.yml')
logger = Logger(local_rank, use_tensorboard=False)
predictor = Predictor(cfg, '/home/fxxk/ObjectDetection/nanodet/checkpoint/nanodet-plus-m_416_checkpoint.ckpt', logger, device="cpu")

def camera_callback(msg):
    # print('camera_callback')
    bridge = CvBridge()
    try:
        # 将ROS Image消息转换为OpenCV图像
        cv_image = bridge.imgmsg_to_cv2(msg, "bgr8")
        # cv_image = bridge.compressed_imgmsg_to_cv2(msg, "bgr8")
    except CvBridgeError as e:
        print(e)
    else:
        raw_image = cv_image.copy()
        cv2.imshow('Raw',raw_image)
        current_time1 = time.localtime()
        meta, res = predictor.inference(cv_image)
        current_time2 = time.localtime()
        
        
        result_image = predictor.visualize(res[0], meta, cfg.class_names, 0.5)
        all_box = []
        for bbox in res[0][0]:
            score = bbox[-1]
            x0, y0, x1, y1 = [int(i) for i in bbox[:4]]
            all_box.append([x0, y0, x1, y1, score])

        '''
        imgWidth = (cv_image.shape)[1]
        imgHeight = (cv_image.shape)[0]
        failed = []
        for line in lines:
            _,gtCenterXRatio,gtCenterYRatio,gtCenterWidthRatio,gtCenterHeightRatio = line.split()
            gtCenterX = imgWidth*float(gtCenterXRatio)
            gtCenterY = imgHeight*float(gtCenterYRatio)
            gtWidth = imgWidth*float(gtCenterWidthRatio)
            gtHeight = imgHeight*float(gtCenterHeightRatio)
            gtMinX = int(gtCenterX-gtWidth/2)
            gtMinY = int(gtCenterY-gtHeight/2)
            gtMaxX = int(gtCenterX+gtWidth/2)
            gtMaxY = int(gtCenterY+gtHeight/2)
            print('Person:\t', gtMinX,'\t',gtMinY,'\t',gtMaxX,'\t',gtMaxY)
            cv2.rectangle(result_image, (gtMinX, gtMinY), (gtMaxX, gtMaxY), (0, 0, 255), 2)
            found = False
            for bbox in all_box:
                IoU = computeIoU()
                x_overlap = max(0,min(gtMaxX,bbox[2])-max(gtMinX,bbox[0]))
                y_overlap = max(0,min(gtMaxY,bbox[3])-max(gtMinY,bbox[1]))
                intersection = x_overlap*y_overlap
                # print(intersection)
                union = (bbox[2]-bbox[0])*(bbox[3]-bbox[1])+(gtMaxX-gtMinX)*(gtMaxY-gtMinY)-intersection
                # print(union)
                IoU = intersection/union
                # print(IoU)
                if IoU>0.5:
                    print('Match!')
                    print('IoU: ',IoU)
                    found = True
                    break
            if found==False:
                failed.append(line)
            print('--------------------')
        '''
        '''
        if len(failed) != 0:
            cv_image_rotate = cv2.rotate(raw_image,cv2.ROTATE_90_CLOCKWISE)
            cv2.imshow('Image_Rotated',cv_image_rotate)
            meta, res = predictor.inference(cv_image_rotate)
            all_box = []
            for bbox in res[0][0]:
                score = bbox[-1]
                if score>0.3:
                    x0, y0, x1, y1 = [int(i) for i in bbox[:4]]
                    all_box.append([x0, y0, x1, y1, score])
            print(all_box)
            result_image = predictor.visualize(res[0], meta, cfg.class_names, 0.3)
            result_image_rotate = cv2.rotate(result_image,cv2.ROTATE_90_COUNTERCLOCKWISE)
            cv2.imshow('Results_Rotated',result_image_rotate)
            for fail in failed:
                _,gtCenterXRatio,gtCenterYRatio,gtCenterWidthRatio,gtCenterHeightRatio = fail.split()
                gtCenterX = imgWidth*float(gtCenterXRatio)
                gtCenterY = imgHeight*float(gtCenterYRatio)
                gtWidth = imgWidth*float(gtCenterWidthRatio)
                gtHeight = imgHeight*float(gtCenterHeightRatio)
                gtMinX = int(gtCenterX-gtWidth/2)
                gtMinY = int(gtCenterY-gtHeight/2)
                gtMaxX = int(gtCenterX+gtWidth/2)
                gtMaxY = int(gtCenterY+gtHeight/2)
                print('Person:\t', gtMinX,'\t',gtMinY,'\t',gtMaxX,'\t',gtMaxY)
                cv2.rectangle(result_image_rotate, (gtMinX, gtMinY), (gtMaxX, gtMaxY), (0, 0, 255), 3)
            result_image = result_image_rotate
        '''
        cv2.imshow('Results',result_image)
        cv2.waitKey(1)
        # print('***********************************')
        '''
        if cv2.waitKey(1)==32:   
            print('Saved image '+str(msg.header.seq))   
            save_path = '/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(msg.header.seq)+'_gt.jpg'
            cv2.imwrite(save_path,raw_image)
            save_path = '/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(msg.header.seq)+'_det.jpg'
            cv2.imwrite(save_path,result_image)    
            save_path = '/home/bowen/PythonProject/nanodet/catkin_ws/src/detector_2d/data/'+str(msg.header.seq)+'_det.txt'
            with open(save_path, 'w') as file:
                for bbox in all_box:
                    file.write(str(bbox[0])+'\t'+str(bbox[1])+'\t'+str(bbox[2])+'\t'+str(bbox[3])+'\n')
        '''
    pub = rospy.Publisher('image_topic',Image,queue_size=10)
    # 将OpenCV图像转换为ROS图像消息
    ros_image = bridge.cv2_to_imgmsg(result_image, encoding="bgr8")
    # 发布图像消息
    pub.publish(ros_image)
    rospy.loginfo("Image published")
    
def main():

    rospy.init_node('detector_2d', anonymous=True)

    # subscriber
    sub_image = rospy.Subscriber("/my_rgb_cam/image_rotated", Image, camera_callback)
    # sub_image = rospy.Subscriber("/usb_cam/image_raw/compressed", CompressedImage, camera_callback)

    # publisher
    # pub_velo = rospy.Publisher("/lidar_det3D", PointCloud2, queue_size=1)
    # pub_image = rospy.Publisher("/image_det3D", Image, queue_size=1)
    # pub_bbox = rospy.Publisher("/detections", BoundingBoxArray, queue_size=10)
    
    rospy.spin()

if __name__ == '__main__':
    main()
