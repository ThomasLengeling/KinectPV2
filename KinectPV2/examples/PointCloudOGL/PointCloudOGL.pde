import java.nio.FloatBuffer;

import KinectPV2*.;
import javax.media.opengl.GL2;

private KinectPV2 kinect;


float a = 0;
int zval = 50;
float scaleVal = 260;
float depthVal = 0;

public void setup() {
  size(1280, 720, P3D);

  kinect = new KinectPV2(this);
  kinect.enableDepthImg(true);
  kinect.enablePointCloud(true);
  depthVal = kinect.getThresholdDepthPointCloud();
  kinect.activateRawDepth(true);

  kinect.init();
}

public void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0, 320, 240);

  kinect.setThresholdPointCloud(depthVal);
  FloatBuffer pointCloudBuffer = kinect.getPointCloudPosFloatBuffer();

  PJOGL pgl = (PJOGL)beginPGL();
  GL2 gl2 = pgl.gl.getGL2();

  gl2.glEnable( GL2.GL_BLEND );
  gl2.glEnable(GL2.GL_POINT_SMOOTH);      

  gl2.glEnableClientState(GL2.GL_VERTEX_ARRAY);
  gl2.glVertexPointer(3, GL2.GL_FLOAT, 0, pointCloudBuffer);

  gl2.glTranslatef(width/2, height/2, zval);
  gl2.glScalef(scaleVal, -1*scaleVal, scaleVal);
  gl2.glRotatef(a, 0.0f, 1.0f, 0.0f);

  gl2.glDrawArrays(GL2.GL_POINTS, 0, kinect.WIDTHDepth * kinect.HEIGHTDepth);

  gl2.glDisable(GL2.GL_BLEND);
  endPGL();

  stroke(255, 0, 0);
  text(frameRate, 50, height- 50);
}

public void mousePressed() {

  println(frameRate);
  saveFrame();
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
