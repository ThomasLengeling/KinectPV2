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
int maxD = 4500; // 4.5mx
int minD = 0;  //  50cm

void setup() {
  size(512*2, 424, P3D);

  kinect = new KinectPV2(this);

  //Enable point cloud
  kinect.enableDepthImg(true);
  kinect.enablePointCloud(true);

  kinect.init();

}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);

  /* Get the point cloud as a PImage
   * Each pixel of the PointCloudDepthImage correspondes to the value 
   * of the Z in the Point Cloud or distances, the values of 
   *  the Point cloud are mapped from (0 - 4500) mm  to gray color (0 - 255)
   */
  image(kinect.getPointCloudDepthImage(), 512, 0);

  //raw Data int valeus from [0 - 4500]
  int [] rawData = kinect.getRawDepthData();

  //Threahold of the point Cloud.
  kinect.setLowThresholdPC(minD);
  kinect.setHighThresholdPC(maxD);
}

void keyPressed() {
  if (key == '1') {
    minD += 10;
    println("Change min: "+minD);
  }

  if (key == '2') {
    minD -= 10;
    println("Change min: "+minD);
  }

  if (key == '3') {
    maxD += 10;
    println("Change max: "+maxD);
  }

  if (key == '4') {
    maxD -=10;
    println("Change max: "+maxD);
  }
}