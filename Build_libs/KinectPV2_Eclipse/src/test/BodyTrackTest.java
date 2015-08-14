package test;


import java.util.ArrayList;

import KinectPV2.KinectPV2;
import KinectPV2.KSkeleton;
import KinectPV2.FaceData;
import processing.core.*;



public class BodyTrackTest extends PApplet {

	private KinectPV2 kinect;

	KSkeleton [] skeleton;
	
	FaceData [] faceData;
	
	
	public void setup() {
		size(512*4, 424*2, P3D);
		
		kinect = new KinectPV2(this);
		kinect.enableBodyTrackImg(true);
		kinect.enableDepthMaskImg(true);
		kinect.init();
	}
	
	public void draw() {
		background(255);
	
		image(kinect.getBodyTrackImage(), 0, 0);
		image(kinect.getDepthMaskImage(), 0, 424);
		
		
		ArrayList<PImage> bodyTrackList = kinect.getBodyTrackUser();
		
		for(int i = 0; i < bodyTrackList.size(); i++){
			PImage bodyTrackImg = (PImage)bodyTrackList.get(i);
			if(i <= 2)
				image(bodyTrackImg, 512 + 512*i, 0);
			else
				image(bodyTrackImg, 512 + 512*(i - 3), 424);
		}
		
		
		fill(0);
		text(kinect.getNumOfUsers(), 50, 50);
		text(bodyTrackList.size(), 50, 70);
	}
	
}