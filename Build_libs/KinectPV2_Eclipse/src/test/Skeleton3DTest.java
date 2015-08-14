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
import processing.core.PVector;
import KinectPV2.*;

public class Skeleton3DTest extends PApplet {

	KinectPV2 kinect;


	float zVal = 300;
	float rotX = PI;

	public void setup() {
		size(1024, 768, P3D);

		kinect = new KinectPV2(this);

		//kinect.enableColorImg(true);
		//kinect.enableSkeleton3DMap(true);
		// enable 3d Skeleton with (x,y,z) position
		
		kinect.enableDepthImg(true);
		kinect.enableSkeletonDepthMap(true);
		
		kinect.init();
	}

	public void draw() {
		background(0);

		//image(kinect.getColorImage(), 0, 0, 320, 240);
		image(kinect.getDepthImage(), 0, 0, 320, 240);
		
		ArrayList skeletonArray =  kinect.getSkeletonDepthMap();

		// translate the scene to the center
		//pushMatrix();
		//translate(width / 2, height / 2, 0);
		//scale(zVal);
		//rotateX(rotX);


		for (int i = 0; i < skeletonArray.size(); i++) {
			KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
			if (skeleton.isTracked()) {
				KJoint[] joints = skeleton.getJoints();

				// draw different color for each hand state
				drawHandState(joints[KinectPV2.JointType_HandRight]);
				drawHandState(joints[KinectPV2.JointType_HandLeft]);
				// Draw body
				int col = skeleton.getIndexColor();
				stroke(col);
				drawBody(joints);

			//	pushMatrix();
				
				scale(0.1f);
				for (int j = 0; j < KinectPV2.JointType_Count; j++) {
					int parentType = getParentJoint(joints[KinectPV2.JointType_Neck]
							.getType());
					//println(parentType + " ");

					PVector pos = joints[j].getPosition();
					KQuaternion orientation = joints[j].getOrientation();
					
					println(orientation.getX()+" "+orientation.getY());

					PVector csX = createEndPoint(pos,
							orientation.rotate(0.1f, 0.0f, 0.0f));
					PVector csY = createEndPoint(pos,
							orientation.rotate(0.0f, 0.1f, 0.0f));
					PVector csZ = createEndPoint(pos,
							orientation.rotate(0.0f, 0.0f, 0.1f));

					PVector dsX = kinect.MapCameraPointToDepthSpace(csX);
					PVector dsY = kinect.MapCameraPointToDepthSpace(csY);
					PVector dsZ = kinect.MapCameraPointToDepthSpace(csZ);
					
					println(joints[j].getPosition().x+" "+joints[j].getPosition().y+" "+dsX.x+" "+dsX.y);
					
					strokeWeight(1.0f);
					stroke(255, 0, 0);
					beginShape(LINES);
					vertex(joints[j].getPosition().x, joints[j].getPosition().y);
					vertex(dsX.x, dsX.y);
					endShape();
					
					stroke(0, 255, 0);
					beginShape(LINES);
					vertex(joints[j].getPosition().x, joints[j].getPosition().y);
					vertex(dsY.x, dsY.y);
					endShape();
					
					stroke(0, 0, 255);
					beginShape(LINES);
					vertex(joints[j].getPosition().x, joints[j].getPosition().y);
					vertex(dsZ.x, dsZ.y);
					endShape();


					// PVector trans = kinect.MapCameraPointToDepthSpace(pos);

					// println(trans.x+" "+trans.y);
					// float kPitch = 0;
					// float kYaw = 0;
					// float kRoll = 0;
					// PVector rotation =
					// extractRotationInDegrees(joints[i].getOrientation());
				}
				//popMatrix();
			}
		}
		//popMatrix();

		fill(255, 0, 0);
		text(frameRate, 50, 50);
	}
	
