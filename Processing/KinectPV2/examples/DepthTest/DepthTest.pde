import KinectPV2.*;

KinectV2 kinect;

void setup() {
  size(512*3, 424);

  kinect = new KinectV2(this);
  kinect.enableDepthImg(true);
  kinect.enableInfraredImg(true);
  kinect.enableLongExposureInfraredImg(true);
  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);
  image(kinect.getInfraredImage(), 512, 0);
  image(kinect.getLongExposureInfraredImage(), 512*2, 0);
  
  fill(255, 255, 0);
  text(frameRate, 50, 50);
}

void mousePressed() {
  println(frameRate);
  saveFrame();
}

