import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(512*2, 424);

  kinect = new KinectPV2(this);
  
  kinect.enableDepthImg(true);
  kinect.enableBodyTrackImg(true);

  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getDepthImage(), 0, 0);
  image(kinect.getBodyTrackImage(), 512, 0);

  fill(255, 0, 0);
  text(frameRate, 50, 50);
}

void mousePressed() {
  println(frameRate);
  saveFrame();
}
