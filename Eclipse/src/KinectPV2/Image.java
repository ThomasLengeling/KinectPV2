package KinectPV2;

import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PImage;


public class Image{
	private float fps;
	
	
	private    PApplet parent;
	
	protected  PImage img;
	
	private int imgPixelSize;

	private boolean processImage = true;
	private byte []  rawByteData;
	private int  []  rawIntData;
	
	public Image(PApplet p, int width, int height, int MODE){
		parent = p;
		img =  parent.createImage(width, height, MODE);
		imgPixelSize = width * height;
		rawByteData = new byte[imgPixelSize];
		rawIntData  = new int[imgPixelSize];
	}
	
	public int getImgSize(){
		return imgPixelSize;
	}
	
	public void updatePixels(){
		img.updatePixels();
	}
	
	public int [] pixels(){
		return img.pixels;
	}
	
	public PImage getColorImage() {		
		return img;
	}
	
	public float getFPS() {
		return fps;
	}
}
