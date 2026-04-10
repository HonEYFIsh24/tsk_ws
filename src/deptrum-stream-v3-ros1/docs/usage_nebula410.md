# Nebula410

## 目录

- [Deptrum ROS SDK](#Deptrum 深度相机ROS驱动程序)
  - [相机启动流程](#相机启动流程)
  - [主要的功能介绍](#主要的功能介绍)
  - [Launch 参数](#launch 参数)
  - [相机控制功能说明](#相机控制功能说明)
  - [相机可用主题](#相机可用主题)

## 相机启动流程

终端1 - 启动驱动节点

```bash
source ./devel/setup.bash
roslaunch deptrum_camera nebula_410.launch
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

获取 SDK 版本信息（包含固件和 Deptrum SDK 版本）：

```bash
rosservice call /camera/get_sdk_version "{}"
```

设置（自动）曝光：

```bash
rosservice call /camera/set_depth_auto_exposure '{data: false}'

#只有当自动曝光为false时才生效
rosservice call /camera/set_depth_exposure "{data: 800, data_two: 3}"
data:
1.mtof 1-2400微秒
2.stof 1-800微秒

data_wto:
1.@brief Stof depth exposure
kDtOptionStofDepthExposureInt = 3,

2.@brief Mtof depth exposure
 kDtOptionMtofDepthExposureInt = 4

```

设置（启用）图像裁剪 ROI：

```bash
rosservice call /camera/set_depth_crop_roi_mtof_enable "{data: true}"

rosservice call /camera/set_depth_crop_roi_stof_enable "{data: true}"

/**
   * @brief STOF depth cropping configuration (Y-direction only)
   * @format uint8_t[4] = {x, y, width, height}
   * @details
   *   - Only cropping in Y direction is supported.
   *   - x must be 0.
   *   - width is fixed to 248.
   *   - y represents crop_up.
   *   - height = 180 - crop_up - crop_down.
   * @constraint crop_up + crop_down < 170
   * @constraint crop_up + crop_down must be even
   * @note For Nebula280 series, the ROI for MTOF and STOF must be set to the same values; cropping
   * STOF or MTOF individually is not supported.
   */
  kDtOptionStofDepthCropRoiStruct = 90,

  /**
   * @brief MTOF depth cropping configuration (Y-direction only)
   * @format uint8_t[4] = {x, y, width, height}
   * @details
   *   - Only cropping in Y direction is supported.
   *   - x must be 0.
   *   - width is fixed to 248.
   *   - y represents crop_up.
   *   - height = 180 - crop_up - crop_down.
   * @constraint crop_up + crop_down < 170
   * @constraint crop_up + crop_down must be even
   * @note For Nebula280 series, the ROI for MTOF and STOF must be set to the same values; cropping
   * STOF or MTOF individually is not supported.
   */
  kDtOptionMtofDepthCropRoiStruct = 91,

#Set depth CROP
#简单说就是参数1:点的x坐标， 参数2:点的y坐标  ，参数3:以(x,y)为坐标点为起点的宽度 参数4:以(x,y)为坐标点为起点的高度。 参数4:kDtOptionStofDepthCropRoiStruct或者kDtOptionMtofDepthCropRoiStruct
rosservice call /camera/set_depth_crop_roi '{data_param:[0,50,248,30,90]}'
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

## Launch 参数

以下是可用的launch启动参数：

- `camera_mode`: 仅适用于nebula 410, 
  0: mode8, 单mToF AE模式（默认）,低USB带宽和计算负载,全范围的稀疏点云,不支持结构光或HDR
  1：mode14, 结构光+ mToF融合模式,高计算负荷,支持结构光和ToF融合,近距离深度密集，中远距离深度稀疏，暂不支持HDR。
  2：mode14, mToF HDR模式 适度的计算负荷，无结构光重建，全范围的稀疏点云,支持mof 暂不支持HDR

- `device_mode`: 从设备同步状态（只读）。0：主模式，1：从模式   

- `enable_point_cloud`: 深度点云开关。

- `enable_depth`: 启用深度流。

- `depth_crop_roi_enable`:启用图像剪裁ROI。

- `depth_crop_roi_[mtof|stof]_[enableleft|top|right|bottom]`:设置深度图像剪裁ROI与类型。启用图像剪裁ROI为true时才起作用，目前只支持Y方向的裁剪。X和宽度必须是固定的.设置为：{0，crop_up, 248,180 - crop_up - crop_down}

  类型为
  kDtOptionStofDepthCropRoiStruct =90

  kDtOptionMtofDepthCropRoiStruct =91。

  ​	

  ```
  示例
  	<arg name="depth_crop_roi_mtof_enable" default="true"/>
      <arg name="depth_crop_roi_mtof_left" default="0"/>
      <arg name="depth_crop_roi_mtof_top" default="50"/>
      <arg name="depth_crop_roi_mtof_width" default="248"/>
      <arg name="depth_crop_roi_mtof_height" default="30"/>
  
      <arg name="depth_crop_roi_stof_enable" default="true"/>
      <arg name="depth_crop_roi_stof_left" default="0"/>
      <arg name="depth_crop_roi_stof_top" default="50"/>
      <arg name="depth_crop_roi_stof_width" default="248"/>
      <arg name="depth_crop_roi_stof_height" default="30"/>
  ```

  

- `enable_depth_auto_exposure`:启用深度自动曝光

- `depth_exposure_mode`:设置当前深度曝光模式

- `depth_exposure_[mtof|stof]_value`:设置当前深度曝光模式，自动曝光为false时才生效。

  类型值Stof depth exposure=3, Mtof depth exposure=4。

- `log_level` : SDK 日志级别，默认值为 `info`，可选值为 `debug`, `info`, `warn`, `error`, `fatal`。保存在 `~/.ros/Log` 中。


**重要提示**：*请仔细阅读其中关于软件配置的说明。如果您不确定，请勿修改这些设置。*

## 相机控制功能说明

设置或获取参数相关的服务（分别表示为 `set_`和 `get_`）只有在激活相应的 `enable_`参数后才可用。例如，使用 `enable_left_ir` 启用左红外 (IR)、使用 `enable_right_ir` 启用右红外、使用 `enable_depth` 启用深度感知或使用 `enable_color` 启用色彩处理等功能（请参阅[Launch 参数](#Launch 参数)）是相应服务正常运行的先决条件。此配置确保只有在启动文件的流参数中启用其特定流时，服务才可用。

- `/camera/get_camera_params`
- `/camera/get_depth_camera_info`
- `/camera/get_device_info`
- `/camera/get_serial`
- `/camera/get_sdk_version`
- `/camera/reset_depth_exposure`(暂不支持)
- `/camera/save_images`
- `/camera/save_point_cloud`
- `/camera/set_depth_auto_exposure`
- `/camera/set_depth_exposure`
- `/camera/set_depth_crop_roi_enable`
- `/camera/set_depth_crop_roi`



## 相机可用主题

- `/camera/depth/camera_info`: 深度相机信息。
- `/camera/depth/image_raw`: 深度流图像。
- `/camera/depth/points` :点云，仅当 `enable_point_cloud`为 `true`时可用。
