import KinectPV2.*;

KinectV2 kinect;


void setup() {
  size(900, 768);

  kinect = new KinectV2(this);
  kinect.enableColorImg(true);
  kinect.enableDepthImg(true);
  kinect.enableInfraredImg(true);

  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0, 854, 480);
  image(kinect.getDepthImage(), 70, 480, 320, 240);
  image(kinect.getInfraredImage(), 460, 480, 320, 240);


  fill(255, 0, 0);
  text(frameRate, 50, 50);
}

void mousePressed() {
  println(frameRate);
  saveFrame();
}

