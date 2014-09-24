package KinectPV2;

/*
Copyright (C) 2014  Thomas Sanchez Lengeling.
KinectPV2, Kinect for Windows v2 library for processing

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

import java.nio.FloatBuffer;

import com.jogamp.common.nio.Buffers;

import processing.core.PApplet;
import processing.core.PImage;

/**
 * Initilice Device
 * @author Thomas Sanchez Lengeling
 *
 */
public class Device implements Constants, FaceProperties, SkeletonProperties, Runnable {
		
	static { 
		int arch = Integer.parseInt(System.getProperty("sun.arch.data.model"));
		String platformName = System.getProperty("os.name");
		platformName = platformName.toLowerCase();
		System.out.println(arch +" "+platformName);
		if (arch == 64) {
			System.loadLibrary("Kinect20.Face");
			System.loadLibrary("KinectPV2");
			System.out.println("Loading KinectV2");
		} else {
			System.out.println("error loading 32bits");
		}
	}
	
	private  Image colorImg;
	private  Image depthImg;
	private  Image infraredImg;
	private  Image longExposureImg;
	private  Image bodyTrackImg;
	private  Image depthMaskImg;
	
	private Skeleton   [] skeletonDepth;
	private Skeleton   [] skeleton3d;
	private Skeleton   [] skeletonColor;
	
	private FaceData   [] faceData; 
	
	FloatBuffer pointCloudPos;
	FloatBuffer pointCloudColor;
	
	protected boolean runningKinect;
	
	private PApplet parent;
	private long ptr;
	
