import KinectPV2.KinectV2;
import KinectPV2.Skeleton;

import processing.core.*;

public class HelloWorld extends PApplet {

	private KinectV2 kinect;

	int [] depthData;
	
	Skeleton [] skeleton;
	
	public void setup() {
		size(512*3, 768, P3D);
		
		kinect = new KinectV2(this);
		//kinect.enableColorImg();
		kinect.enableDepthImg(true);
		kinect.enableBodyTrackImg(true);
	//	kinect.activateRawDepth(true);
		kinect.enableBodyTrackImg(true);
		kinect.enableLongExposureInfrared(true);
		kinect.enableInfraredImg(true);
		kinect.enableSkeleton(true);
		
		kinect.init();
		
		//depthData = new int[kinect.WIDTHDepth * kinect.HEIGHTDepth ];
	}

	public void draw() {
		background(0);
		
		//image(kinect.getColorImage(), 0, 0, 854, 480);
		image(kinect.getDepthImage(), 0, 0);
		image(kinect.getBodyTrackImage(), 512, 0);
		//image(kinect.getInfraredImage(), 512, 0);
		image(kinect.getLongExposureInfrared(), 512*2, 0);
		//image(kinect.getInfraredImage(), 460, 480, 320, 240);
		//image(kinect.getInfraredImage(), 460, 480, 320, 240);
		
		fill(255, 0, 0);
		
		skeleton = kinect.getSkeleton();
		
		for(int i = 0; i < skeleton.length; i++){
			if(skeleton[i].isTracked()){
				skeleton[i].drawBody();
				skeleton[i].drawHandStates();
			}
		}
		
		stroke(255, 0, 0);
		text(frameRate, 50, height- 50);
	}

	public void mousePressed() {
		println(frameRate);
		//saveFrame();
	}

}
