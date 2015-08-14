package test;

import KinectPV2.KinectPV2;
import KinectPV2.KSkeleton;
import KinectPV2.FaceData;
import processing.core.*;

public class TestImg extends PApplet {

	private KinectPV2 kinect;

	KSkeleton [] skeleton;
	
	FaceData [] faceData;
	
	
	public void setup() {
		size(512*3, 424*2, P3D);
		
		kinect = new KinectPV2(this);
		
		kinect.enableColorImg(true);
		kinect.enableDepthImg(true);
		kinect.enableInfraredImg(true);
		kinect.enableInfraredLongExposureImg(true);
		kinect.enableBodyTrackImg(true);

		kinect.init();
	}
	
	public void draw() {
		background(0);
		
		image(kinect.getDepthImage(), 0, 0, 512, 424);
		image(kinect.getDepth256Image(), 512, 0, 512, 424);
		
		image(kinect.getInfraredImage(), 0, 424, 512, 424);
		image(kinect.getInfraredLongExposureImage(), 512, 424, 512, 424);
		
		image(kinect.getColorImage(), 512*2, 424, 512, 424);
		
		
		fill(255, 0, 0);
		text(frameRate, 50, 50);
	}
}