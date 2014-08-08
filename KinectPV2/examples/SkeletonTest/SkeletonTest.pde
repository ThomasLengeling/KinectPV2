import KinectPV2.*;

KinectPV2 kinect;

Skeleton [] skeleton;

/*
JointType_SpineBase     = 0;
 JointType_SpineMid      = 1;
 JointType_Neck          = 2;
 JointType_Head          = 3;
 JointType_ShoulderLeft  = 4;
 JointType_ElbowLeft     = 5;
 JointType_WristLeft     = 6;
 JointType_HandLeft      = 7;
 JointType_ShoulderRight = 8;
 JointType_ElbowRight    = 9;
 JointType_WristRight    = 10;
 JointType_HandRight     = 11;
 JointType_HipLeft       = 12;
 JointType_KneeLeft      = 13;
 JointType_AnkleLeft     = 14;
 JointType_FootLeft      = 15;
 JointType_HipRight      = 16;
 JointType_KneeRight     = 17;
 JointType_AnkleRight    = 18;
 JointType_FootRight     = 19;
 JointType_SpineShoulder = 20;
 JointType_HandTipLeft   = 21;
 JointType_ThumbLeft     = 22;
 JointType_HandTipRight  = 23;
 JointType_ThumbRight    = 24;
 */

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


  for (int i = 0; i < skeleton.length; i++) {
    if (skeleton[i].isTracked()) {
      skeleton[i].drawBody();
      skeleton[i].drawHandStates();
    }
  }


  //individual JOINTS

  for (int i = 0; i < skeleton.length; i++) {
    if (skeleton[i].isTracked()) {
      KJoint[] joints = skeleton[i].getJoints();
      ellipse(joints[kinect.JointType_Head].getX(), joints[kinect.JointType_Head].getY(), 50, 50);
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
