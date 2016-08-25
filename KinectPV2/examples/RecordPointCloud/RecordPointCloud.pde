/*
Thomas Sanchez Lengeling.
 http://codigogenerativo.com/
 
 KinectPV2, Kinect for Windows v2 library for processing
 
 How to record Point Cloud Data and store it in several obj files.
 Record with 'r'
 */

import java.util.ArrayList;
import java.nio.*;
import KinectPV2.*;

KinectPV2 kinect;

PGL pgl;
PShader sh;

int  vertLoc;

//transformations
float a = 7;
int zval = -50;
float scaleVal = 220;

//Distance Threashold
int maxD = 4500; // 4.5 m
int minD = 0;  //  50 cm


int numFrames =  30; // 30 frames  = 1s of recording
int frameCounter = 0; // frame counter

boolean recordFrame = false;  //recording flag
boolean doneRecording = false;

//Array where all the frames are allocated
ArrayList<FrameBuffer> mFrames;

//VBO buffer location in the GPU
int vertexVboId;


void setup() {
  size(1024, 768, P3D);

  kinect = new KinectPV2(this);

  //create arrayList
  mFrames = new ArrayList<FrameBuffer>();

  //Enable point cloud
  kinect.enableDepthImg(true);
  kinect.enablePointCloud(true);

  kinect.init();

  sh = loadShader("frag.glsl", "vert.glsl");
  
  PGL pgl = beginPGL();

  IntBuffer intBuffer = IntBuffer.allocate(1);
  pgl.genBuffers(1, intBuffer);

  //memory location of the VBO
  vertexVboId = intBuffer.get(0);

  endPGL();

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

  //data size, 512 x 424 x 3 (XYZ) coordinate
  int vertData = kinect.WIDTHDepth * kinect.HEIGHTDepth  * 3;

  pgl = beginPGL();
  sh.bind();

  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");

  pgl.enableVertexAttribArray(vertLoc);

  //vertex
  {
    pgl.bindBuffer(PGL.ARRAY_BUFFER, vertexVboId);

    pgl.bufferData(PGL.ARRAY_BUFFER, Float.BYTES * vertData, pointCloudBuffer, PGL.DYNAMIC_DRAW);

    pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, Float.BYTES * 3, 0);
  }
  
  // unbind VBOs
  pgl.bindBuffer(PGL.ARRAY_BUFFER, 0);

  pgl.drawArrays(PGL.POINTS, 0, vertData);

  pgl.disableVertexAttribArray(vertLoc);

  sh.unbind();
  endPGL();

  //allocate the current pointCloudBuffer into an array of FloatBuffers
  allocateFrame(pointCloudBuffer);

  //when the allocation is done write the obj frames
  writeFrames();

  stroke(255, 0, 0);
  text(frameRate, 50, height - 50);
}

//allocate all the frame in a temporary array
void allocateFrame(FloatBuffer buffer) {
  if (recordFrame) {
    if ( frameCounter < numFrames) {
      FrameBuffer frameBuffer = new FrameBuffer(buffer);
      frameBuffer.setFrameId(frameCounter);
      mFrames.add(frameBuffer);
    } else {
      recordFrame = false;
      doneRecording = true;
    }
    frameCounter++;
  }
}

//Write all the frames recorded
void writeFrames() {
  if (doneRecording) {
    for (int i = 0; i < mFrames.size(); i++) {
      FrameBuffer fBuffer =  (FrameBuffer)mFrames.get(i);
      fBuffer.saveOBJFrame();
    }
    doneRecording = false;
    println("Done Recording frames: "+numFrames);
  }
}

public void mousePressed() {

  println(frameRate);
  // saveFrame();
}

public void keyPressed() {

  //start recording 30 frames with 'r'
  if (key == 'r') {
    recordFrame = true;
  }
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