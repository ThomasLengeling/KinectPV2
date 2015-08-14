package test;

import java.util.ArrayList;

import KinectPV2.KJoint;
import KinectPV2.KinectPV2;
import KinectPV2.KSkeleton;
import processing.core.*;


public class SkeletonDepthTest  extends PApplet{

	KinectPV2 kinect;
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.SkeletonDepthTest"});
	}
	
	public void settings(){
		size(512*2, 424, P3D);
	}
	

	public void setup() {
	  kinect = new KinectPV2(this);

	  //Enables depth and Body tracking (mask image)
	  kinect.enableBodyTrackImg(true);
	  kinect.enableDepthMaskImg(true);
	  //kinect.enableDepthImg(true);
	  kinect.enableSkeletonDepthMap(true );

	  kinect.init();

	  // skeleton = new Skeleton[6];
	}

	public void draw() {
	  background(0);
	  image(kinect.getBodyTrackImage(), 0, 0);
	  image(kinect.getDepthMaskImage(), 512, 0);
	 // image(kinect.getDepthImage(), 0, 0);
	  ArrayList skeletonArray =  kinect.getSkeletonDepthMap();

	  //individual JOINTS
	  for (int i = 0; i < skeletonArray.size(); i++) {
		  KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
		  if (skeleton.isTracked()) {
			  KJoint[] joints = skeleton.getJoints();

			  int col  = skeleton.getIndexColor();
			  fill(col);
			  stroke(col);

			  drawBody(joints);
	      	drawHandState(joints[KinectPV2.JointType_HandRight]);
	      	drawHandState(joints[KinectPV2.JointType_HandLeft]);
	    }
	  }
	  
	  fill(255, 0, 0);
	  text(frameRate, 50, 50);
	}

	//use different color for each skeleton tracked
	int getIndexColor(int index) {
	  int col = color(255);
	  if (index == 0)
	    col = color(0, 0, 255);
	  else  if (index == 1)
	    col = color(0, 255, 0);
	  else if (index == 2)
	    col = color(255, 0, 0);
	  else if (index == 3)
	    col = color(255, 255, 0);
	  else if (index == 4)
	    col = color(255, 0, 255);
	  else if (index == 5)
	    col = color(0, 255, 255);

	  return col;
	}
	
	//DRAW BODY
	void drawBody(KJoint[] joints) {
	  drawBone(joints, KinectPV2.JointType_Head, KinectPV2.JointType_Neck);
	  drawBone(joints, KinectPV2.JointType_Neck, KinectPV2.JointType_SpineShoulder);
	  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_SpineMid);
	  drawBone(joints, KinectPV2.JointType_SpineMid, KinectPV2.JointType_SpineBase);
	  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_ShoulderRight);
	  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_ShoulderLeft);
	  drawBone(joints, KinectPV2.JointType_SpineBase, KinectPV2.JointType_HipRight);
	  drawBone(joints, KinectPV2.JointType_SpineBase, KinectPV2.JointType_HipLeft);

	  // Right Arm    
	  drawBone(joints, KinectPV2.JointType_ShoulderRight, KinectPV2.JointType_ElbowRight);
	  drawBone(joints, KinectPV2.JointType_ElbowRight, KinectPV2.JointType_WristRight);
	  drawBone(joints, KinectPV2.JointType_WristRight, KinectPV2.JointType_HandRight);
	  drawBone(joints, KinectPV2.JointType_HandRight, KinectPV2.JointType_HandTipRight);
	  drawBone(joints, KinectPV2.JointType_WristRight, KinectPV2.JointType_ThumbRight);

	  // Left Arm
	  drawBone(joints, KinectPV2.JointType_ShoulderLeft, KinectPV2.JointType_ElbowLeft);
	  drawBone(joints, KinectPV2.JointType_ElbowLeft, KinectPV2.JointType_WristLeft);
	  drawBone(joints, KinectPV2.JointType_WristLeft, KinectPV2.JointType_HandLeft);
	  drawBone(joints, KinectPV2.JointType_HandLeft, KinectPV2.JointType_HandTipLeft);
	  drawBone(joints, KinectPV2.JointType_WristLeft, KinectPV2.JointType_ThumbLeft);

	  // Right Leg
	  drawBone(joints, KinectPV2.JointType_HipRight, KinectPV2.JointType_KneeRight);
	  drawBone(joints, KinectPV2.JointType_KneeRight, KinectPV2.JointType_AnkleRight);
	  drawBone(joints, KinectPV2.JointType_AnkleRight, KinectPV2.JointType_FootRight);

	  // Left Leg
	  drawBone(joints, KinectPV2.JointType_HipLeft, KinectPV2.JointType_KneeLeft);
	  drawBone(joints, KinectPV2.JointType_KneeLeft, KinectPV2.JointType_AnkleLeft);
	  drawBone(joints, KinectPV2.JointType_AnkleLeft, KinectPV2.JointType_FootLeft);

	  drawJoint(joints, KinectPV2.JointType_HandTipLeft);
	  drawJoint(joints, KinectPV2.JointType_HandTipRight);
	  drawJoint(joints, KinectPV2.JointType_FootLeft);
	  drawJoint(joints, KinectPV2.JointType_FootRight);

	  drawJoint(joints, KinectPV2.JointType_ThumbLeft);
	  drawJoint(joints, KinectPV2.JointType_ThumbRight);

	  drawJoint(joints, KinectPV2.JointType_Head);
	}

	void drawJoint(KJoint[] joints, int jointType) {
	  pushMatrix();
	  translate(joints[jointType].getX(), joints[jointType].getY(), joints[jointType].getZ());
	  ellipse(0, 0, 25, 25);
	  popMatrix();
	}

	void drawBone(KJoint[] joints, int jointType1, int jointType2) {
	  pushMatrix();
	  translate(joints[jointType1].getX(), joints[jointType1].getY(), joints[jointType1].getZ());
	  ellipse(0, 0, 25, 25);
	  popMatrix();
	  line(joints[jointType1].getX(), joints[jointType1].getY(), joints[jointType1].getZ(), joints[jointType2].getX(), joints[jointType2].getY(), joints[jointType2].getZ());
	}

	void drawHandState(KJoint joint) {
	  noStroke();
	  handState(joint.getState());
	  pushMatrix();
	  translate(joint.getX(), joint.getY(), joint.getZ());
	  ellipse(0, 0, 70, 70);
	  popMatrix();
	}

	/*
	Different hand state
	 KinectPV2.HandState_Open
	 KinectPV2.HandState_Closed
	 KinectPV2.HandState_Lasso
	 KinectPV2.HandState_NotTracked
	 */

	void handState(int handState) {
	  switch(handState) {
	  case KinectPV2.HandState_Open:
	    fill(0, 255, 0);
	    break;
	  case KinectPV2.HandState_Closed:
	    fill(255, 0, 0);
	    break;
	  case KinectPV2.HandState_Lasso:
	    fill(0, 0, 255);
	    break;
	  case KinectPV2.HandState_NotTracked:
	    fill(100, 100, 100);
	    break;
	  }
	}
}
