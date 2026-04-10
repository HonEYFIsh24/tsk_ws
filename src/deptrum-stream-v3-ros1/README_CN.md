# Deptrum 深度相机 ROS 驱动程序

Deptrum 深度相机ROS驱动程序为 Deptrum 相机与 ROS 环境之间提供了无缝集成，支持 ROS1 Kinetic、 Melodic、 Noetic 和 ROS2 Foxy、Galactic、Humble 发行版。

设备支持列表：

<table border="1" style="border-collapse: collapse; text-align: left; width: 100%;">
  <thead>
    <tr style="background-color: #1f4e78; color: white; text-align: center;">
      <th>产品系列</th>
      <th>产品</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td rowspan="8" style="text-align: center; font-weight: bold;">Nebula系列</td>
      <td>Nebula410</td>
    </tr>
  </tbody>
</table>

注意：若您的设备型号未在上述列表中，请联系技术支持团队或销售代表获取专业协助。

## 目录

- [Deptrum ROS SDK](#Deptrum 深度相机ROS驱动程序)
  - [环境配置](#环境配置)
  - [创建 ROS 工作区并构建](#创建 ROS 工作区并构建)
  - [相机启动流程](#相机启动流程)
  - [主要的功能介绍](#主要的功能介绍)
  - [Launch 参数](#launch 参数)
  - [多相机](#多相机)
  - [相机控制功能说明](#相机控制功能说明)
  - [相机可用主题](#相机可用主题)

## 环境配置

ROS 系统安装

* 请参考ROS官方[安装指南](http://wiki.ros.org/ROS/Installation)完成系统配置。

依赖包安装

```bash
sudo apt install libgflags-dev ros-$ROS_DISTRO-image-geometry ros-$ROS_DISTRO-camera-info-manager \
ros-${ROS_DISTRO}-image-transport-plugins ros-${ROS_DISTRO}-compressed-image-transport \
ros-$ROS_DISTRO-image-transport ros-$ROS_DISTRO-image-publisher libgoogle-glog-dev libusb-1.0-0-dev libeigen3-dev \
ros-$ROS_DISTRO-diagnostic-updater ros-$ROS_DISTRO-diagnostic-msgs \
libdw-dev
```

如果系统在设备枚举时出现问题，需要配置udev权限（已配置过的系统可跳过此步骤）。

```bash
scripts/
├── 99-deptrum-libusb.rules
└── install_udev_rules.sh
```

执行权限配置命令：

```bash
sh ./install_udev_rules.sh
```

驱动包安装

```
# 请将deptrum_camera_xxxx.deb 替换为实际的包名
sudo dpkg -i deptrum_cameraxxxx.deb
```

## 创建 ROS 工作区并构建

**<u>这是源码编译，安装deb包不需要</u>**

创建 ROS 工作区（如果您没有）：

```bash
mkdir -p ~/ros_ws/src
```

获取源代码：

```bash
源码放置src下
```

构建包：

```bash
cd ~/ros_ws
catkin_make
```

安装 udev 规则：

```bash
#如果非root权限用户启动Ros程序时不能枚举到设备，而root权限能枚举到设备时，则需要安装udev,之前安装过的不必重复安装。
同环境配置
```

## 相机启动流程

终端1 - 启动驱动节点

```bash
source ./devel/setup.bash
roslaunch deptrum_camera nebula_410.launch #替换成对应模组的launch文件
```

终端2 - 启动可视化工具

```bash
source ./devel/setup.bash
rviz
```

## 主要的功能介绍

查看系统资源 (第3个终端)

```bash
rostopic list
rosservice list
rosparam list
```

获取相机参数（需先启动数据流）：

```bash
rosservice call /camera/get_camera_params "{}"
```

查看相机参数：

```bash
rostopic echo /camera/depth/camera_info
```

获取设备信息：

```bash
rosservice call /camera/get_device_info "{}"
```

获取 SDK 版本信息：

```bash
rosservice call /camera/get_sdk_version "{}"
```

保存图像：

```bash
rosservice call /camera/save_images "{}"
```

保存点云：

```bash
rosservice call /camera/save_point_cloud "{}"
```

注意：图像保存在 `~/.ros/image`下，并且仅在摄像头数据流打开时可用。

**Nebula410模组查看**

## Launch 参数

以下是可用的launch启动参数：

- `log_level` : SDK 日志级别，默认值为 `info`，可选值为 `debug`, `info`, `warn`, `error`, `fatal`。保存在 `~/.ros/Log` 中。
- **Nebula410模组Launch参数说明查看文档：[usage_nebula410](./docs/usage_nebula410.md)**

**重要提示**：*请仔细阅读其中关于软件配置的说明。如果您不确定，请勿修改这些设置。*

## 多相机

#### 设置多个相机启动

为每个相机单独创建或者复制一个 launch文件，如 nebula_410_copy.launch

```
修改启动参数，比如
<arg name="camera_name" value="camera_01"/>
<arg name="usb_port" value="2-3.4.4.4.1"/>
<arg name="device_num" value="2"/>
```

#### 启动 launch文件

执行 launch配置：

```
roslaunch deptrum_camera nebula_410_copy.launch
```

## 相机控制功能说明

设置或获取参数相关的服务（分别表示为 `set_`和 `get_`）只有在激活相应的 `enable_`参数后才可用。例如，使用 `enable_left_ir` 启用左红外 (IR)、使用 `enable_right_ir` 启用右红外、使用 `enable_depth` 启用深度感知或使用 `enable_color` 启用色彩处理等功能（请参阅[Launch 参数](#Launch 参数)）是相应服务正常运行的先决条件。此配置确保只有在启动文件的流参数中启用其特定流时，服务才可用。

- `/camera/get_camera_params`
- `/camera/get_depth_camera_info`
- `/camera/get_device_info`
- `/camera/get_serial`
- `/camera/get_sdk_version`
- `/camera/save_images`
- `/camera/save_point_cloud`

**Nebula410模组Launch参数说明查看文档：[usage_nebula410](./docs/usage_nebula410.md)**

## 相机可用主题

- `/camera/depth/camera_info`: 深度相机信息。
- `/camera/depth/image_raw`: 深度流图像。
- `/camera/depth/points` :点云，仅当 `enable_point_cloud`为 `true`时可用。

**Nebula410模组Launch参数说明查看文档：[usage_nebula410](./docs/usage_nebula410.md)**