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

import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(1920, 1080);
  
  kinect = new KinectPV2(this);
  kinect.enableHDFaceDetection(true);
  kinect.init();
}

void draw() {
  background(0);
  
 //DRAW COLOR IMAGE MAP
 // image(kinect.getColorImage(), 0, 0);
  
  HDFaceData []  hdFaceData = kinect.getHDFaceVertex();
  
  stroke(0, 255, 0);
  for (int j = 0; j < KinectPV2.BODY_COUNT; j++) {
    beginShape(POINTS);
    if (hdFaceData[j].isTracked()) {
      for (int i = 0; i < KinectPV2.HDFaceVertexCount; i++) {
        float x = hdFaceData[j].getX(i);
        float y = hdFaceData[j].getY(i);
        vertex(x, y);
      }
    }
    endShape();
  }
}
