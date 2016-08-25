/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing

Test all Images
*/

import KinectPV2.*;

KinectPV2 kinect;

public void setup() {
  size(1536, 848);
  kinect = new KinectPV2(this);
  kinect.enableColorImg(true);
  kinect.enableDepthImg(true);
  kinect.enableInfraredImg(true);
  kinect.enableInfraredLongExposureImg(true);
  kinect.enableBodyTrackImg(true);
  kinect.enableDepthMaskImg(true);
  
  frameRate(60);

  kinect.init();
}

public void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 424, 512, 424);
  image(kinect.getDepthImage(), 0, 0, 512, 424);

  image(kinect.getInfraredImage(), 512, 0, 512, 424);
  image(kinect.getInfraredLongExposureImage(), 512, 424, 512, 424);

  image(kinect.getBodyTrackImage(), 512*2, 0, 512,  424);
  image(kinect.getDepthMaskImage(), 512*2, 424, 512,  424);

  fill(255, 0, 0);
  text(frameRate, 50, 50);
}