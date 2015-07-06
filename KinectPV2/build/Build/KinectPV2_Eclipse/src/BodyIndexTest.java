
import KinectPV2.KinectPV2;
import KinectPV2.Skeleton;
import KinectPV2.FaceData;
import processing.core.*;



public class BodyIndexTest extends PApplet {

	private KinectPV2 kinect;

	Skeleton [] skeleton;
	
	FaceData [] faceData;
	
	
	public void setup() {
		size(512*4, 424*2, P3D);
		
		kinect = new KinectPV2(this);
		kinect.enableBodyTrackImg(true);
		kinect.init();
	}
	
	public void draw() {
		background(255);
	
		/*
		image(kinect.getBodyTrackImage(), 0, 424);
		for(int i = 0; i < 4; i++)
			image(kinect.getBodyIndexUser(i+1), 512*i, 0);
		
		for(int i = 0; i < 3; i++)
			image(kinect.getBodyIndexUser(i + 5), 512*(i+1), 424);
		*/
	}
	
	public void keyPressed() {
		if(key == '1') {
			kinect.setNumberOfUsers(1);
			println("1");
		}
		if(key == '2') {
			kinect.setNumberOfUsers(2);
			println("2");
		}
		if(key == '3') {
			kinect.setNumberOfUsers(3);
			println("3");
		}
	}
}