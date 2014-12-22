import java.nio.FloatBuffer;

import KinectPV2.FaceFeatures;
import KinectPV2.KJoint;
import KinectPV2.KinectPV2;
import KinectPV2.Skeleton;
import KinectPV2.FaceData;
import KinectPV2.HDFaceData;
<<<<<<< HEAD
import KinectPV2.Rectangle;
=======
import KinectPV2.KRectangle;
>>>>>>> origin/dev
import processing.core.*;
import processing.opengl.PGL;
import processing.opengl.PJOGL;

import javax.media.opengl.GL2;

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

public class HelloWorld extends PApplet {

	private KinectPV2 kinect;

	int [] depthData;
	
	Skeleton [] skeleton;
	
	FaceData [] faceData;
	
	float a = 0;
	
	int zval = 50;
	
	float scaleVal = 360;
	
	float depthVal = 0;
	
	float zVal = 220;

	float rotY = 0;
	float rotZ = 0;
	float rotX = PI;
	
	float th   =1;
	
	float maxD = 4.0f; //meters
	float minD = 1.0f;
	
<<<<<<< HEAD
	public void setup() {
		size(1920, 1080, P3D);
		
=======
	boolean  activateMapDepthTable = false;
	
	PImage depthToColorImg;
	
	int [] depthZero;
	
	public void setup() {
		size(1920, 1080, P3D);
		
		depthToColorImg = createImage(512, 424, PImage.RGB);
		depthZero		= new int[512*424];
		
		 for(int i = 0; i < KinectPV2.WIDTHDepth; i++) {
			 for(int j = 0; j < KinectPV2.HEIGHTDepth; j++) {
				 depthZero[424*i + j] = 0;
			 }
		 }
			 
		
>>>>>>> origin/dev
		kinect = new KinectPV2(this);
		//kinect.enableCoordinateMapperRGBDepth(true);
		//kinect.enableColorImg(true);
		//kinect.enableDepthImg(true);
		//kinect.enableLongExposureInfraredImg(true);
		//kinect.enableInfraredImg(true);
		//kinect.enableBodyTrackImg(true);
		//kinect.enableDepthMaskImg(true);
		//kinect.enablePointCloud(true);
		
	    //kinect.enableDepthMaskImg(true);
		
		//depthVal = kinect.getThresholdDepthPointCloud();
		
		//kinect.enablePointCloudColor(false);
		//kinect.activateRawDepth(true);
		//kinect.activateRawDepthMaskImg(true);
		//kinect.enableBodyTrackImg(true);
	    //kinect.activateRawDepth(true);
		//kinect.enableBodyTrackImg(true);
		//
<<<<<<< HEAD
		
	//	kinect.enableFaceDetection(true);
		kinect.enableHDFaceDetection(true);
=======
		kinect.enableDepthImg(true);
		kinect.enableColorImg(true);
		kinect.activateRawDepth(true);
		kinect.activateRawColor(true);
	//	kinect.enableFaceDetection(true);
		//kinect.enablePointCloudColor(true);
	//	kinect.activateRawColor(true);
		//kinect.enableHDFaceDetection(true);
>>>>>>> origin/dev
		
		//kinect.enableSkeleton(true);
		//kinect.enableSkeletonColorMap(true);
		//kinect.enableSkeletonDepthMap(true);
		
		//kinect.enableSkeleton3dMap(true);
		
	//	kinect.enablePointCloud(true);
		
		//kinect.skeletonMapToDimentions(1920, 1080);
		
		kinect.init();
		
		//PImage img = loadImage("C:/Users/thomas/Documents/Programacion/Java/Workspace/KinectPV2/data/mars.png");
		//img.loadPixels();
		//kinect.setCoordBkgImg(img.pixels);

		//depthData = new int[kinect.WIDTHDepth * kinect.HEIGHTDepth ];
	}

	public void draw() {
		background(0);
<<<<<<< HEAD
		//image(kinect.getColorImage(), 0, 0);
		
=======
		
		// image(kinect.getColorImage(), 0, 0, 320, 240);
		
		 float [] mapDCT = kinect.getMapDepthToColor();
		 
		 int [] colorRaw = kinect.getRawColor();
		 int [] depthRaw = kinect.getRawDepth();
		 
		 int count = 0;
		 int step =1;
		 
		 PApplet.arrayCopy(depthZero, depthToColorImg.pixels);
			
		 depthToColorImg.loadPixels();
		 for(int i = 0; i < KinectPV2.WIDTHDepth; i+=step) {
			 for(int j = 0; j < KinectPV2.HEIGHTDepth; j+=step) {
				 float valX = mapDCT[count*2 + 0];
				 float valY = mapDCT[count*2 + 1];
				 
				 int valXDepth = (int)((valX/1920.0)*512.0);
				 int valYDepth = (int)((valY/1080.0)*424.0);
				 
				 int  valXColor = (int)(valX);
				 int  valYColor = (int)(valY);
				 
				 //512*i + j
				 

			 //     depthToColorImg.pixels[424*i + j] = 77;

				if(	valXDepth >= 0 && valXDepth < 512 && valYDepth >= 0 && valYDepth < 424 ) {
					if(valXColor >= 0 && valXColor < 1920 && valYColor >= 0 && valYColor < 1080) {
					    int color = colorRaw[valYColor*1920 + valXColor];
						//int color = depthRaw[valYDepth*512 + valXDepth];
						// depthToColorImg.pixels[valYDepth*512 + valXDepth] = depthRaw[valYDepth*512 + valXDepth];
						 
						depthToColorImg.pixels[valYDepth*512 + valXDepth] = color;
					}
				}
				count++;
			 }
		 }
		 depthToColorImg.updatePixels();
		 
		 image(depthToColorImg, 0, 424);
		 image(kinect.getColorImage(), 0, 0, 512, 424);
		 image(kinect.getDepthImage(), 512, 0);
		 

		  //Threahold of the point Cloud.
		 // kinect.setLowThresholdPC(minD);
		//  kinect.setHighThresholdPC(maxD);

		  FloatBuffer pointCloudBuffer = kinect.getPointCloudColorPos();
		  FloatBuffer colorBuffer = kinect.getColorChannelBuffer();
		  
		  PJOGL pgl = (PJOGL)beginPGL();
		  GL2 gl2 = pgl.gl.getGL2();

		  gl2.glEnable( GL2.GL_BLEND );
		 // gl2.glEnable(GL2.GL_POINT_SMOOTH);      

		  gl2.glEnableClientState(GL2.GL_VERTEX_ARRAY);
		  gl2.glEnableClientState(GL2.GL_COLOR_ARRAY);
		  gl2.glColorPointer(3, GL2.GL_FLOAT, 0, colorBuffer);
		  gl2.glVertexPointer(3, GL2.GL_FLOAT, 0, pointCloudBuffer);

		  gl2.glTranslatef(width/2, height/2, zval);
		  gl2.glScalef(scaleVal, -1*scaleVal, scaleVal);
		  gl2.glRotatef(a, 0.0f, 1.0f, 0.0f);

		  gl2.glDrawArrays(GL2.GL_POINTS, 0, kinect.WIDTHColor * kinect.HEIGHTColor);

		  gl2.glDisableClientState(GL2.GL_VERTEX_ARRAY);
		  gl2.glDisableClientState(GL2.GL_COLOR_ARRAY);
		  gl2.glDisable(GL2.GL_BLEND);
		  endPGL();

		  
		//image(kinect.getColorImage(), 0, 0);
	/*	
>>>>>>> origin/dev
		HDFaceData []  hdFaceData = kinect.getHDFaceVertex();
		
		stroke(0, 255, 0);
		
		for(int j = 0; j < KinectPV2.BODY_COUNT; j++) {
			beginShape(POINTS);
			if(hdFaceData[j].isTracked()) {
				for(int i = 0; i < KinectPV2.HDFaceVertexCount; i++) {
					float x = hdFaceData[j].getX(i);
					float y = hdFaceData[j].getY(i);
					vertex(x, y);
				}
			}
			endShape();
		}
<<<<<<< HEAD
		
=======
	*/
>>>>>>> origin/dev
		
		//image(kinect.getCoordinateRGBDepthImage(), 0, 0);

		//image(kinect.getColorImage(), 0, 0);
		//image(kinect.getDepthMaskImage(), 0, 0);
		//image(kinect.getDepthImage(), 512, 0);
	//	image(kinect.getBodyTrackImage(), 512, 0);
		
		//image(kinect.getColorImage(), 0, 0);
		//image(kinect.getDepthImage(), 0, 0);
		//image(kinect.getLongExposureInfraredImage(), 0, 0);
		 /*
		image(kinect.getDepthImage(), 0, 0);
	 	image(kinect.getPointCloudDepthImage(), 512, 0);
	 	
	 	kinect.setLowThresholdPC(minD);
	 	kinect.setHighThresholdPC(maxD);
	
	 	int []  dataRaw = kinect.getRawPointCloudDepth();
	 	
	 	for(int i = 0; i < 512; i++) {
	 		for(int j = 0; j < 424; j++) {
	 			int index = j*512 + i;
	 			int val = dataRaw[index];
	
	 			stroke(val);
	 			point(i + 512,j);
	 		}
	 	}
	 	
	 	//println(maxD+" "+minD);
	 //	image(kinect.getPointCloudDepthImage(), 512, 0);
		//image(kinect.getBodyTrackImage(), 0, 0);
	*/
	 	
	 	
		//image(kinect.getDepthImage(), 0, 0);
	
	/*
		faceData =  kinect.getFaceData();
		
		for(int i = 0; i < faceData.length; i++){
			if(faceData[i].isFaceTracked()){
				PVector [] facePointsColor = faceData[i].getFacePointsColorMap();
				
				Rectangle rectFace = faceData[i].getBoundingRect();
				
				FaceFeatures [] faceFeatures = faceData[i].getFaceFeatures();
				
				PVector nosePos = new PVector();
				noStroke();
				
				if(i == 0)
					fill(255, 0, 0);
				if(i == 1)
					fill(0, 255, 0);
				if(i == 2)
					fill(0, 0, 255);
				if(i == 3)
					fill(255, 255, 0);
				if(i == 4)
					fill(0, 255, 255);
				if(i == 5)
					fill(255, 0, 255);
				
				for(int j = 0; j < facePointsColor.length; j++){
					if(j == KinectPV2.Face_Nose)
						nosePos.set(facePointsColor[j].x, facePointsColor[j].y);
					
					ellipse(facePointsColor[j].x, facePointsColor[j].y, 15, 15);
				}
				
				if(nosePos.x != 0 && nosePos.y != 0)
				for(int j = 0; j < 8; j++){
					int st   = faceFeatures[j].getState();
					int type = faceFeatures[j].getFeatureType();
					
					//System.out.println(st+" "+type);
					
					String str = "";
					fill(255);
					switch(type) {
						case KinectPV2.FaceProperty_Happy:
							str = "Happy";
							break;
							
						case KinectPV2.FaceProperty_Engaged:
							str = "Engaged";
							break;
								
						case KinectPV2.FaceProperty_LeftEyeClosed:
							str = "LeftEyeClosed";
							break;
									
						case KinectPV2.FaceProperty_RightEyeClosed:
							str = "RightEyeClosed";
							break;
										
						case KinectPV2.FaceProperty_LookingAway:
							str = "LookingAway";
							break;
							
						case KinectPV2.FaceProperty_MouthMoved:
							str = "MouthMoved";
							break;
							
						case KinectPV2.FaceProperty_MouthOpen:
							str = "MouthOpen";
							break;
							
						case KinectPV2.FaceProperty_WearingGlasses:
							str = "WearingGlasses";
							break;
					}
					switch(st) {
						case KinectPV2.DetectionResult_Unknown:
							str += ": Unknown";
							break;
						case KinectPV2.DetectionResult_Yes:
							str += ": Yes";
							break;	
						case KinectPV2.DetectionResult_No:
							str += ": No";
							break;
					}
					

					text(str, nosePos.x + 150, nosePos.y - 70 + j*25);
				}
				stroke(255, 0, 0);
				noFill();
				rect(rectFace.getX(), rectFace.getY(), rectFace.getWidth(), rectFace.getHeight());
			}
		}
	


		//skeleton =  kinect.getSkeletonDepthMap();
		//skeleton =  kinect.getSkeletonColorMap();
		/*
		skeleton =  kinect.getSkeleton3d();

	 	  pushMatrix();

	 	 translate(width/2, height/2, 0);
	 	  scale(zVal);
	 	  rotateX(rotX);
	 	  rotateY(rotY);
	 	  rotateZ(rotZ);
	 	 

	 	  for (int i = 0; i < skeleton.length; i++) {
	 	    if (skeleton[i].isTracked()) {

	 	      KJoint[] joints = skeleton[i].getJoints();
	 	      drawBody(joints);
	 	      
	 	      if(joints[i].getType() == KinectPV2.JointType_Head)
	 	    	  System.out.println(joints[i].getX()+" "+joints[i].getY());//+" "+joints[i].getZ());

	 	      drawHandState(joints[KinectPV2.JointType_HandRight]);
	 	      drawHandState(joints[KinectPV2.JointType_HandLeft]);
	 	    }
	 	  }
	 	  popMatrix();
	 	  */

	 	  fill(255, 0, 0);
	 	  text(frameRate, 50, 550);
	 	}

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
		 	stroke(255, 100);
		 	strokeWeight(10.0f + joints[jointType].getZ()*8);
		 	point(joints[jointType].getX(), joints[jointType].getY(), joints[jointType].getZ());
	 	}

