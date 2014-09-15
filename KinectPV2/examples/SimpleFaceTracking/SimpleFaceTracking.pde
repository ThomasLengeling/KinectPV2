/*
 Copyright (C) 2014  Thomas Sanchez Lengeling.
 KinectPV2, Kinect one library for processing
 
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

FaceData [] faceData;

void setup() {
  size(1920, 1080, P2D);

  kinect = new KinectPV2(this);

  kinect.enableColorImg(true);
  kinect.enableFaceDetection(true);

  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0);

  faceData =  kinect.getFaceData();

  for (int i = 0; i < faceData.length; i++) {
    if (faceData[i].isFaceTracked()) {
      PVector [] facePoints = faceData[i].getFacePoints();
      Rectangle rectFace = faceData[i].getBoundingRect();

      for (int f = 0; f < facePoints.length; f++) {
        fill(255, 255, 0);
        ellipse(facePoints[f].x, facePoints[f].y, 10, 10);
      }

      stroke(255, 0, 0);
      noFill();
      rect(rectFace.getX(), rectFace.getY(), rectFace.getWidth(), rectFace.getHeight());
    }
  }


  fill(255, 0, 0);
  text(frameRate, 50, 50);
}
