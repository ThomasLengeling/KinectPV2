import KinectPV2.*;

import gab.opencv.*;
import java.awt.*;

OpenCV opencv;
KinectPV2 kinect;

void setup() {
  size(1920, 1080);

  kinect = new KinectPV2(this);
  kinect.enableColorImg(true);
  kinect.init();

  opencv = new OpenCV(this, 1920, 1080);
}

void draw() {
  opencv.loadImage(kinect.getColorImage());
  PImage img = opencv.getInput();
  image(img, 0, 0);
}
