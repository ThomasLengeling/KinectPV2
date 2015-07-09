KinectPV2
==========

### Kinect for Windows v2 library for Processing

=======
Version 0.7.3

Library is curretly on develop, with SDK Version 1409 (9/16/2014)

---

####Requirements

- A Kinect for Windows v2 Device (K4W2)
- [Kinect SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
- Computer with a dedicated USB 3.0 and 64bits
- Windows 8, 8.1
- [Processing 3.0 or 2.2.1](http://processing.org/)
- Update your latest video card driver

#### Reference and tutorial coming soon

[Webpage](http://codigogenerativo.com/kinect-2-0-library-for-processing/) with tutorial and some couple of useful things. 
#### Install 

- Install [Kinect for Windows SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
- Copy KinectPV2 folder to your processing libraries sketch folder. 


---

#### Examples

- TestImages, Test all Frames/Images for the Kinect
- SkeletonMaskDepth, Skeleton positions are mapped to match the depth and body index frames
- SkeletonColor, Skeleton is mapped to match the color frame
- Skeleton3d, 3d Skeleton example needs love.
- SimpleFaceTracking, simple face tracking with mode detection
- PointCloudOGL, Point cloud depth render using openGL and shaders
- PointCloudDepth, point cloud in a single 2d Image and threshold example
- PointCloudColor, Point cloud in color, using openGL and shaders
- MaskTest, Body Index test, and body index with depth
- Mask_findUsers, find number of users base on body index information
- MapDepthToColor, depth to color mapping, depth frame is aligned with color frame
- HDFaceVertex, Face vertices are match with the color frame
- HDColor, 1920 x 1080 RGB frame
- DepthTest, Depth test with raw depth data.
- CoordinateMapperRGBDepth, example broken, check 0.7.2 version
- [OpenCV](https://github.com/atduskgreg/opencv-processing) examples:
  - Live Capture App
  - Find Contours with depth or bodyIndex


To build the library from source, look at the Build_libs folder
- KinectPV2_vc2012, build the .dll library with JNI code
- KinectPV2_Eclipse, build the .jar library.

---

#### Todo

- Heart rate detection
- Multiple Devices
- [Kinect Fusion](http://msdn.microsoft.com/en-us/library/dn188670.aspx)

---

#### Simple Device

To include the library into a processing sketh you just need to import it and initialize it.

```java
import KinectPV2.*;
KinectV2 kinect;

void setup() {

  kinect = new KinectV2(this);
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
  kinect = new KinectV2(this);
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


