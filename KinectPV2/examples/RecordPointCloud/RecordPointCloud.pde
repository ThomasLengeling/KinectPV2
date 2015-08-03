/*
Thomas Sanchez Lengeling.
 http://codigogenerativo.com/

 KinectPV2, Kinect for Windows v2 library for processing

 How to record Point Cloud Data and store it in several obj files.
 */

import java.nio.*;
import KinectPV2.*;

KinectPV2 kinect;

PGL pgl;
PShader sh;

int  vertLoc;

//transformations
float a = 1.2;
int zval = 50;
float scaleVal = 260;



//Distance Threashold
int maxD = 4500; // 4.5 m
int minD = 0;  //  50 cm

int numFrames =  30 * 60; // 1m of recording

ArrayList<Frame> mFrames;

void setup() {
  size(512*2, 424, P3D);

  kinect = new KinectPV2(this);

  mFrames = new ArrayList<Frame>();

  //Enable point cloud
  kinect.enableDepthImg(true);
  kinect.enablePointCloud(true);

  kinect.init();

  sh = loadShader("frag.glsl", "vert.glsl");

  //set framerate to 30
  frameRate(30);
}

void draw() {
  background(0);
  background(0);

  //draw the depth capture images
  image(kinect.getDepthImage(), 0, 0, 320, 240);
  image(kinect.getPointCloudDepthImage(), 320, 0, 320, 240);

  //translate the scene to the center
  translate(width / 2, height / 2, zval);
  scale(scaleVal, -1 * scaleVal, scaleVal);
  rotate(a, 0.0f, 1.0f, 0.0f);

  // Threahold of the point Cloud.
  kinect.setLowThresholdPC(minD);
  kinect.setHighThresholdPC(maxD);

  //get the points in 3d space
  FloatBuffer pointCloudBuffer = kinect.getPointCloudDepthPos();

    //data size
  int vertData = kinect.WIDTHDepth * kinect.HEIGHTDepth;

  stroke(255);
  beginShape(POINTS);
  for (int i = 0; i < vertData * 3 - 3; i++) {
    //if ( i % 500 == 0) {
      float x =  pointCloudBuffer.get(i + 0);
      float y =  pointCloudBuffer.get(i + 1);
      float z =  pointCloudBuffer.get(i + 2);
      vertex(x, y, z);
   // }
  }
  endShape();

/*
  pgl = beginPGL();
  sh.bind();

  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");

  //color for each POINT of the point cloud
  sh.set("fragColor", 1.0f, 1.0f, 1.0f, 1.0f);

  pgl.enableVertexAttribArray(vertLoc);



  //pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 3 * (Float.SIZE/8), pointCloudBuffer);
  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, pointCloudBuffer);
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  pgl.disableVertexAttribArray(vertLoc);

  sh.unbind();
  endPGL();
  */

  stroke(255, 0, 0);
  text(frameRate, 50, height - 50);
}

public void mousePressed() {

  println(frameRate);
  // saveFrame();
}

public void keyPressed() {
  if (key == 'a') {
    zval +=1;
    println(zval);
  }
  if (key == 's') {
    zval -= 1;
    println(zval);
  }

  if (key == 'z') {
    scaleVal += 0.1;
    println(scaleVal);
  }
  if (key == 'x') {
    scaleVal -= 0.1;
    println(scaleVal);
  }

  if (key == 'q') {
    a += 0.1;
    println(a);
  }
  if (key == 'w') {
    a -= 0.1;
    println(a);
  }

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
    maxD -= 10;
    println("Change max: "+maxD);
  }
}
