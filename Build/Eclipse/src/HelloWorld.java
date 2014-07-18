import KinectPV2.KinectV2;
import processing.core.PApplet;


public class HelloWorld extends PApplet {

	private KinectV2 kinect;

	int [] depthData;
	
	public void setup() {
		size(900, 768);
		
		kinect = new KinectV2(this);
		kinect.enableColorImg();
		kinect.enableDepthImg();
		//kinect.activateRawDepth(true);
		kinect.enableInfraredImg();
		
		kinect.init();
		
		depthData = new int[kinect.WIDTHDepth * kinect.WIDTHDepth ];
	}

	public void draw() {
		background(0);
		
		image(kinect.getColorImage(), 0, 0, 854, 480);
		image(kinect.getDepthImage(),70, 480, 320, 240);
		image(kinect.getInfraredImage(), 460, 480, 320, 240);
		
		depthData =  kinect.getRawDepth();
		
		//arrayCopy(kinect.getRawDepth(), 0, depthData, 0, kinect.WIDTHDepth* kinect.HEIGHTDepth);
	
		
		fill(255, 0, 0);
		text(frameRate, 50, 50);
	}

	public void mousePressed() {
		println(frameRate);
		saveFrame();
	}
	

}