	 	void drawBone(KJoint[] joints, int jointType1, int jointType2) {
	 		stroke(255, 100);
	 		strokeWeight(10.0f + joints[jointType1].getZ()*8);
	 		point(joints[jointType2].getX(), joints[jointType2].getY(), joints[jointType2].getZ());

	 	  
	 	 // noStroke();
	 	 
	 	  /*
	 	  noFill();
	 	  strokeWeight(1.0f);
	 	  stroke(255, 0, 0, 100);
	 	  beginShape(LINES);
	 	  vertex(joints[jointType1].getX(), joints[jointType1].getY(), joints[jointType1].getZ());
	 	  vertex(joints[jointType2].getX(), joints[jointType2].getY(), joints[jointType2].getZ()); 
	 	  endShape();
	 	  */
	 	  
	 	// strokeWeight(1.0f);
	 	// stroke(255, 0, 0, 100);
	 	//  line(joints[jointType1].getX(), joints[jointType1].getY(), 0,
	 	//		 joints[jointType2].getX(), joints[jointType2].getY(), 0 );	 
	 	 
	 	}

	 	void drawHandState(KJoint joint) {
	 	  handState(joint.getState());
	 	  strokeWeight(15.0f + joint.getZ()*9);
	 	  point(joint.getX(), joint.getY(), joint.getZ());
	 	}

