#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <opencv2/opencv.hpp>
#include <camera_info_manager/camera_info_manager.h>
#include <filesystem>
#include <regex>
#include <iostream>
#include <cstdlib>
#include <array>
#include <memory>

std::string findDicotaCamera() {
    namespace fs = std::filesystem;
    std::regex videoRegex("^video([0-9]+)$");
    int minIndex = 9999;
    std::string selectedDevice;

    for (const auto& entry : fs::directory_iterator("/dev")) {
        std::string filename = entry.path().filename();
        std::smatch match;
        if (std::regex_match(filename, match, videoRegex)) {
            std::string devicePath = "/dev/" + filename;

            // 调用 v4l2-ctl 读取设备名
            std::string cmd = "v4l2-ctl --device=" + devicePath + " --info 2>/dev/null";
            std::array<char, 128> buffer;
            std::string result;
            std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
            if (!pipe) continue;
            while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                result += buffer.data();
            }

            if (result.find("W19 HD Webcam") != std::string::npos) {
                int index = std::stoi(match[1].str());
                if (index < minIndex) {
                    minIndex = index;
                    selectedDevice = devicePath;
                }
            }
            else if(result.find("DICOTA 4K")!= std::string::npos){
                int index = std::stoi(match[1].str());
                if (index < minIndex) {
                    minIndex = index;
                    selectedDevice = devicePath;
                }
            }
        }
    }

    return selectedDevice;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "cam_node");
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(nh);

    std::string dicotaCam = findDicotaCamera();
    if (dicotaCam.empty()) {
        ROS_ERROR("No W19 HD Webcam or DICOTA 4K found");
        return -1;
    }

    // 参数
    int cam_id, width, height, fps, angle;
    std::string camera_name, camera_info_url;
    nh.param("cam_id", cam_id, 0);
    nh.param("width", width, 640);
    nh.param("height", height, 480);
    nh.param("fps", fps, 10);
    nh.param("angle", angle, 90);
    nh.param("camera_name", camera_name, std::string("my_rgb_cam"));
    nh.param("camera_info_url", camera_info_url, std::string(""));

    // 相机
    cv::VideoCapture cap(dicotaCam);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));

    if (!cap.isOpened()) {
        ROS_ERROR("Failed to open camera %d", cam_id);
        return -1;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    // CameraInfo 管理器
    camera_info_manager::CameraInfoManager cinfo(nh, camera_name, camera_info_url);

    // 发布器
    image_transport::CameraPublisher pub_raw = it.advertiseCamera("/my_rgb_cam/image_raw", 1);
    image_transport::CameraPublisher pub_rot = it.advertiseCamera("/my_rgb_cam/image_rotated", 1);

    ros::Rate loop_rate(fps);

    while (ros::ok()) {
        cv::Mat frame;
        cap >> frame;


        if (frame.empty()) {
            ROS_WARN_THROTTLE(5, "Empty frame captured, skipping...");
            // 不重启 VideoCapture，直接跳过，等待下一帧
            ros::spinOnce();
            loop_rate.sleep();
            continue;
        }



        ros::Time stamp = ros::Time::now();

        // CameraInfo
        sensor_msgs::CameraInfo cam_info = cinfo.getCameraInfo();
        cam_info.header.stamp = stamp;
        cam_info.header.frame_id = camera_name;

        // 原始图像
        cv_bridge::CvImage cv_img(cam_info.header, "bgr8", frame);
        pub_raw.publish(*cv_img.toImageMsg(), cam_info);

        // 旋转图像
        cv::Mat rotated;
        if (angle == 90) {
            cv::rotate(frame, rotated, cv::ROTATE_90_CLOCKWISE);
        } else if (angle == -90) {
            cv::rotate(frame, rotated, cv::ROTATE_90_COUNTERCLOCKWISE);
        } else if (angle == 180) {
            cv::rotate(frame, rotated, cv::ROTATE_180);
        } else {
            rotated = frame.clone();
        }

        cv_bridge::CvImage rot_img(cam_info.header, "bgr8", rotated);
        pub_rot.publish(*rot_img.toImageMsg(), cam_info);

        ros::spinOnce();
        loop_rate.sleep();
    }

    cap.release();
    return 0;
}
