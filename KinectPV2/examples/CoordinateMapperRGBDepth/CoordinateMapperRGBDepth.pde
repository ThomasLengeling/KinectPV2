/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing
 
Green background example

Not working, check library version 0.7.2
*/

import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(1920, 1080);

  kinect = new KinectPV2(this);
  kinect.enableCoordinateMapperRGBDepth(true);

  kinect.init();

  //SETUP A BACKGROUND IMAGE
  PImage img = loadImage("mars.png");
  img.loadPixels();
  kinect.setCoordBkgImg(img.pixels);
}

void draw() {
  background(0);
  image(kinect.getCoordinateRGBDepthImage(), 0, 0);
}