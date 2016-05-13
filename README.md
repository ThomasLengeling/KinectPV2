KinectPV2
==========

### Kinect v2 library for Processing

---
Version 0.7.8

Library is curretly on develop with the Windows SDK Version 1409 (9/16/2014)

Library for Mac check out  the [OpenKinect-for-Processing library](https://github.com/shiffman/OpenKinect-for-Processing).

---

####Requirements

- A Kinect for Windows v2 Device (K4W2).
- [Kinect SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
- 64bit computer with a dedicated USB 3.0.
- Windows 10, 8, 8.1
- [Processing 3.0](http://processing.org/)
- Update your latest video card driver.
- Install DirectX 11.

#### Reference and tutorial

[Webpage](http://codigogenerativo.com/code/kinectpv2-k4w2-processing-library/) tutorial with a couple of useful examples. 


### Install for Processing 3.0

- Install the [Kinect for Windows SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
    - Install the library using Processing Contributed Library Manager
    - Manual install, download the latest KinectPV2 version from the releases tab, and copy the KinectPV2 folder into your processing libraries sketch folder. 
- Enjoy

For Processing 2.2.1 please use the [KinectPV2 0.7.2 version](https://github.com/ThomasLengeling/KinectPV2/releases/tag/0.7.2).

#### Examples

- TestImages, Test all Frames/Images for the Kinect.
- SkeletonMaskDepth, Skeleton positions are mapped to match the depth and body index frames.
- SkeletonColor, Skeleton is mapped to match the color frame.
- Skeleton3d, 3d Skeleton example needs love.
- SimpleFaceTracking, simple face tracking with mode detection.
- PointCloudOGL, Point cloud depth render using openGL and shaders.
- PointCloudDepth, point cloud in a single 2d Image and threshold example.
- PointCloudColor, Point cloud in color, using openGL and shaders.
- MaskTest, Body Index test, and body index with depth.
- Mask_findUsers, find number of users base on body index information.
- MapDepthToColor, depth to color mapping, depth frame is aligned with color frame.
- HDFaceVertex, Face vertices are match with the color frame.
- HDColor, 1920 x 1080 RGB frame.
- DepthTest, Depth test with raw depth data.
- CoordinateMapperRGBDepth, example broken, check 0.7.2 version.
- RecordPointCloud, simple OBJ recording of the point cloud positions.
- [OpenCV](https://github.com/atduskgreg/opencv-processing) examples:
  - Live Capture App
  - Find Contours with depth or bodyIndex

#### Build

To build the KinectPV2 library from source code, look at the Build_libs folder
- KinectPV2_vc2012, builds the .dll library with JNI code.
- KinectPV2_Eclipse, builds the .jar library for processing.


#### Know issues

- Missing "msvcp110.dll", or  "Kinect20.Face.dll: Can't find dependent libraries on thread"
    - Install [Visual 2012 C++ Redistributable Packages](https://www.microsoft.com/en-us/download/details.aspx?id=30679).
    - Check if the Kinect v2 SDK 2.0 is installed correctly.
- Problems with the video, update your video card driver and install DirectX 11.


---

#### Todo

- Heart rate detection
- Multiple Devices
- [Kinect Fusion](http://msdn.microsoft.com/en-us/library/dn188670.aspx)

---

### Simple Device

To include the library into a processing sketch you just need to import it and initialize it.

```java
import KinectPV2.*;
KinectPV2 kinect;

void setup() {
  kinect = new KinectPV2(this);
  //Start up methods go here
  kinect.init();
}
```

#### Images

To obtain the color Image, depth Image, infrared Image, bodyIndex Image and long Exposure Image as a PImage you need to active with the following method

```java
 void enableColorImg(boolean toggle);
 void enableDepthImg(boolean toggle);
 void enableInfraredImg(boolean toggle);
 void enableBodyTrackImg(boolean toggle);
 void enableInfraredLongExposureImg(boolean toggle);
 
 PImage getColorImage();
 PImage getDepthImage();
 PImage getInfraredImage();
 PImage getBodyTrackImage();
 PImage getInfraredLongExposureImage();
```
just initialize in the setup()

```java
  kinect = new KinectPV2(this);
  kinect.enableColorImg(true);
  kinect.init();
```

get the PImage in the draw()

```java
PImage imgC = kinect.getColorImage();
image(imgC, 0, 0);
```

Raw Data is only available for the color, depth ad bodytrack frames/images. Once the frames are activated just call them.

```java
  //raw Data int valeus from [0 - 4500]
  int [] rawData = kinect.getRawDepthData();
  
  //values for [0 - 256] strip
  int [] rawData256 = kinect.getRawDepth256Data();
  
  //raw body data 0-6 users 255 nothing
  int [] rawData = kinect.getRawBodyTrack();
  
  //unpacket color values
  int [] colorRaw = kinect.getRawColor();
  
```


#### License

MIT License http://en.wikipedia.org/wiki/MIT_License


