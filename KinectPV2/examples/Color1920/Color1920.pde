import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(1920, 1080);

  kinect = new KinectPV2(this);
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
