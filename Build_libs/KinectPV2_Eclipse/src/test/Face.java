package test;

import java.util.ArrayList;

import KinectPV2.FaceFeatures;
import KinectPV2.KinectPV2;
import KinectPV2.FaceData;
import KinectPV2.KRectangle;
import processing.core.*;



public class Face extends PApplet {

	private KinectPV2 kinect;
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.Face"});
	}
	
	public void settings(){
		size(1920, 1080, P3D);
	}
		
	
	public void setup() {		
		  kinect = new KinectPV2(this);
		  kinect.enableColorImg(true);
		  kinect.enableInfraredImg(true);
		  kinect.enableFaceDetection(true);

		  kinect.init();
	}
	
	public void draw() {
		  background(0);
		  
		  kinect.generateFaceData();

		  pushMatrix();
		  scale(0.5f);
		  image(kinect.getColorImage(), 0, 0);
		  getFaceMapColorData();
		  popMatrix();
		 
		  
		  
		  pushMatrix();
		  translate(1920*0.5f, 0.0f);
		  image(kinect.getInfraredImage(), 0, 0);
		  getFaceMapInfraredData();
		  popMatrix();
		  
		  
		  fill(255);
		  text("frameRate "+frameRate, 50, 50);
		 
	}
	
	public void getFaceMapColorData() {
		 ArrayList<FaceData> faceData =  kinect.getFaceData();

		  for (int i = 0; i < faceData.size(); i++) {
			  FaceData faceD = faceData.get(i);
		    if (faceD.isFaceTracked()) {
		      PVector [] facePointsColor = faceD.getFacePointsColorMap();

		      KRectangle rectFace = faceD.getBoundingRectColor();

		      FaceFeatures [] faceFeatures = faceD.getFaceFeatures();

		      PVector nosePos = new PVector();
		      noStroke();

		      int col = getIndexColor(i);
		      
		      //head orientation
		      float pitch = faceD.getPitch();
		      float yaw   = faceD.getYaw();
		      float roll  = faceD.getRoll();
		      
		      //System.out.println("processing "+pitch+" "+yaw+" "+roll);

		      fill(col);   
		      for (int j = 0; j < facePointsColor.length; j++) {
		        if (j == KinectPV2.Face_Nose)
		          nosePos.set(facePointsColor[j].x, facePointsColor[j].y);

		        ellipse(facePointsColor[j].x, facePointsColor[j].y, 15, 15);
		      }

		      if (nosePos.x != 0 && nosePos.y != 0){
		        for (int j = 0; j < 8; j++) {
		          int st   = faceFeatures[j].getState();
		          int type = faceFeatures[j].getFeatureType();
	
		          String str = getStateTypeAsString(st, type);
	
		          fill(255);
		          text(str, nosePos.x + 150, nosePos.y - 70 + j*25);
		        }
		        stroke(255, 0, 0);
		        noFill();
		        rect(rectFace.getX(), rectFace.getY(), rectFace.getWidth(), rectFace.getHeight());
		    }
		  }
		}

	}
	
	  public void getFaceMapInfraredData(){
		  ArrayList<FaceData> faceData =  kinect.getFaceData();

		  for (int i = 0; i < faceData.size(); i++) {
			  FaceData faceD = faceData.get(i);
			  if (faceD.isFaceTracked()) {
				  PVector [] facePointsInfrared = faceD.getFacePointsInfraredMap();

				  KRectangle rectFace = faceD.getBoundingRectInfrared();

			      FaceFeatures [] faceFeatures = faceD.getFaceFeatures();
	
			      PVector nosePos = new PVector();
			      noStroke();
	
			      int col = getIndexColor(i);
	
			      fill(col);   
			      for (int j = 0; j < facePointsInfrared.length; j++) {
			        if (j == KinectPV2.Face_Nose)
			          nosePos.set(facePointsInfrared[j].x, facePointsInfrared[j].y);
	
			        ellipse(facePointsInfrared[j].x, facePointsInfrared[j].y, 15, 15);
			      }
	
			      if (nosePos.x != 0 && nosePos.y != 0)
			        for (int j = 0; j < 8; j++) {
			          int st   = faceFeatures[j].getState();
			          int type = faceFeatures[j].getFeatureType();
	
			          String str = getStateTypeAsString(st, type);
	
			          fill(255);
			          text(str, nosePos.x + 150, nosePos.y - 70 + j*25);
			        }
			      stroke(255, 0, 0);
			      noFill();
			      rect(rectFace.getX(), rectFace.getY(), rectFace.getWidth(), rectFace.getHeight());
			  }
		  }

	  }


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


		String getStateTypeAsString(int state, int type) {
		  String  str ="";
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

		  switch(state) {
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

		  return str;
		}
}