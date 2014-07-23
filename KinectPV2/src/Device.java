package KinectPV2;

import processing.core.PApplet;
import processing.core.PImage;

public class Device implements Constants, Runnable {
		
	static {
		int arch = Integer.parseInt(System.getProperty("sun.arch.data.model"));
		String platformName = System.getProperty("os.name");
		platformName = platformName.toLowerCase();
		System.out.println(arch +" "+platformName);
		if (arch == 64) {
			System.loadLibrary("KinectPV2");
			System.out.println("Loading KinectV2");
		} else {
			System.loadLibrary("KinectPV2");
			System.out.println("ERROR CANOT LOAD 32bits ");
		}
	}
	
	public  Image colorImg;
	public  Image depthImg;
	public  Image infraredImg;
	
	protected boolean runningKinect;
	
	private PApplet parent;
	private long ptr;
	
	public Device(PApplet _p){
		parent = _p;
		colorImg    =   new Image(parent, WIDTHColor, HEIGHTColor, parent.ARGB);
		depthImg    =   new Image(parent, WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		infraredImg =   new Image(parent, WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		
		jniDevice();
		runningKinect = true;
		(new Thread(this)).start();
	}
	
	public void initDevice(){
		boolean result = jniInit();
		if(result == false){
			System.out.println("ERROR STARTING KINECT V2");
			parent.exit();
		}
		
		String load = jniVersion();
		System.out.println("Version: "+load);
	}
	
	//COPY IMAGES TYPES FROM JNI FUNTIONS
	void copyColorImg(int [] rawData){
		//if(rawData.length == colorImg.getImgSize() && rawData != null ){
			PApplet.arrayCopy(rawData, 0, colorImg.pixels(), 0, colorImg.getImgSize());
			colorImg.updatePixels();
		//}
	}
	
	void copyDepthImg(int [] rawData){
		//if(rawData.length == depthImg.getImgSize() && rawData != null){
			PApplet.arrayCopy(rawData, 0, depthImg.pixels(), 0, depthImg.getImgSize());
			depthImg.updatePixels();
			//if(depthImg.processRawData)
			//	PApplet.arrayCopy(rawData, 0, depthImg.rawIntData, 0, depthImg.getImgSize());
			
		//}
	}
	
	void copyInfraredImg(int [] rawData){
		//if(rawData.length == infraredImg.getImgSize() && rawData != null ){
			PApplet.arrayCopy(rawData, 0, infraredImg.pixels(), 0, infraredImg.getImgSize());
			infraredImg.updatePixels();
		//}
	}
	
	public void setWindowSize(int width, int height){
		jniSetWindowSize(width, height);
	}
	
	//IAMGES
	public PImage getColorImage(){
		return colorImg.img;
	}
	
	public PImage getDepthImage(){
		return depthImg.img;
	}
	
	public PImage getInfraredImage(){
		return infraredImg.img;
	}
	
	//RAW DATA
	public int [] getRawDepth(){
		return depthImg.rawIntData;
	}
	
	public void activateRawDepth(boolean toggle){
		depthImg.processRawData = toggle;
	}
	
	public boolean updateDevice(){
		boolean result =  jniUpdate();
		return result;
	}
	
	public void enableColorImg(){
		jniEnableColorFrame();
	}
	
	public void enableDepthImg(){
		jniEnableDepthFrame();
	}
	
	public void enableInfraredImg(){
		jniEnableInfraredFrame();
	}
	
	public void disableColorImg(){
		jniDisableColorFrame();
	}
	
	public void disableDepthImg(){
		jniDisableDepthFrame();
	}
	public void disableInfraredImg(){
		jniDisableInfraredFrame();
	}

	public void stopDevice(){
		jniStopDevice();
	}
	
	//------JNI FUNCTIONS
	private native void jniDevice();
	
	private native boolean jniInit();
	
	private native void jniSetWindowSize(int width, int height);
	
	private native String jniVersion();
	
	private native boolean jniUpdate();
	
	private native void jniStopDevice();
	
	private native void jniEnableColorFrame();
	
	private native void jniEnableDepthFrame();
	
	private native void jniEnableInfraredFrame();
	
	private native void jniDisableColorFrame();
	
	private native void jniDisableDepthFrame();
	
	private native void jniDisableInfraredFrame();


	public void run() {
		//int fr = PApplet.round(1000.0f / parent.frameRate);
		while (runningKinect) {
			boolean result = updateDevice();
			if(!result){
				System.out.println(result+" error");
			}
			try {
				Thread.sleep(2); //2
			} catch (InterruptedException e) {
				 e.printStackTrace();
			}
		}
		
	}
}
