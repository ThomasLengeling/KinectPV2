import KinectPV2.KinectPV2;
import processing.core.PApplet;
import processing.core.PImage;


public class CoordinateMapperRGBDepth extends PApplet{
	
	KinectPV2 kinect;

	public void setup() {
	  size(1920, 1080);

	  kinect = new KinectPV2(this);
	  kinect.enableCoordinateMapperRGBDepth(true);

	  kinect.init();

	  //SETUP A BACKGROUND IMAGE
	  PImage img = loadImage("mars.png");
	  img.loadPixels();
	  kinect.setCoordBkgImg(img.pixels);
	}

	public void draw() {
		background(0);
		image(kinect.getCoordinateRGBDepthImage(), 0, 0);
	}
}
