KinectPV2
==========

### Kinect for Windows v2 library for Processing

<<<<<<< HEAD
<<<<<<< HEAD
Version 0.5
=======
Version 0.7
>>>>>>> origin/dev
=======
Version 0.7
>>>>>>> 5cda745594f90fbedfefe7c38a50632d888f8780

Library is curretly on develop, with SDK Version 1409 (9/16/2014)

---

####Requirements

- A Kinect for Windows v2 Device (K4W2)
- [Kinect SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
- Computer with a dedicated USB 3.0 and 64bits
- Windows 8, 8.1
- [Processing 2.2.1](http://processing.org/) or greater
- Update your latest video card driver


#### Reference and tutorial coming soon

[Webpage](http://codigogenerativo.com/kinect-2-0-library-for-processing/) with tutorial and some couple of useful things. 
#### Install 

- Install [Kinect for Windows SDK v2](http://www.microsoft.com/en-us/kinectforwindows/default.aspx)
- Copy KinectPV2 folder to your processing libraries sketch folder. 


---

#### Examples

- Color1920, color image 1920 x 1080.
- DepthTest, how to obtain depth information.
- MaskTest, get only the contour image of the users.
- PointCloudOGL, point cloud render using opengl.
- PointCloudDepth, obtain the point cloud as a deph Image, and as a int [] array.
- SimpleFaceTracking, simple face tracking up 6 users.
- Skeleton3d, 3d skeleton.
- SkeletonColor, map color frame to skeleton.
- SkeletonMaskDepth, map depth frame and mask frame to skeleton.
- SkeletonTest, simple 6 users skeleton.
- imageTest, images frame test.
<<<<<<< HEAD
<<<<<<< HEAD
=======
- CoordinateMapperRGBDepth, RGB+Depth
- HDFaceVertex. HDFace Points
>>>>>>> origin/dev
=======
- CoordinateMapperRGBDepth, RGB+Depth
- HDFaceVertex. HDFace Points
>>>>>>> 5cda745594f90fbedfefe7c38a50632d888f8780


To build the library from source, use this repository ( not always updated ) [KinectPV2_BuildLibs](https://github.com/ThomasLengeling/KinectPV2_BuildLibs)

---

#### Todo

- Heart rate detection
- Contour example
- Color Point Cloud
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
 void enableLongExposureInfrared(boolean toggle);
 
 PImage getColorImage();
 PImage getDepthImage();
 PImage getInfraredImage();
 PImage getBodyTrackImage();
 PImage getLongExposureInfrared();
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

If you need to obtain the raw data of each Image as a array of int's you need to active it first.

```java
void activateRawColor(boolean toggle);
void activateRawDepth(boolean toggle);
void activateRawDepthMaskImg(boolean toggle);
void activateRawInfrared(boolean toggle);
void activateRawBodyTrack(boolean toggle);
void activateRawLongExposure(boolean toggle);
```

to obtain the raw data as a array of intengers (RGB or gray scale)

```java
int [] getRawDepth();
int [] getRawDepthMask();
int [] getRawColor();
int [] getRawInfrared();
int [] getRawBodyTrack();
int [] getRawLongExposure();
```

---

#### License

MIT License http://en.wikipedia.org/wiki/MIT_License


