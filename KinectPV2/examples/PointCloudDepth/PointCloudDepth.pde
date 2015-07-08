/*
 Copyright (C) 2014  Thomas Sanchez Lengeling.
 KinectPV2, Kinect for Windows v2 library for processing
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
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