/*
Thomas Sanchez Lengeling.
 http://codigogenerativo.com/
 KinectPV2, Kinect for Windows v2 library for processing

 Point Cloud example using openGL and Shaders
 */

import java.nio.*;
import KinectPV2.*;

KinectPV2 kinect;


int  vertLoc;

//transformations
float a = 1.2;
int zval = 50;
float scaleVal = 260;

//Distance Threashold
int maxD = 4000; // 4m
int minD = 0;  //  0m

//openGL object and shader
PGL     pgl;
PShader sh;

public void setup() {
  size(1280, 720, P3D);

  kinect = new KinectPV2(this);

  kinect.enableDepthImg(true);

  kinect.enablePointCloud(true);

  kinect.setLowThresholdPC(minD);
  kinect.setHighThresholdPC(maxD);

  kinect.init();

  sh = loadShader("frag.glsl", "vert.glsl");
}

public void draw() {
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

  // obtain XYZ the values of the point cloud
  // for(int i = 0; i < kinect.WIDTHDepth * kinect.HEIGHTDepth; i++){
  //    float x = pointCloudBuffer.get(i*3 + 0);
  //    float y = pointCloudBuffer.get(i*3 + 1);
  //    float z = pointCloudBuffer.get(i*3 + 2);
  // }
  
  //begin openGL calls and bind the shader
  pgl = beginPGL();
  sh.bind();

  //obtain the vertex location in the shaders.
  //useful to know what shader to use when drawing the vertex positions
  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");

  //default color for each point is white.
  //change the color for each point here
  sh.set("fragColor", 1.0f, 1.0f, 1.0f, 1.0f);

  pgl.enableVertexAttribArray(vertLoc);

  //data size
  int vertData = kinect.WIDTHDepth * kinect.HEIGHTDepth;
  
  //attach the vertex positions to the video card
  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, pointCloudBuffer);
  
  //draw the point buffer as a set of POINTS
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  //disable the vertex positions
  pgl.disableVertexAttribArray(vertLoc);

  //finish drawing
  sh.unbind();
  endPGL();
  

  stroke(255, 0, 0);
  text(frameRate, 50, height - 50);
}

public void mousePressed() {
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