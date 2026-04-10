# Building the ROS Driver

## Pre-requisites

Before trying to build the ROS Driver, you will need to install required dependencies:

- ROS Melodic (Ubuntu18.04) 
- ROS Kinetic (Ubuntu16.04)
- ROS Noetic  (Ubuntu20.04)

### ROS

Follow the [installation instructions](https://wiki.ros.org/Installation) for your operating system of choice: [Ubuntu](https://wiki.ros.org/Installation/Ubuntu), or [Windows](https://wiki.ros.org/Installation/Windows) to install ROS. Verify that ROS is working and that you can build sample projects in your catkin workspace before trying to build the ROS Driver.

## Compiling

Once the Stream SDK has been installed, the ROS node can be built using `catkin_make`. Please note that you may need to run `catkin_make --force-cmake` to update the SDK binaries which are copied into the ROS output folders.

## Binary packing

- `sudo apt-get install python-bloom`: Install the bloom if it has not been installed before.
- `cd deptrum-ros-driver`: Change directory to deptrum-ros-driver
- `bloom-generate rosdebian --os-name ubuntu --ros-distro $ROS_DISTRO` : Generate debain directory.
- `fakeroot debian/rules binary` : Generate binary debian package.
