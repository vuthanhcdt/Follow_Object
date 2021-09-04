# Follow Object

![image](https://github.com/vuthanhcdt/Follow_Object/blob/main/Image/t81942.jpg)


## Demo Video ##
**Robot follow object video**<br/> 
[![image](https://img.youtube.com/vi/j25A7QEM0ak/0.jpg)](https://youtu.be/j25A7QEM0ak)<br/>


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
$ git clone https://github.com/vuthanhcdt/Follow_Object.git
```
3. build code<br/>
```
$ cd ~/follow_object/
$ catkin_make
```
run below command to follow object

```
$ roslaunch follow_object follow_object.launch
```