	PVector extractRotationInDegrees(KQuaternion pQuaternion) {
		float x = pQuaternion.getX();
		float y = pQuaternion.getY();
		float z = pQuaternion.getZ();
		float w = pQuaternion.getW();

		// convert face rotation quaternion to Euler angles in degrees
		float dPitch = atan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z)
				/ PI * 180.0f;
		float dYaw = asin(2 * (w * y - x * z)) / PI * 180.0f;
		float dRoll = atan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z)
				/ PI * 180.0f;

		// clamp rotation values in degrees to a specified range of values to
		// control the refresh rate
		float increment = 5;
		float kPitch = ((dPitch + increment / 2.0f
				* (dPitch > 0.0f ? 1.0f : -1.0f)) / increment)
				* (increment);
		float kYaw = ((dYaw + increment / 2.0f * (dYaw > 0.0f ? 1.0f : -1.0f)) / increment)
				* (increment);
		float kRoll = ((dRoll + increment / 2.0f
				* (dRoll > 0.0f ? 1.0f : -1.0f)) / increment)
				* (increment);
		return new PVector(kPitch, kYaw, kRoll);
	}
	


	void drawBody(KJoint[] joints) {
		drawBone(joints, KinectPV2.JointType_Head, KinectPV2.JointType_Neck);
		drawBone(joints, KinectPV2.JointType_Neck,
				KinectPV2.JointType_SpineShoulder);
		drawBone(joints, KinectPV2.JointType_SpineShoulder,
				KinectPV2.JointType_SpineMid);

		drawBone(joints, KinectPV2.JointType_SpineMid,
				KinectPV2.JointType_SpineBase);
		drawBone(joints, KinectPV2.JointType_SpineShoulder,
				KinectPV2.JointType_ShoulderRight);
		drawBone(joints, KinectPV2.JointType_SpineShoulder,
				KinectPV2.JointType_ShoulderLeft);
		drawBone(joints, KinectPV2.JointType_SpineBase,
				KinectPV2.JointType_HipRight);
		drawBone(joints, KinectPV2.JointType_SpineBase,
				KinectPV2.JointType_HipLeft);

		// Right Arm
		drawBone(joints, KinectPV2.JointType_ShoulderRight,
				KinectPV2.JointType_ElbowRight);
		drawBone(joints, KinectPV2.JointType_ElbowRight,
				KinectPV2.JointType_WristRight);
		drawBone(joints, KinectPV2.JointType_WristRight,
				KinectPV2.JointType_HandRight);
		drawBone(joints, KinectPV2.JointType_HandRight,
				KinectPV2.JointType_HandTipRight);
		drawBone(joints, KinectPV2.JointType_WristRight,
				KinectPV2.JointType_ThumbRight);

		// Left Arm
		drawBone(joints, KinectPV2.JointType_ShoulderLeft,
				KinectPV2.JointType_ElbowLeft);
		drawBone(joints, KinectPV2.JointType_ElbowLeft,
				KinectPV2.JointType_WristLeft);
		drawBone(joints, KinectPV2.JointType_WristLeft,
				KinectPV2.JointType_HandLeft);
		drawBone(joints, KinectPV2.JointType_HandLeft,
				KinectPV2.JointType_HandTipLeft);
		drawBone(joints, KinectPV2.JointType_WristLeft,
				KinectPV2.JointType_ThumbLeft);

		// Right Leg
		drawBone(joints, KinectPV2.JointType_HipRight,
				KinectPV2.JointType_KneeRight);
		drawBone(joints, KinectPV2.JointType_KneeRight,
				KinectPV2.JointType_AnkleRight);
		drawBone(joints, KinectPV2.JointType_AnkleRight,
				KinectPV2.JointType_FootRight);

		// Left Leg
		drawBone(joints, KinectPV2.JointType_HipLeft,
				KinectPV2.JointType_KneeLeft);
		drawBone(joints, KinectPV2.JointType_KneeLeft,
				KinectPV2.JointType_AnkleLeft);
		drawBone(joints, KinectPV2.JointType_AnkleLeft,
				KinectPV2.JointType_FootLeft);

		drawJoint(joints, KinectPV2.JointType_HandTipLeft);
		drawJoint(joints, KinectPV2.JointType_HandTipRight);
		drawJoint(joints, KinectPV2.JointType_FootLeft);
		drawJoint(joints, KinectPV2.JointType_FootRight);

		drawJoint(joints, KinectPV2.JointType_ThumbLeft);
		drawJoint(joints, KinectPV2.JointType_ThumbRight);

		drawJoint(joints, KinectPV2.JointType_Head);
	}

	void drawJoint(KJoint[] joints, int jointType) {
		strokeWeight(2.0f + joints[jointType].getZ() * 8);
		point(joints[jointType].getX(), joints[jointType].getY(),
				joints[jointType].getZ());
	}

	void drawBone(KJoint[] joints, int jointType1, int jointType2) {
		strokeWeight(2.0f + joints[jointType1].getZ() * 8);
		point(joints[jointType2].getX(), joints[jointType2].getY(),
				joints[jointType2].getZ());
	}

	void drawHandState(KJoint joint) {
		handState(joint.getState());
		strokeWeight(5.0f + joint.getZ() * 8);
		point(joint.getX(), joint.getY(), joint.getZ());
	}

	void handState(int handState) {
		switch (handState) {
		case KinectPV2.HandState_Open:
			stroke(0, 255, 0);
			break;
		case KinectPV2.HandState_Closed:
			stroke(255, 0, 0);
			break;
		case KinectPV2.HandState_Lasso:
			stroke(0, 0, 255);
			break;
		case KinectPV2.HandState_NotTracked:
			stroke(100, 100, 100);
			break;
		}
	}

	public int getParentJoint(int joint) {
		switch (joint) {
		case KinectPV2.JointType_SpineBase:
			return KinectPV2.JointType_SpineBase;

		case KinectPV2.JointType_Neck:
			return KinectPV2.JointType_SpineShoulder;

		case KinectPV2.JointType_SpineShoulder:
			return KinectPV2.JointType_SpineBase;

		case KinectPV2.JointType_ShoulderLeft:
		case KinectPV2.JointType_ShoulderRight:
			return KinectPV2.JointType_SpineShoulder;

		case KinectPV2.JointType_HipLeft:
		case KinectPV2.JointType_HipRight:
			return KinectPV2.JointType_SpineBase;

		case KinectPV2.JointType_HandTipLeft:
		case KinectPV2.JointType_ThumbLeft:
			return KinectPV2.JointType_HandLeft;

		case KinectPV2.JointType_HandTipRight:
		case KinectPV2.JointType_ThumbRight:
			return KinectPV2.JointType_HandRight;
		}

		return ((int) joint - 1);
	}

	private PVector createEndPoint(PVector startP, PVector vec) {
		return new PVector(startP.x + vec.x, startP.y + vec.y, startP.z + vec.z);
	}

}