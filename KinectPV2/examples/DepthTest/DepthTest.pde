/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing
 
Depth Test
*/

import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(512*2, 424*2, P3D);

  kinect = new KinectPV2(this);
  kinect.enableDepthImg(true);
  kinect.enableInfraredImg(true);
  kinect.enableInfraredLongExposureImg(true);
  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);
  image(kinect.getDepth256Image(), 512, 0);
  image(kinect.getInfraredImage(), 0, 424);
  image(kinect.getInfraredLongExposureImage(), 512, 424);
  
  //raw Data int valeus from [0 - 4500]
  int [] rawData = kinect.getRawDepthData();
  
  //values for [0 - 256] strip
  int [] rawData256 = kinect.getRawDepth256Data();

  stroke(255);
  text(frameRate, 50, height - 50);
}