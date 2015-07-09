/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing
 
Point Cloud example in a 2d Image, with threshold example 
*/

import KinectPV2.KJoint;
import KinectPV2.*;

KinectPV2 kinect;

//Distance Threashold
int maxD = 5000; // 4m
int minD = 50;  //  50cm

void setup() {
  size(512*2, 424, P3D);

  kinect = new KinectPV2(this);

  //Enable point cloud
  kinect.enableDepthImg(true);

  kinect.activateRawDepth(true);
  kinect.enablePointCloud(true);

  kinect.init();

}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);

  /* Get the point cloud as a PImage
   * Each pixel of the PointCloudDepthImage correspondes to the value 
   * of the Z in the Point Cloud or distances, the values of 
   *  the Point cloud are mapped from (0.0 - 8.0)  to gray color (0 - 255)
   */
  image(kinect.getPointCloudDepthImage(), 512, 0);

  //get each pixel as int [] 512 x 424
  //int [] rawData = kinect.getRawPointCloudDepth();

  //Threahold of the point Cloud.
  kinect.setLowThresholdPC(minD);
  kinect.setHighThresholdPC(maxD);
}

void keyPressed() {
  if (key == '1') {
    minD += 0.01;
    println("Change min: "+minD);
  }

  if (key == '2') {
    minD -= 0.01;
    println("Change min: "+minD);
  }

  if (key == '3') {
    maxD += 0.01;
    println("Change max: "+maxD);
  }

  if (key == '4') {
    maxD -= 0.01;
    println("Change max: "+maxD);
  }
}