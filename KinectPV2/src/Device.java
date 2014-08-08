package KinectPV2;

import java.nio.FloatBuffer;

import com.jogamp.common.nio.Buffers;

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
	
	FloatBuffer pointCloudPos;
	FloatBuffer pointCloudColor;
	
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
		
		pointCloudPos   = Buffers.newDirectFloatBuffer(WIDTHDepth * HEIGHTDepth * 3);
		
		pointCloudColor = Buffers.newDirectFloatBuffer(WIDTHColor * HEIGHTColor * 3);
				
		//FloatBuffer.allocate( WIDTHDepth * HEIGHTDepth * 3);
		
		jniDevice();
		runningKinect = true;
		(new Thread(this)).start();
	}
	
	protected void initDevice(){
		boolean result = jniInit();
		if(result == false){
			System.out.println("ERROR STARTING KINECT V2");
			parent.exit();
		}
		
		String load = jniVersion();
		System.out.println("Version: "+load);
	}
	
	//COPY IMAGES TYPES FROM JNI FUNTIONS
	private void copyColorImg(int [] rawData){
		//if(rawData.length == colorImg.getImgSize() && rawData != null ){
			PApplet.arrayCopy(rawData, 0, colorImg.pixels(), 0, colorImg.getImgSize());
			colorImg.updatePixels();
			
			if(colorImg.processRawData)
				PApplet.arrayCopy(rawData, 0, colorImg.rawIntData, 0, colorImg.getImgSize());
		//}
	}
	
	private void copyDepthImg(int [] rawData){
		//if(rawData.length == depthImg.getImgSize() && rawData != null){
			PApplet.arrayCopy(rawData, 0, depthImg.pixels(), 0, depthImg.getImgSize());
			depthImg.updatePixels();
			//if(depthImg.processRawData)
			//	PApplet.arrayCopy(rawData, 0, depthImg.rawIntData, 0, depthImg.getImgSize());
			
		//}
	}
	
	private void copyInfraredImg(int [] rawData){
		//if(rawData.length == infraredImg.getImgSize() && rawData != null ){
			PApplet.arrayCopy(rawData, 0, infraredImg.pixels(), 0, infraredImg.getImgSize());
			infraredImg.updatePixels();
			
			if(infraredImg.processRawData)
				PApplet.arrayCopy(rawData, 0, infraredImg.rawIntData, 0, infraredImg.getImgSize());
		//}
	}
	
	private void copyBodyTrackImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, bodyTrackImg.pixels(), 0, bodyTrackImg.getImgSize());
		bodyTrackImg.updatePixels();
		
		
		if(bodyTrackImg.processRawData)
			PApplet.arrayCopy(rawData, 0, bodyTrackImg.rawIntData, 0, bodyTrackImg.getImgSize());
	}
	
	private void copyLongExposureImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, longExposureImg.pixels(), 0, longExposureImg.getImgSize());
		longExposureImg.updatePixels();
		
		if(longExposureImg.processRawData)
			PApplet.arrayCopy(rawData, 0, longExposureImg.rawIntData, 0, longExposureImg.getImgSize());
	}
	
	private void copySkeletonRawData(float [] rawData){
		//PApplet.arrayCopy(rawData, 0, skeleton.getData(), 0, skeleton.skeletonSize);
		if(rawData.length == JOINTSIZE)
		for(int i = 0; i < BODY_COUNT; i++){
			skeleton[i].createSkeletons(rawData, i);
		}
	}
	
	private void copyRawDepthImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, depthImg.rawIntData, 0, depthImg.getImgSize());
	}
	
	private void copyPointCloudPos(float [] rawData){
		if(rawData.length == WIDTHDepth * HEIGHTDepth * 3){
			pointCloudPos.put(rawData, 0, WIDTHDepth * HEIGHTDepth * 3);
			pointCloudPos.rewind();
		}
	}

	
	private void copyPointCloudColor(float [] rawData){
		if(rawData.length == WIDTHColor * HEIGHTColor * 3){
			pointCloudColor.put(rawData, 0, WIDTHColor * HEIGHTColor * 3);
			pointCloudColor.rewind();
		}
	}
	
	/**
	 * Get Point Cloud as FloatBuffer
	 * @return
	 */
	public FloatBuffer getPointCloudPosFloatBuffer(){
		return pointCloudPos;
	}
	
	/*
	public FloatBuffer getPointCloudFloatColorBuffer(){
		return pointCloudColor;
	}
	*/
	
	/**
	 * Get Skeleton as Joints with Positions and Tracking states 
	 * @return
	 */
	public Skeleton [] getSkeleton(){
		return skeleton;
	}
	
	public void windowSizeSkeleton(int width, int height){
		jniSetWindowSizeSkeleton(width, height);
	}
	
	//IMAGES
	/**
	 * Get Color Image as PImage
	 * 1920 x 1080
	 * @return PImage
	 */
	public PImage getColorImage(){
		return colorImg.img;
	}
	
	/**
	 * Get Depth Image as PImage
	 * 512 x 424
	 * @return PImage
	 */
	public PImage getDepthImage(){
		return depthImg.img;
	}
	
	/**
	 * Get InfraredImage as PImage
	 * 512 x 424
	 * @return PImage
	 */
	public PImage getInfraredImage(){
		return infraredImg.img;
	}
	
	/**
	 * Get BodyTracking as PImage
	 * 512 x 424
	 * @return PImage
	 */
	public PImage getBodyTrackImage(){
		return bodyTrackImg.img;
	}
	
	/**
	 * Get Long Exposure Infrared Image as PImage
	 * 512 x 424
	 * @return PImage
	 */
	public PImage getLongExposureInfraredImage(){
		return longExposureImg.img;
	}
	
	
	/**
	 * Get Raw Depth Data
	 *  512 x 424
	 * @return int []
	 */
	public int [] getRawDepth(){
		return depthImg.rawIntData;
	}
	
	/**
	 * Get Raw Color Data
	 * 1920 x 1080
	 * @return int []
	 */
	public int [] getRawColor(){
		return colorImg.rawIntData;
	}
	
	/**
	 * Get Raw Infrared Data
	 *  512 x 424
	 * @return int []
	 */
	public int [] getRawInfrared(){
		return infraredImg.rawIntData;
	}
	
	/**
	 * Get Raw BodyTracking Data
	 *  512 x 424
	 * @return int []
	 */
	public int [] getRawBodyTrack(){
		return bodyTrackImg.rawIntData;
	}
	
	/**
	 *  Get Raw LongExposure Data
	 *  512 x 424
	 * @return int []
	 */
	public int [] getRawLongExposure(){
		return longExposureImg.rawIntData;
	}
	
	//ACTIVATE RAW DATA
	/**
	 * Activate Raw Color Image Capture.
	 * Use getRawColor() Method
	 * @param toggle 
	 */
	public void activateRawColor(boolean toggle){
		colorImg.processRawData = toggle;
	}
	
	/**
	 * Activate Raw Depth Image Capture
	 * Use getRawDepth() Method
	 * @param toggle
	 */
	public void activateRawDepth(boolean toggle){
		depthImg.processRawData = toggle;
	}
	
	/**
	 * Activate Raw Infrared Image Capture
	 * Use getRawInfrared() Method
	 * @param toggle
	 */
	public void activateRawInfrared(boolean toggle){
		infraredImg.processRawData = toggle;
	}
	
	/**
	 * Activate Raw BodyTrack Image Capture
	 * Use getRawBodyTrack() Method
	 * @param toggle
	 */
	public void activateRawBodyTrack(boolean toggle){
		bodyTrackImg.processRawData = toggle;
	}
	/**
	 * Activate Raw LongExposureInfrared Image Capture
	 * use getRawLongExposure() method
	 * @param toggle
	 */
	public void activateRawLongExposure(boolean toggle){
		longExposureImg.processRawData = toggle;
	}
	
	/**
	 * Enable or Disable Color Image Capture
	 * @param toggle
	 */
	public void enableColorImg(boolean toggle){
		jniEnableColorFrame(toggle);
	}
	
	/**
	 * Enable or Disable Depth Image Capture
	 * @param toggle
	 */
	public void enableDepthImg(boolean toggle){
		jniEnableDepthFrame(toggle);
	}
	
	/**
	 * Enable or Disable Infrared Image Capture
	 * @param toggle
	 */
	public void enableInfraredImg(boolean toggle){
		jniEnableInfraredFrame(toggle);
	}
	
	/**
	 * Enable or Disable BodyTrack Image Capture
	 * @param toggle
	 */
	public void enableBodyTrackImg(boolean toggle){
		jniEnableBodyTrackFrame(toggle);
	}
	
	/**
	 * Enable or Disable LongExposure Infrared Image Capture
	 * @param toggle
	 */
	public void enableLongExposureInfraredImg(boolean toggle){
		jniEnableLongExposureInfrared(toggle);
	}
	
	/**
	 * Enable or Disable Skeleton tracking
	 * @param toggle
	 */
	public void enableSkeleton(boolean toggle){
		jniEnableSkeleton(toggle);
	}
	
	
	
	/**
	 * Enable or Disable Point Cloud from Depth 
	 * @param toggle
	 */
	public void enablePointCloud(boolean toggle){
		jniEnablePointCloud(toggle);
	}
	
	
	/**
	 * Set Threshold Depth Value Z for Point Cloud
	 * @param val
	 */
	public void setThresholdPointCloud(float val){
		jniThresholdDepthPointCloud(val);
	}
	
	/**
	 * Get Threshold Depth Value Z from Point Cloud
	 * Default 1.9
	 * @return
	 */
	public float  getThresholdDepthPointCloud(){
		return jniGetDefaultThresholdDepthPointCloud();
	}
	
	/*
	public void enablePointCloudColor(boolean toggle){
		jniEnablePointCloudColor(toggle);
	}
	*/

	protected boolean updateDevice(){
		boolean result =  jniUpdate();
		return result;
	}
	

	protected void stopDevice(){
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
	
	private native void jniEnablePointCloud(boolean toggle);
	
	private native void jniEnablePointCloudColor(boolean toggle);
	
	private native void jniThresholdDepthPointCloud(float val);
	
	private native float jniGetDefaultThresholdDepthPointCloud();


	public void run() {
		//int fr = PApplet.round(1000.0f / parent.frameRate);
		while (runningKinect) {
			boolean result = updateDevice();
			if(!result){
				System.out.println("Error updating Kinect");
			}
			try {
				Thread.sleep(2); //2
			} catch (InterruptedException e) {
				 e.printStackTrace();
			}
		}
		
	}
}