	 	void handState(int handState) {
	 	  switch(handState) {
	 	  case KinectPV2.HandState_Open:
	 	    stroke(0, 255, 0, 120);
	 	    break;
	 	  case KinectPV2.HandState_Closed:
	 		stroke(255, 0, 0, 120);
	 	    break;
	 	  case KinectPV2.HandState_Lasso:
	 		stroke(0, 0, 255, 120);
	 	    break;
	 	  case KinectPV2.HandState_NotTracked:
	 		stroke(100, 100, 100, 120);
	 	    break;
	 	  }
	 	}

	public void mousePressed() {
		
		println(frameRate);
		//saveFrame();
	}
	
	public void keyPressed(){
		if(key == 'a'){
			zval +=1;
			println(zval);
		}
		if(key == 's'){
			zval -= 1;
			println(zval);
		}
		
		if(key == 'z'){
			scaleVal += 0.1;
			println(scaleVal);
		}
		if(key == 'x'){
			scaleVal -= 0.1;
			println(scaleVal);
		}
		
		if(key == 'q'){
			a += 1;
			println(a);
		}
		if(key == 'w'){
			a -= 1;
			println(a);
		}
		
		if(key == 'c'){
			depthVal -= 0.01;
			println(depthVal);
		}
		
		if(key == 'v'){
			depthVal += 0.01;
			println(depthVal);
		}
		
		if(key == '1') {
			minD += 0.1;
			println("the min"+minD);
		}
		
		if(key == '2') {
			minD -= 0.1;
			println("the min"+minD);
		}
		
		if(key == '3') {
			maxD += 0.1;
			println("the max"+maxD);
		}
		
		if(key == '4') {
			maxD -= 0.1;
			println("the max"+maxD);
		}
		
	}

}
