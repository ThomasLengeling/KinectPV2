/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing

Point Cloud Color Example using low level openGL calls and Shaders
*/

import java.nio.FloatBuffer;

import KinectPV2.*;

private KinectPV2 kinect;

//values for the 3d scene
//rotation
float a = 3;
//z scale value
int   zval     = 350;
//value to scale the point cloud
float scaleVal = 990;

//rotation
float rotY = 0;
float rotZ = 0;
float rotX = PI;

float depthVal = 0;

int vertLoc;
int colorLoc;

//openGL instances
//render ot openGL object
PGL pgl;

//create a shader
PShader sh;

public void setup() {
  size(1280, 720, P3D);

  kinect = new KinectPV2(this);

  kinect.enableDepthImg(true);
  kinect.enableColorImg(true);
  kinect.enableColorPointCloud(true);

  kinect.init();

  //create shader object with a vertex shader and a fragment shader
  sh = loadShader("frag.glsl", "vert.glsl");
}

public void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0, 320, 240);

  pushMatrix();
  translate(width / 2, height / 2, zval);
  scale(scaleVal, -1 * scaleVal, scaleVal);
  rotate(a, 0.0f, 1.0f, 0.0f);

  //render to the openGL object
  pgl = beginPGL();
  sh.bind();

  //obtain the point cloud positions
  FloatBuffer pointCloudBuffer = kinect.getPointCloudColorPos();

  //get the color for each point of the cloud Points
  FloatBuffer colorBuffer      = kinect.getColorChannelBuffer();

  //send the the vertex positions (point cloud) and color down the render pipeline
  //positions are render in the vertex shader, and color in the fragment shader
  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");
  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");

  //enable drawing to the vertex and color buffer
  pgl.enableVertexAttribArray(vertLoc);
  pgl.enableVertexAttribArray(colorLoc);

  int vertData = kinect.WIDTHColor * kinect.HEIGHTColor;

  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, pointCloudBuffer);
  pgl.vertexAttribPointer(colorLoc, 3, PGL.FLOAT, false, 0, colorBuffer);
    
  //draw the point cloud as a set of points
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  //disable drawing
  pgl.disableVertexAttribArray(vertLoc);
  pgl.disableVertexAttribArray(colorLoc);

  
  //close the shader
  sh.unbind();
  //close the openGL object
  endPGL();

  popMatrix();


  stroke(255, 0, 0);
  text(frameRate, 50, height- 50);
}

public void mousePressed() {

  println(frameRate);
  //  saveFrame();
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
    scaleVal += 1;
    println(scaleVal);
  }
  if (key == 'x') {
    scaleVal -= 1;
    println(scaleVal);
  }

  if (key == 'q') {
    a += 0.1;
    println("angle "+a);
  }
  if (key == 'w') {
    a -= 0.1;
    println("angle "+a);
  }

  if (key == 'c') {
    depthVal -= 0.01;
    println(depthVal);
  }

  if (key == 'v') {
    depthVal += 0.01;
    println(depthVal);
  }
}