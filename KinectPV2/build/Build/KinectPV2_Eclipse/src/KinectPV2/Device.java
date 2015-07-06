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
	
	private  Image colorImg;
	private  Image depthImg;
	private  Image infraredImg;
	private  Image longExposureImg;
	private  Image bodyTrackImg;
	
	private Skeleton [] skeleton;
	
	protected boolean runningKinect;
	
	private PApplet parent;
	private long ptr;
	
	public Device(PApplet _p){
		parent = _p;
		colorImg        = new Image(parent, WIDTHColor, HEIGHTColor, parent.ARGB);
		depthImg        = new Image(parent, WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		infraredImg     = new Image(parent, WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		bodyTrackImg    = new Image(parent, WIDTHDepth, HEIGHTDepth, parent.RGB);
		longExposureImg = new Image(parent, WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		
		skeleton 		= new Skeleton[BODY_COUNT];
		for(int i = 0; i < BODY_COUNT; i++){
			skeleton[i] = new Skeleton(parent);
		}
		
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
			if(depthImg.processRawData)
				PApplet.arrayCopy(rawData, 0, depthImg.rawIntData, 0, depthImg.getImgSize());
			
		//}
	}
	
	void copyInfraredImg(int [] rawData){
		//if(rawData.length == infraredImg.getImgSize() && rawData != null ){
			PApplet.arrayCopy(rawData, 0, infraredImg.pixels(), 0, infraredImg.getImgSize());
			infraredImg.updatePixels();
		//}
	}
	
	void copyBodyTrackImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, bodyTrackImg.pixels(), 0, bodyTrackImg.getImgSize());
		bodyTrackImg.updatePixels();
	}
	
	void copyLongExposureImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, longExposureImg.pixels(), 0, longExposureImg.getImgSize());
		longExposureImg.updatePixels();
	}
	
	void copySkeletonRawData(float [] rawData){
		//PApplet.arrayCopy(rawData, 0, skeleton.getData(), 0, skeleton.skeletonSize);
		for(int i = 0; i < BODY_COUNT; i++){
			skeleton[i].createSkeletons(rawData, i);
		}
	}
	
	public Skeleton [] getSkeleton(){
		return skeleton;
	}
	
	public void windowSizeSkeleton(int width, int height){
		jniSetWindowSizeSkeleton(width, height);
	}
	
	//IMAGES
	public PImage getColorImage(){
		return colorImg.img;
	}
	
	public PImage getDepthImage(){
		return depthImg.img;
	}
	
	public PImage getInfraredImage(){
		return infraredImg.img;
	}
	
	public PImage getBodyTrackImage(){
		return bodyTrackImg.img;
	}
	
	public PImage getLongExposureInfrared(){
		return longExposureImg.img;
	}
	
	//RAW DATA
	public int [] getRawDepth(){
		return depthImg.rawIntData;
	}
	
	public int [] getRawColor(){
		return colorImg.rawIntData;
	}
	
	public int [] getRawInfrared(){
		return infraredImg.rawIntData;
	}
	
	public int [] getRawBodyTrack(){
		return bodyTrackImg.rawIntData;
	}
	
	public int [] getRawLongExposure(){
		return longExposureImg.rawIntData;
	}
	
	//ACTIVATE RAW DATA
	public void activateRawDepth(boolean toggle){
		depthImg.processRawData = toggle;
	}
	
	public boolean updateDevice(){
		boolean result =  jniUpdate();
		return result;
	}
	
	public void enableColorImg(boolean toggle){
		jniEnableColorFrame(toggle);
	}
	
	public void enableDepthImg(boolean toggle){
		jniEnableDepthFrame(toggle);
	}
	
	public void enableInfraredImg(boolean toggle){
		jniEnableInfraredFrame(toggle);
	}
	
	public void enableBodyTrackImg(boolean toggle){
		jniEnableBodyTrackFrame(toggle);
	}
	
	public void enableLongExposureInfrared(boolean toggle){
		jniEnableLongExposureInfrared(toggle);
	}
	
	public void enableSkeleton(boolean toggle){
		jniEnableSkeleton(toggle);
	}
	

	public void stopDevice(){
		skeleton = null;
		colorImg = null;
		depthImg = null;
		infraredImg = null;
		longExposureImg = null;
		bodyTrackImg= null;
		jniStopDevice();
	}
	
	//------JNI FUNCTIONS
	private native void jniDevice();
	
	private native boolean jniInit();
	
	//for Skeleton
	private native void jniSetWindowSizeSkeleton(int width, int height);
	
	private native String jniVersion();
	
	private native boolean jniUpdate();
	
	private native void jniStopDevice();
	
	private native void jniEnableColorFrame(boolean toggle);
	
	private native void jniEnableDepthFrame(boolean toggle);
	
	private native void jniEnableInfraredFrame(boolean toggle);
	
	private native void jniEnableBodyTrackFrame(boolean toggle);
	
	private native void jniEnableLongExposureInfrared(boolean toggle);
	
	private native void jniEnableSkeleton(boolean toggle);
	
	private native void jniGetSkeleton();


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
