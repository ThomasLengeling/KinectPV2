KinectPV2
==========

### Kinect for Windows v2 library for Processing

Version 0.5

Library is curretly on develop, with SDK Version 1409 (9/16/2014)

---

####Requirements

- A Kinect for Windows v2 Device (K4W2)
- [Kinect SDK v2](http://www.microsoft.com/en-us/download/details.aspx?id=43661)
- Computer with a dedicated USB 3.0 and 64bits
- Windows 8, 8.1
- [Processing 2.2.1](http://processing.org/) or greater


#### Reference and tutorial coming soon

[Webpage](http://codigogenerativo.com/kinect-2-0-library-for-processing/) with tutorial and some couple of useful things. 
#### Install 

- Install [Kinect for Windows SDK v2](http://www.microsoft.com/en-us/download/details.aspx?id=43661)
- Copy KinectPV2 folder to your processing libraries sketch folder. 


---

#### Examples

- Color1920, color image 1920 x 1080
- DepthTest, how to obtain depth information
- MaskTest, get only the contour image of the users.
- PointCloudOGL, point cloud render using opengl
- SimpleFaceTracking, simple face tracking up 6 users.
- Skeleton3d, 3d skeleton.
- SkeletonColor, map color frame to skeleton.
- SkeletonMaskDepth, map depth frame and mask frame to skeleton
- SkeletonTest, simple 6 users skeleton
- imageTest, images frame test


To build the library from source, use this repository ( not always updated ) [KinectPV2_BuildLibs](https://github.com/ThomasLengeling/KinectPV2_BuildLibs)

---

#### Todo

- Heart rate detection
- Contour example
- Color Point Cloud
- Multiple Devices
- [Kinect Fusion](http://msdn.microsoft.com/en-us/library/dn188670.aspx)

---

#### Device

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

To obtain the color, depth, infrared, bodyIndex or the long Exposure Ir you need to active then in the setup method.

```java
 void enableColorImg(boolean toggle);
 void enableDepthImg(boolean toggle);
 void enableInfraredImg(boolean toggle);
 void enableBodyTrackImg(boolean toggle);
 void enableLongExposureInfrared(boolean toggle);
```
initialize in the setup and obtain

```java
  kinect = new KinectV2(this);
  kinect.enableColorImg(true);
  kinect.init();
```

#### Skeleton tracking

#### Face Tracking

#### Point Cloud
---

#### License

MIT License http://en.wikipedia.org/wiki/MIT_License


