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

import java.nio.FloatBuffer;

import KinectPV2.*;
import javax.media.opengl.GL;

private KinectPV2 kinect;

float a = 180;
int zval = 350;
float scaleVal = 990;
float rotY = 0;
float rotZ = 0;
float rotX = PI;

float depthVal = 0;

int vertLoc;
int colorLoc;

PGL pgl;
PShader sh;

public void setup() {
  size(1280, 720, P3D);

  kinect = new KinectPV2(this);

  kinect.enableDepthImg(true);
  kinect.enableColorImg(true);
  kinect.enableColorChannel(true);

  // kinect.enableDepthImg(true);
  // kinect.enableColorImg(true);
  //kinect.enablePointCloudColor(true);

  kinect.init();

  sh = loadShader("frag.glsl", "vert.glsl");
}

public void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0, 320, 240);

  FloatBuffer pointCloudBuffer = kinect.getPointCloudColorPos();
  FloatBuffer colorBuffer      = kinect.getColorChannelBuffer();

  pgl = beginPGL();
  sh.bind();

  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");
  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");

  pgl.enableVertexAttribArray(vertLoc);
  pgl.enableVertexAttribArray(colorLoc);

  int vertData = kinect.WIDTHColor * kinect.WIDTHColor;

  pgl.vertexAttribPointer(vertLoc, vertData, PGL.FLOAT, false, 3, pointCloudBuffer);
  pgl.vertexAttribPointer(colorLoc, vertData, PGL.FLOAT, false, 3, colorBuffer);

  pgl.drawArrays(PGL.POINTS, 0, vertData-1);

  pgl.disableVertexAttribArray(vertLoc);
  pgl.disableVertexAttribArray(colorLoc);

  sh.unbind(); 
  endPGL();


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
    a += 1;
    println(a);
  }
  if (key == 'w') {
    a -= 1;
    println(a);
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

