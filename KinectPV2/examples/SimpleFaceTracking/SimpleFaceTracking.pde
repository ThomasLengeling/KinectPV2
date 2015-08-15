/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing

Simple Face tracking, up-to 6 users with mode identifier
*/


import KinectPV2.*;

KinectPV2 kinect;

FaceData [] faceData;

void setup() {
  size(1920, 1080, P2D);

  kinect = new KinectPV2(this);

  //for face detection based on the color Img
  kinect.enableColorImg(true);

  //for face detection base on the infrared Img
  kinect.enableInfraredImg(true);

  kinect.enableFaceDetection(true);

  kinect.init();
}

void draw() {
  background(0);

  kinect.generateFaceData();

  //draw face information obtained by the color frame
  pushMatrix();
  scale(0.5f);
  image(kinect.getColorImage(), 0, 0);
  getFaceMapColorData();
  popMatrix();


  //draw face information obtained by the infrared frame
  pushMatrix();
  translate(1920*0.5f, 0.0f);
  image(kinect.getInfraredImage(), 0, 0);
  getFaceMapInfraredData();
  popMatrix();


  fill(255);
  text("frameRate "+frameRate, 50, 50);
}


public void getFaceMapColorData() {
  //get the face data
  FaceData [] faceData =  kinect.getFaceData();

  for (int i = 0; i < faceData.length; i++) {
    if (faceData[i].isFaceTracked()) {

      //obtain the face data from the colo frame
      PVector [] facePointsColor = faceData[i].getFacePointsColorMap();

      KRectangle rectFace = faceData[i].getBoundingRectColor();

      FaceFeatures [] faceFeatures = faceData[i].getFaceFeatures();

      //nose position
      PVector nosePos = new PVector();
      noStroke();

      int col = getIndexColor(i);

      fill(col);

      //update the nose positions
      for (int j = 0; j < facePointsColor.length; j++) {
        if (j == KinectPV2.Face_Nose)
          nosePos.set(facePointsColor[j].x, facePointsColor[j].y);

        ellipse(facePointsColor[j].x, facePointsColor[j].y, 15, 15);
      }

      //Feature detection of the user
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

public void getFaceMapInfraredData() {
  FaceData [] faceData =  kinect.getFaceData();

  for (int i = 0; i < faceData.length; i++) {
    if (faceData[i].isFaceTracked()) {

      //get the face data from the infrared frame
      PVector [] facePointsInfrared = faceData[i].getFacePointsInfraredMap();

      KRectangle rectFace = faceData[i].getBoundingRectInfrared();

      FaceFeatures [] faceFeatures = faceData[i].getFaceFeatures();

      PVector nosePos = new PVector();
      noStroke();

      int col = getIndexColor(i);

      fill(col);
      for (int j = 0; j < facePointsInfrared.length; j++) {
        if (j == KinectPV2.Face_Nose)
          nosePos.set(facePointsInfrared[j].x, facePointsInfrared[j].y);

        ellipse(facePointsInfrared[j].x, facePointsInfrared[j].y, 15, 15);
      }

      //Feature detection of the user
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


color getIndexColor(int index) {
  color col = color(255);
  if (index == 0)
    col = color(255, 0, 0);
  if (index == 1)
    col = color(0, 255, 0);
  if (index == 2)
    col = color(0, 0, 255);
  if (index == 3)
    col = color(255, 255, 0);
  if (index == 4)
    col = color(0, 255, 255);
  if (index == 5)
    col = color(255, 0, 255);

  return col;
}

//Face properties
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
