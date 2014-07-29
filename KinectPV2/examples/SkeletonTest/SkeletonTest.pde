import KinectPV2.*;

KinectPV2 kinect;

Skeleton [] skeleton;

void setup() {
  size(1024, 768);

  kinect = new KinectPV2(this);
  
  kinect.enableSkeleton(true );
  kinect.windowSizeSkeleton(1024, 768);
  kinect.enableBodyTrackImg(true );

  kinect.init();
  
 // skeleton = new Skeleton[6];
  
  
}

void draw() {
  background(0);
  
  image(kinect.getBodyTrackImage(), 0, 0, 200, 200);
  skeleton =  kinect.getSkeleton();
  
   for(int i = 0; i < skeleton.length; i++){
      if(skeleton[i].isTracked()){
        skeleton[i].drawBody();
        skeleton[i].drawHandStates();
      }
    }

  fill(255, 0, 0);
  text(frameRate, 50, 50);
}

void mousePressed() {
  println(frameRate);
  saveFrame();
}
