/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing
 
Depth Test
*/

import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(512*3, 424, P3D);

  kinect = new KinectPV2(this);
  kinect.enableDepthImg(true);
  kinect.enableInfraredImg(true);
  kinect.enableInfraredLongExposureImg(true);
  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);
  image(kinect.getInfraredImage(), 512, 0);
  image(kinect.getInfraredLongExposureImage(), 512*2, 0);

  stroke(255);
  text(frameRate, 50, height - 50);
}