/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing

Mask test, number of users and find user test
*/

import KinectPV2.*;

KinectPV2 kinect;

boolean foundUsers = false;

void setup() {
  size(1024, 424);

  kinect = new KinectPV2(this);

  kinect.enableDepthImg(true);
  kinect.enableBodyTrackImg(true);

  kinect.init();
}

void draw() {
  background(0);

  image(kinect.getBodyTrackImage(), 0, 0);
  image(kinect.getDepthImage(), 512, 0);

  //raw body data 0-6 users 255 nothing
  int [] rawData = kinect.getRawBodyTrack();

  foundUsers = false;
  //iterate through 1/5th of the data
  for(int i = 0; i < rawData.length; i+=5){
    if(rawData[i] != 255){
     //found something
     foundUsers = true;
     break;
    }
  }


  fill(0);
  textSize(16);
  text(kinect.getNumOfUsers(), 50, 50);
  text("Found User: "+foundUsers, 50, 70);
  text(frameRate, 50, 90);

}

void mousePressed() {
  println(frameRate);
  ///saveFrame();
}
