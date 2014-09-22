package KinectPV2;

/*
Copyright (C) 2014  Thomas Sanchez Lengeling.
KinectPV2, Kinect one library for processing

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

import processing.core.PApplet;

public class Skeleton implements Constants{
	
	private   PApplet parent;
	
	protected KJoint [] kJoints;
	
	protected int leftHandState;
	protected int rightHandState;
	
	private boolean tracked;
	
	Skeleton(PApplet p){
		parent = p;
		kJoints  = new KJoint[JointType_Count + 1];
		for(int i = 0; i < JointType_Count + 1; i++){
			kJoints[i] = new KJoint(0,0,0, new Quartenion(), 0);
		}
	}
	
	public boolean isTracked(){
		return tracked;
	}
	
	public KJoint [] getJoints(){
		return kJoints;
	}
	
	public int getLeftHandState(){
		return leftHandState;
	}
	
	public int getRightHandState(){
		return rightHandState;
	}
	
	protected void createSkeleton3D(float [] rawData, int i){
		int index2 = i * (JointType_Count+1) * 9;
		int indexJoint = index2 + (JointType_Count) * 9 + 0;
		if(rawData[indexJoint] == 1.0){
			tracked = true;
		}else{
			tracked = false;
		}
		
		if(tracked){
			for(int j = 0; j < JointType_Count; ++j){			
				int index1 = j * 9;
				kJoints[j].x = rawData[index2 + index1 + 0];
				kJoints[j].y = rawData[index2 + index1 + 1];
				kJoints[j].z = rawData[index2 + index1 + 2];
				
				kJoints[j].orientation.w = rawData[index2 + index1 + 4];
				kJoints[j].orientation.x = rawData[index2 + index1 + 5];
				kJoints[j].orientation.y = rawData[index2 + index1 + 6];
				kJoints[j].orientation.z = rawData[index2 + index1 + 7];
				
				int state =   (int)rawData[index2 + index1 + 7];
				int type  =   (int)rawData[index2 + index1 + 8];
				
				kJoints[j].state = state;
				kJoints[j].type  = type;
				if(type == JointType_HandLeft)
					leftHandState = state;
				if(type == JointType_HandRight)
					rightHandState = state;
			}
		}
		//System.out.println(i+" "+rawData[indexJoint]);
	}
	
	protected void createSkeletonDepth(float [] rawData, int i){
		int index2 = i * (JointType_Count+1) * 9;
		int indexJoint = index2 + (JointType_Count) * 9 + 0;
		if(rawData[indexJoint] == 1.0){
			tracked = true;
		}else{
			tracked = false;
		}
		
		if(tracked){
			for(int j = 0; j < JointType_Count; ++j){			
				int index1 = j * 9;
				kJoints[j].x = rawData[index2 + index1 + 0];
				kJoints[j].y = rawData[index2 + index1 + 1];
				kJoints[j].z = rawData[index2 + index1 + 2];
		
				int state =   (int)rawData[index2 + index1 + 7];
				int type  =   (int)rawData[index2 + index1 + 8];
				
				kJoints[j].state = state;
				kJoints[j].type  = type;
				if(type == JointType_HandLeft)
					leftHandState = state;
				if(type == JointType_HandRight)
					rightHandState = state;
			}
		}
		//System.out.println(i+" "+rawData[indexJoint]);
	}
	
	public void drawBody(){
		drawBone(JointType_Head, JointType_Neck);
		drawBone(JointType_Neck, JointType_SpineShoulder);
		drawBone(JointType_SpineShoulder, JointType_SpineMid);
		drawBone(JointType_SpineMid, JointType_SpineBase);
	    drawBone(JointType_SpineShoulder, JointType_ShoulderRight);
	    drawBone(JointType_SpineShoulder, JointType_ShoulderLeft);
	    drawBone(JointType_SpineBase, JointType_HipRight);
	    drawBone(JointType_SpineBase, JointType_HipLeft);
	    
	    // Right Arm    
	    drawBone(JointType_ShoulderRight, JointType_ElbowRight);
	    drawBone(JointType_ElbowRight, JointType_WristRight);
	    drawBone(JointType_WristRight, JointType_HandRight);
	    drawBone(JointType_HandRight, JointType_HandTipRight);
	    drawBone(JointType_WristRight, JointType_ThumbRight);

	    // Left Arm
	    drawBone(JointType_ShoulderLeft, JointType_ElbowLeft);
	    drawBone(JointType_ElbowLeft, JointType_WristLeft);
	    drawBone(JointType_WristLeft, JointType_HandLeft);
	    drawBone(JointType_HandLeft, JointType_HandTipLeft);
	    drawBone(JointType_WristLeft, JointType_ThumbLeft);

	    // Right Leg
	    drawBone(JointType_HipRight, JointType_KneeRight);
	    drawBone(JointType_KneeRight, JointType_AnkleRight);
	    drawBone(JointType_AnkleRight, JointType_FootRight);

	    // Left Leg
	    drawBone(JointType_HipLeft, JointType_KneeLeft);
	    drawBone(JointType_KneeLeft, JointType_AnkleLeft);
	    drawBone(JointType_AnkleLeft, JointType_FootLeft);
	    
	    drawJoint(JointType_HandTipLeft);
	    drawJoint(JointType_HandTipRight);
	    drawJoint(JointType_FootLeft);
	    drawJoint(JointType_FootRight);
	    
	    drawJoint(JointType_ThumbLeft);
	    drawJoint(JointType_ThumbRight);
	    
	    drawJoint(JointType_Head);
	}
	
	public void drawJoint(int jointType){
		parent.pushMatrix();
		parent.noStroke();
		parent.fill(50);
		parent.translate(kJoints[jointType].x, kJoints[jointType].y, kJoints[jointType].z);
		parent.ellipse(0, 0, 20 ,20);
		parent.popMatrix();
	}
	
	public void drawBone(int jointType1, int jointType2){
		parent.pushMatrix();
		parent.noStroke();
		parent.fill(50);
		parent.translate(kJoints[jointType1].x, kJoints[jointType1].y, kJoints[jointType1].z);
		parent.ellipse(0, 0, 20 ,20);
		parent.stroke(0);
		parent.popMatrix();
		parent.line(kJoints[jointType1].x, kJoints[jointType1].y, kJoints[jointType1].z, kJoints[jointType2].x, kJoints[jointType2].y,  kJoints[jointType2].z);
	}
	
	public void drawHandStates(){
		
		handState(leftHandState);
		parent.pushMatrix();
		parent.translate(kJoints[JointType_HandLeft].x, kJoints[JointType_HandLeft].y, kJoints[JointType_HandLeft].z);
		parent.ellipse(0, 0, 40, 40);
		parent.popMatrix();
		
		parent.pushMatrix();
		handState(rightHandState);
		parent.translate(kJoints[JointType_HandRight].x, kJoints[JointType_HandRight].y, kJoints[JointType_HandRight].z);
		parent.ellipse(0, 0, 40, 40);
		parent.popMatrix();
	}
	
	public void handState(int handState){
		switch(handState){
		case HandState_Open:
			parent.fill(0, 255, 0, 120);
			break;
		case HandState_Closed:
			parent.fill(255, 0, 0, 120);
			break;
		case HandState_Lasso:
			parent.fill(0, 0, 255, 120);
			break;
		case HandState_NotTracked:
			parent.fill(100, 100, 100, 120);
			break;
		}	
	}
}
