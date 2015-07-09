/*
Thomas Sanchez Lengeling.
http://codigogenerativo.com/

KinectPV2, Kinect for Windows v2 library for processing
 
HD Face tracking.
Vertex Face positions are mapped to the Color Frame or to the Infrared Frame
*/
import KinectPV2.*;

KinectPV2 kinect;

void setup() {
  size(1920, 1080);
  
  kinect = new KinectPV2(this);
  kinect.enableHDFaceDetection(true);
  kinect.init();
}

void draw() {
  background(0);
  
 //DRAW COLOR IMAGE MAP
 // image(kinect.getColorImage(), 0, 0);
  
  HDFaceData []  hdFaceData = kinect.getHDFaceVertex();
  
  stroke(0, 255, 0);
  for (int j = 0; j < KinectPV2.BODY_COUNT; j++) {
    beginShape(POINTS);
    if (hdFaceData[j].isTracked()) {
      for (int i = 0; i < KinectPV2.HDFaceVertexCount; i++) {
        float x = hdFaceData[j].getX(i);
        float y = hdFaceData[j].getY(i);
        vertex(x, y);
      }
    }
    endShape();
  }
}