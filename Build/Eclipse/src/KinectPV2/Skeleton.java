package KinectPV2;

import processing.core.PApplet;

public class Skeleton implements Constants{
	
	private   PApplet parent;
	
	protected Joint [] joints;
	
	protected int leftHandState;
	protected int rightHandState;
	
	private boolean tracked;
	
	Skeleton(PApplet p){
		parent = p;
		joints  = new Joint[JointType_Count + 1];
		for(int i = 0; i < JointType_Count + 1; i++){
			joints[i] = new Joint(0,0,0,0);
		}
	}
	
	public boolean isTracked(){
		return tracked;
	}
	
	public Joint [] getJoints(){
		return joints;
	}
	
	public int getLeftHandState(){
		return leftHandState;
	}
	
	public int getRightHandState(){
		return rightHandState;
	}
	
	public void createSkeletons(float [] rawData, int i){
		int index2 = i * JointType_Count * 5;
		if(rawData[index2 + JointType_Count*5 + 0] == 1.0)
			tracked = true;
		else
			tracked = false;
		
		if(tracked){
			for(int j = 0; j < JointType_Count; j ++){			
				int index1 = j * 5;
				joints[j].x = rawData[index2 + index1 + 0];
				joints[j].y = rawData[index2 + index1 + 1];
				joints[j].z = 0;
				
				int state =   (int)rawData[index2 + index1 + 3];
				int type  =   (int)rawData[index2 + index1 + 4];
				
				joints[j].state = state;
				joints[j].type  = type;
				if(type == JointType_HandLeft)
					leftHandState = state;
				if(type == JointType_HandRight)
					rightHandState = state;
			}
		}
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
	      
	}
	
	public void drawJoint(int jointType){
		parent.noStroke();
		parent.fill(155);
		parent.ellipse(joints[jointType].x, joints[jointType].y, 10 ,10);
		
	}
	
	public void drawBone(int jointType1, int jointType2){
		parent.noStroke();
		parent.fill(155);
		parent.ellipse(joints[jointType1].x, joints[jointType1].y, 20 ,20);
		parent.stroke(255);
		parent.line(joints[jointType1].x, joints[jointType1].y, joints[jointType2].x, joints[jointType2].y);
	}
	
	public void drawHandStates(){
		
		handState(leftHandState);
		parent.ellipse(joints[JointType_HandLeft].x, joints[JointType_HandLeft].y, 90, 90);
		
		handState(rightHandState);
		parent.ellipse(joints[JointType_HandRight].x, joints[JointType_HandRight].y, 90, 90);
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
