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

  //enable HD Face detection
  kinect.enableHDFaceDetection(true);
  kinect.enableColorImg(true); //to draw the color image
  kinect.init();
}

void draw() {
  background(0);

  // Draw the color Image
  image(kinect.getColorImage(), 0, 0);

  //Obtain the Vertex Face Points
  // 1347 Vertex Points for each user.
  ArrayList<HDFaceData> hdFaceData = kinect.getHDFaceVertex();

  for (int j = 0; j < hdFaceData.size(); j++) {
    //obtain a the HDFace object with all the vertex data
    HDFaceData HDfaceData = (HDFaceData)hdFaceData.get(j);

    if (HDfaceData.isTracked()) {

      //draw the vertex points
      stroke(0, 255, 0);
      beginShape(POINTS);
      for (int i = 0; i < KinectPV2.HDFaceVertexCount; i++) {
        float x = HDfaceData.getX(i);
        float y = HDfaceData.getY(i);
        vertex(x, y);
      }
      endShape();
    }
  }
}
