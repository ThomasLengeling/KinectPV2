
import KinectPV2.KinectPV2;
import KinectPV2.Skeleton;
import KinectPV2.FaceData;
import processing.core.*;



public class BodyTrackTest extends PApplet {

	private KinectPV2 kinect;

	Skeleton [] skeleton;
	
	FaceData [] faceData;
	
	
	public void setup() {
		size(512*4, 424*2, P3D);
		
		kinect = new KinectPV2(this);
		kinect.enableBodyTrackImg(true);
		kinect.enableDepthMaskImg(true);
		kinect.init();
	}
	
	public void draw() {
		background(255, 0, 0);
	
		image(kinect.getBodyTrackImage(), 0, 0);
		image(kinect.getDepthMaskImage(), 0, 424);
		
		kinect.generteBodyTrackUsers();
		
		for(int i = 0; i < 3; i++)
			image(kinect.getBodyTrackUser(i), 512 + 512*i, 0);
		
		for(int i = 0; i < 3; i++)
			image(kinect.getBodyTrackUser(i + 3), 512 + 512*i, 424);
		
		fill(0);
		text(kinect.getNumOfUsers(), 50, 50);
	}
	
}