import JNIKinect.PKinectV2;
import processing.core.*;

public class HelloWorld extends PApplet {

	private static final long serialVersionUID = 1L;
	private PKinectV2 kinect;

	public void setup() {
		size(1280, 720);
		background(0);
		
		println("Kinect");
		kinect = new PKinectV2(this);
		smooth();
	}

	public void draw() {
		background(0);
		image(kinect.GetImage(), 0, 0, 320, 240);
		image(kinect.GetDepth(), 0, 240, 320, 240);
		//image(img, 0, 240, 320, 240);
		image(kinect.GetMask(), 320, 0, 320, 240);
		
		text(frameRate, 50, 50);
	}

	public void mousePressed() {
		println(frameRate);
		saveFrame();
	}

}
