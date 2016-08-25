	/*
	Copyright (C) 2014  Thomas Sanchez Lengeling.
	 KinectPV2, Kinect for Windows v2 library for processing
	 
	 Permission is hereby granted, free of charge, to any person obtaining a copy
	 of this software and associated documentation files (the "Software"), to deal
	 in the Software without restriction, including without limitation the rights
	 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	 copies of the Software, and to permit persons to whom the Software is
	 furnished to do so, subject to the following conditions:
	 
	 The above copyright notice and this permission notice shall be included in
	 all copies or substantial portions of the Software.
	 
	 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	 THE SOFTWARE.
	 */

package test;

import java.util.ArrayList;

import processing.core.PApplet;
import KinectPV2.*;
	
public class SkeletonColorTest extends PApplet{

	KinectPV2 kinect;
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.SkeletonColorTest"});
	}
	
	public void settings(){
		size(1920, 1080, P3D);
	}

	public void setup() {

	  kinect = new KinectPV2(this);

	  kinect.enableSkeletonColorMap(true);
	  kinect.enableColorImg(true);

	  kinect.init();
	}

	public void draw() {
	  background(0);

	  image(kinect.getColorImage(), 0, 0, width, height);

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
	      
			  //draw different color for each hand state
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
	    fill(255, 255, 255);
	    break;
	  }
	}

}
