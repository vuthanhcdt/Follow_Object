# Fllow Object

![image](https://github.com/vuthanhcdt/Follow_Object/blob/main/Image/t81942.jpg)


## Demo Video ##
**Robot follow object video**<br/> 
[![image](http://img.youtu.be/j25A7QEM0ak/0.jpg)](https://youtu.be/j25A7QEM0ak)<br/>


## source code download and build code please follow steps ##
1. create a new workspace on your local machine first<br/>
```
$ source /opt/ros/kinetic/setup.bash
$ mkdir -p ~/follow_object/src
```
(Reference: http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment)

2. git clone source code to your workspace<br/>
```
$ cd ~/follow_object/src
$ git clone https://github.com/andyangtw/pcar.git
```
3. build code<br/>
```
$ cd ~/follow_object/
$ catkin_make
```
If you finish building code, you need to source your worksapce.
please add **source ~/follow_object/devel/setup.bash** in your .bashrc.<br/>
and use the commands below to copy the rule for pcar usb devices.<br/>




run below command to send navigation goal

```
$ rosrun loco waypoint_nav.py (send navigation goals and ctrl+c key can stop navigation)
```


## Simulate pcar navigation ##
```
$ roscore
  open new terminal
$ roslaunch rbx1_nav pcar_sim.launch
```
