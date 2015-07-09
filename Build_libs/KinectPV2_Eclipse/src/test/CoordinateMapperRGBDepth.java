package test;

import KinectPV2.KinectPV2;
import processing.core.PApplet;
import processing.core.PImage;


public class CoordinateMapperRGBDepth extends PApplet{
	
	KinectPV2 kinect;
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.CoordinateMapperRGBDepth"});
	}
	
	public void settings(){
		size(1280, 720,  P3D);
	}
	

	public void setup() {

	  kinect = new KinectPV2(this);
	  kinect.enableCoordinateMapperRGBDepth(true);

	  kinect.init();

	  //SETUP A BACKGROUND IMAGE
	//  PImage img = loadImage("mars.png");
	//  img.loadPixels();
	 // kinect.setCoordBkgImg(img.pixels);
	}

	public void draw() {
		background(0);
		//image(kinect.getCoordinateRGBDepthImage(), 0, 0);
	}
}