	private boolean startSensor;
	
	
	/**
	 * Start device
	 * @param _p PApplet
	 */
	public Device(PApplet _p){
		parent = _p;
		colorImg        = new Image(parent, WIDTHColor, HEIGHTColor, PImage.ARGB);
		depthImg        = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);
		infraredImg     = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);
		
		bodyTrackImg    = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.RGB);
		depthMaskImg    = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.RGB);
		
		longExposureImg = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);
		
		skeletonDepth 		= new Skeleton[BODY_COUNT];
		for(int i = 0; i < BODY_COUNT; i++){
			skeletonDepth[i] = new Skeleton();
		}
		
		skeleton3d 		= new Skeleton[BODY_COUNT];
		for(int i = 0; i < BODY_COUNT; i++){
			skeleton3d[i] = new Skeleton();
		}
		
		skeletonColor 	= new Skeleton[BODY_COUNT];
		for(int i = 0; i < BODY_COUNT; i++){
			skeletonColor[i] = new Skeleton();
		}
		
		
		faceData 		= new FaceData[BODY_COUNT];
		for(int i = 0; i < BODY_COUNT; i++){
			faceData[i] = new FaceData();
		}
		
		pointCloudPos   = Buffers.newDirectFloatBuffer(WIDTHDepth * HEIGHTDepth * 3);
		
		pointCloudColor = Buffers.newDirectFloatBuffer(WIDTHColor * HEIGHTColor * 3);
				
		//FloatBuffer.allocate( WIDTHDepth * HEIGHTDepth * 3);
		startSensor = false;
		jniDevice();
		
	}
	
	protected void initDevice(){
		startSensor = jniInit();
		if(startSensor == false){
			System.out.println("ERROR STARTING KINECT V2");
			parent.exit();
		}
		
		if(startSensor){
			runningKinect = true;
			(new Thread(this)).start();
		}
		
		String load = jniVersion();
		System.out.println("Version: "+load);
	}
	
	//COPY IMAGES TYPES FROM JNI FUNTIONS
	private void copyColorImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, colorImg.pixels(), 0, colorImg.getImgSize());
		colorImg.updatePixels();
			
		if(colorImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, colorImg.rawIntData, 0, colorImg.getImgSize());
	}
	
	private void copyDepthImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, depthImg.pixels(), 0, depthImg.getImgSize());
		depthImg.updatePixels();
		if(depthImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, depthImg.rawIntData, 0, depthImg.getImgSize());
	}
	
	private void copyDepthMaskImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, depthMaskImg.pixels(), 0, depthMaskImg.getImgSize());
		depthMaskImg.updatePixels();
		if(depthMaskImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, depthMaskImg.rawIntData, 0, depthMaskImg.getImgSize());
		
	}
	
	private void copyInfraredImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, infraredImg.pixels(), 0, infraredImg.getImgSize());
		infraredImg.updatePixels();
			
		if(infraredImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, infraredImg.rawIntData, 0, infraredImg.getImgSize());
	}
	
	private void copyBodyTrackImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, bodyTrackImg.pixels(), 0, bodyTrackImg.getImgSize());
		bodyTrackImg.updatePixels();
		
		if(bodyTrackImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, bodyTrackImg.rawIntData, 0, bodyTrackImg.getImgSize());
	}
	
	private void copyLongExposureImg(int [] rawData){
		PApplet.arrayCopy(rawData, 0, longExposureImg.pixels(), 0, longExposureImg.getImgSize());
		longExposureImg.updatePixels();
		
		if(longExposureImg.isProcessRawData())
			PApplet.arrayCopy(rawData, 0, longExposureImg.rawIntData, 0, longExposureImg.getImgSize());
	}
	
	//SKELETON DEPTH
	private void copySkeletonDepthData(float [] rawData){
		if(rawData.length == JOINTSIZE){
			for(int i = 0; i < BODY_COUNT; i++){
				skeletonDepth[i].createSkeletonDepth(rawData, i);
			}
		}
	}
	
	//Color Data 
	private void copySkeletonColorData(float [] rawData){
		if(rawData.length == JOINTSIZE)
			for(int i = 0; i < BODY_COUNT; i++){
				skeletonColor[i].createSkeletonDepth(rawData, i);
			}
	}
	
	//SKELETON 3D
	private void copySkeleton3DData(float [] rawData){
		if(rawData.length == JOINTSIZE) {
			for(int i = 0; i < BODY_COUNT; i++){
				skeleton3d[i].createSkeleton3D(rawData, i);
			}
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
	
	private void copyFaceRawData(float [] rawData){
		if(rawData.length == FACESIZE){
			for(int i = 0; i < BODY_COUNT; i++)
				faceData[i].createFaceData(rawData, i);
		}
	}
	
	/**
	 * Get Point Cloud Depth Map as FloatBuffer
	 * @return FloatBuffer
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
	 * Get Face Data, up to 6 users
	 * @return Array of FaceData 
	 */
	public FaceData [] getFaceData(){
		return faceData;
	}
	
	/**
	 * Get Skeleton as Joints with Positions and Tracking states
	 * in 3D, (x,y,z) joint and orientation,  Skeleton up to 6 users
	 * @return Skeleton []
	 */
	public Skeleton [] getSkeleton3d(){
		return skeleton3d;
	}
	
	/**
	 * Get Skeleton as Joints with Positions and Tracking states
	 * base on Depth Image, Skeleton with only (x, y) skeleton position mapped
	 * to the depth Image, get z value from the Depth Image.
	 * @return Skeleton []
	 */
	public Skeleton [] getSkeletonDepthMap(){
		return skeletonDepth;
	}
	
	/**
	 * Get Skeleton as Joints with Positions and Tracking states
	 * base on color Image, 
	 * @return Skeleton []
	 */
	public Skeleton [] getSkeletonColorMap(){
		return skeletonColor;
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
	 * get Depth Mask Image, outline color of the users.
	 * @return PImage
	 */
	public PImage getDepthMaskImage(){
		return depthMaskImg.img;
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
	 * Get Raw DepthMask Data
	 *  512 x 424
	 * @return int []
	 */
	public int [] getRawDepthMask(){
		return depthMaskImg.rawIntData;
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
	 * @param boolean toggle 
	 */
	public void activateRawColor(boolean toggle){
		colorImg.activateRawData(toggle);
	}
	
	/**
	 * Activate Raw Depth Image Capture
	 * Use getRawDepth() Method
	 * @param boolean toggle 
	 */
	public void activateRawDepth(boolean toggle){
		depthImg.activateRawData(toggle);
	}
	
	/**
	 * Activate Raw Depth Image Capture
	 * Use getDepthMaskRaw() Method
	 * @param boolean toggle 
	 */
	public void activateRawDepthMaskImg(boolean toggle){
		depthMaskImg.activateRawData(toggle);
	}
	
	/**
	 * Activate Raw Infrared Image Capture
	 * Use getRawInfrared() Method
	 * @param boolean toggle 
	 */
	public void activateRawInfrared(boolean toggle){
		infraredImg.activateRawData(toggle);
	}
	
	/**
	 * Activate Raw BodyTrack Image Capture
	 * Use getRawBodyTrack() Method
	 * @param boolean toggle 
	 */
	public void activateRawBodyTrack(boolean toggle){
		bodyTrackImg.activateRawData(toggle);
	}
	/**
	 * Activate Raw LongExposureInfrared Image Capture
	 * use getRawLongExposure() method
	 * @param boolean toggle 
	 */
	public void activateRawLongExposure(boolean toggle){
		longExposureImg.activateRawData(toggle);
	}
	
	/**
	 * Enable or Disable Color Image Capture
	 * @param boolean toggle 
	 */
	public void enableColorImg(boolean toggle){
		jniEnableColorFrame(toggle);
	}
	
	/**
	 * Enable or Disable Depth Image Capture
	 * @param boolean toggle 
	 */
	public void enableDepthImg(boolean toggle){
		jniEnableDepthFrame(toggle);
	}
	
	
	
	/**
	 * Enable or Disable DepthMask Image Capture
	 * @param boolean toggle 
	 */
	public void enableDepthMaskImg(boolean toggle){
		jniEnableDepthMaskFrame(toggle);
	}
	
	/**
	 * Enable or Disable Infrared Image Capture
	 * @param boolean toggle 
	 */
	public void enableInfraredImg(boolean toggle){
		jniEnableInfraredFrame(toggle);
	}
	
	/**
	 * Enable or Disable BodyTrack Image Capture
	 * @param boolean toggle 
	 */
	public void enableBodyTrackImg(boolean toggle){
		jniEnableBodyTrackFrame(toggle);
	}
	
	/**
	 * Enable or Disable LongExposure Infrared Image Capture
	 * @param boolean toggle 
	 */
	public void enableLongExposureInfraredImg(boolean toggle){
		jniEnableLongExposureInfrared(toggle);
	}
	
	/**
	 * Enable or Disable Skeleton tracking
	 * @param boolean toggle 
	 */
	public void enableSkeleton(boolean toggle){
		jniEnableSkeleton(toggle);
	}
	
	/**
	 * Enable or Disable Skeleton tracking Color Map
	 * @param boolean toggle 
	 */
	public void enableSkeletonColorMap(boolean toggle){
		jniEnableSkeletonColorMap(toggle);
	}
	
	/**
	 * Enable or Disable Skeleton tracking 3d Map
	 * @param boolean toggle 
	 */
	public void enableSkeleton3dMap(boolean toggle){
		jniEnableSkeleton3dMap(toggle);
	}
	
	/**
	 * Enable or Disable Skeleton tracking Depth Map
	 * @param boolean toggle 
	 */
	public void enableSkeletonDepthMap(boolean toggle){
		jniEnableSkeletonDepthMap(toggle);
	}
	
	
	/**
	 * Enable or Disable Face Tracking
	 * @param boolean toggle 
	 */
	public void enableFaceDetection(boolean toggle){
		jniEnableFaceDetection(toggle);
	}
	
	
	/**
	 * Enable or Disable Point Cloud from Depth 
	 * @param boolean toggle 
	 */
	public void enablePointCloud(boolean toggle){
		jniEnablePointCloud(toggle);
	}
	
	/**
	 * Set Threshold Depth Value Z for Point Cloud
	 * @param float val
	 */
	public void setThresholdPointCloud(float val){
		jniThresholdDepthPointCloud(val);
	}
	
	/**
	 * Get Threshold Depth Value Z from Point Cloud
	 * Default 1.9
	 * @return default Threshold
	 */
	public float  getThresholdDepthPointCloud(){
		return jniGetDefaultThresholdDepthPointCloud();
	}
	
	/*
	public void enablePointCloudColor(boolean toggle){
		jniEnablePointCloudColor(toggle);
	}
	*/
	
	/*
	public void enableMirror(boolean toggle){
		jniSetMirror(toggle);
	}
	*/

	protected boolean updateDevice(){
		boolean result =  jniUpdate();
		return result;
	}
	

	protected void stopDevice(){
		skeleton3d = null;
		skeletonDepth = null;
		skeletonColor = null;
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
	
	private native String jniVersion();
	
	private native boolean jniUpdate();
	
	private native void jniStopDevice();
	
	private native void jniEnableColorFrame(boolean toggle);
	
	private native void jniEnableDepthFrame(boolean toggle);
	
	private native void jniEnableDepthMaskFrame(boolean toggle);
	
	private native void jniEnableInfraredFrame(boolean toggle);
	
	private native void jniEnableBodyTrackFrame(boolean toggle);
	
	private native void jniEnableLongExposureInfrared(boolean toggle);
	
	
	private native void jniEnableSkeleton(boolean toggle);
	
	private native void jniEnableSkeletonColorMap(boolean toggle);
	
	private native void jniEnableSkeletonDepthMap(boolean toggle);
	
	private native void jniEnableSkeleton3dMap(boolean toggle);
	
	
	private native void jniEnableFaceDetection(boolean toggle);
	
	private native void jniEnablePointCloud(boolean toggle);
	
	private native void jniEnablePointCloudColor(boolean toggle);
	
	private native void jniThresholdDepthPointCloud(float val);
	
	private native float jniGetDefaultThresholdDepthPointCloud();
	

	public void run() {
		//int fr = PApplet.round(1000.0f / parent.frameRate);
		while (runningKinect) {
			boolean result = updateDevice();
			if(!result){
				System.out.println("Error updating Kinect EXIT");
			}
			try {
				Thread.sleep(2); //2
			} catch (InterruptedException e) {
					e.printStackTrace();
			}
		}
		
	}
}
