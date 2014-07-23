import KinectPV2.*;

KinectV2 kinect;

void setup() {
  size(900, 768);

  kinect = new KinectV2(this);
  kinect.enableColorImg(true);

  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0, 1920, 1080);

  fill(255, 0, 0);
  text(frameRate, 50, 50);
}

void mousePressed() {
  println(frameRate);
  saveFrame();
}

