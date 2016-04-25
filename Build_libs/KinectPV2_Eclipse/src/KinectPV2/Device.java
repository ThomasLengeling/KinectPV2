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
import java.nio.IntBuffer;
import java.util.ArrayList;

import com.jogamp.common.nio.Buffers;

import processing.core.PApplet;
import processing.core.PImage;
import processing.core.PVector;

/**
 * Initilice Device
 *
 * @author Thomas Sanchez Lengeling
 *
 */
public class Device implements Constants, FaceProperties, SkeletonProperties,
		Runnable {

	static {
		int arch = Integer.parseInt(System.getProperty("sun.arch.data.model"));
		String platformName = System.getProperty("os.name");
		platformName = platformName.toLowerCase();
		System.out.println(arch + " " + platformName);
		if (arch == 64) {
			System.loadLibrary("Kinect20.Face");
			System.loadLibrary("KinectPV2");
			System.out.println("Loading KinectV2");
		} else {
			System.out.println("not compatible with 32bits");
		}
	}

	// IMAGES
	private Image colorImg;
	private Image depthImg;
	private Image depth256Img;
	private Image infraredImg;
	private Image infraredLongExposureImg;
	private Image bodyTrackImg;
	private Image []  bodyTrackUsersImg;
	private Image depthMaskImg;


	private Image pointCloudDepthImg;

	// SKELETON
	private KSkeleton[] skeletonDepth;
	private KSkeleton[] skeleton3d;
	private KSkeleton[] skeletonColor;

	private HDFaceData[] HDFace;

	private FaceData[] faceData;

	protected boolean runningKinect;
	protected boolean stopDevice;

	FloatBuffer pointCloudDepthPos;


	FloatBuffer pointCloudColorPos;
	FloatBuffer colorChannelBuffer;

	//color buffers
	IntBuffer   depthColorBuffer;
    IntBuffer   irColorBuffer;
    IntBuffer   registeredColorBuffer;

	private PApplet parent;
	private long ptr;

	private boolean startSensor;

	private String Version = "0.7.8";

	/**
	 * Start device
	 *
	 * @param _p
	 *            PApplet
	 */
	public Device(PApplet _p) {
		parent = _p;
		// SETUP IMAGES
		colorImg = new Image(parent, WIDTHColor, HEIGHTColor, PImage.ARGB);
		depthImg = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);
		depth256Img = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);
		infraredImg = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.ALPHA);

		bodyTrackImg = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.RGB);
		depthMaskImg = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.RGB);

		bodyTrackUsersImg = new Image[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			bodyTrackUsersImg[i] = new Image(parent, WIDTHDepth, HEIGHTDepth, PImage.RGB);
		}

		infraredLongExposureImg = new Image(parent, WIDTHDepth, HEIGHTDepth,
				PImage.ALPHA);

		pointCloudDepthImg = new Image(parent, WIDTHDepth, HEIGHTDepth,
				PImage.ALPHA);

		pointCloudDepthPos = Buffers.newDirectFloatBuffer(WIDTHDepth
				* HEIGHTDepth * 3);

		pointCloudColorPos = Buffers.newDirectFloatBuffer(WIDTHColor
				* HEIGHTColor * 3);
		colorChannelBuffer = Buffers.newDirectFloatBuffer(WIDTHColor
				* HEIGHTColor * 3);

		// SETUP SKELETON
		skeletonDepth = new KSkeleton[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			skeletonDepth[i] = new KSkeleton();
		}

		skeleton3d = new KSkeleton[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			skeleton3d[i] = new KSkeleton();
		}

		skeletonColor = new KSkeleton[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			skeletonColor[i] = new KSkeleton();
		}

		// SETUP FACEDATA
		faceData = new FaceData[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			faceData[i] = new FaceData();
		}

		HDFace = new HDFaceData[BODY_COUNT];
		for (int i = 0; i < BODY_COUNT; i++) {
			HDFace[i] = new HDFaceData();
		}

		//colors
		depthColorBuffer       = Buffers.newDirectIntBuffer(WIDTHDepth * HEIGHTDepth);
		irColorBuffer          = Buffers.newDirectIntBuffer(WIDTHDepth * HEIGHTDepth);
		registeredColorBuffer  = Buffers.newDirectIntBuffer(WIDTHDepth * HEIGHTDepth);

		startSensor = false;

		jniDevice();

	}

	protected void initDevice() {
		startSensor = jniInit();

		if (startSensor == false) {
			System.out.println("ERROR STARTING KINECT V2");
			parent.exit();
		}
		
		System.out.println("Version: " + Version);

		if (startSensor) {
			runningKinect = true;
			(new Thread(this)).start();
		}
	}
	
	/**
	 * Check if device is open 
	 * @return

	public boolean isDeviceStarted(){
		return startSensor;
	}
	 */
	
	public void closeDevice(){
		runningKinect = false;
		stopDevice();
		cleanDevice();
	}

	// IMAGES
	/**
	 * Get Color Image as PImage 1920 x 1080
	 *
	 * @return PImage
	 */
	public PImage getColorImage() {
		int[] colorData = jniGetColorData();
		PApplet.arrayCopy(colorData, 0, colorImg.pixels(), 0,
				colorImg.getImgSize());
		colorImg.updatePixels();

        PApplet.arrayCopy(colorData, 0, colorImg.rawIntData, 0,
                colorImg.getImgSize());


		return colorImg.img;
	}


	public int [] getRawColor(){
		return colorImg.rawIntData;
	}

	/**
	 * Get Depth Image as PImage 512 x 424
	 *
	 * @return PImage
	 */
	public PImage getDepthImage() {
		int [] depthData = jniGetDepth16Data();
		PApplet.arrayCopy(depthData, 0, depthImg.pixels(), 0,
				depthImg.getImgSize());
		depthImg.updatePixels();

		return depthImg.img;
	}

	/**
	 * Get Depth 256 strip data 512 x 424
	 * @return PImage
	 */
	public PImage getDepth256Image() {
		int[] depth256Data = jniGetDepth256Data();
		PApplet.arrayCopy(depth256Data, 0, depth256Img.pixels(), 0,
				depth256Img.getImgSize());
		depth256Img.updatePixels();

		// jniDepthReadyCopy(true);
		return depth256Img.img;
	}

	/**
	 * Obtain the raw depth data values in mm from 0 to 4500
	 * @return array of int
	 */
	public int []  getRawDepthData(){
		return jniGetRawDepth16Data();
	}


	/**
	 * Obtain the raw depth data values in mm from 0 to 256
	 * Data based on the  getDepth256Image
	 * @return array of int
	 */
	public int []  getRawDepth256Data(){
		return jniGetRawDepth256Data();
	}


	/**
	 * Get Depth Mask Image, outline color of the users.
	 *
	 * @return PImage
	 */
	public PImage getDepthMaskImage() {
		int[] depthMaskData = jniGetDepthMask();
		PApplet.arrayCopy(depthMaskData, 0, depthMaskImg.pixels(), 0,
				depthMaskImg.getImgSize());
		depthMaskImg.updatePixels();

		// jniDepthReadyCopy(true);
		return depthMaskImg.img;
	}

	/**
	 * Get InfraredImage as PImage 512 x 424
	 *
	 * @return PImage
	 */
	public PImage getInfraredImage() {
		int[] infraredData = jniGetInfraredData();
		PApplet.arrayCopy(infraredData, 0, infraredImg.pixels(), 0,
				infraredImg.getImgSize());
		infraredImg.updatePixels();

		return infraredImg.img;
	}

	/**
	 * Get BodyTracking as PImage 512 x 424
	 *
	 * @return PImage
	 */
	public PImage getBodyTrackImage() {
		int[] bodyTrackData = jniGetBodyTrack();
		PApplet.arrayCopy(bodyTrackData, 0, bodyTrackImg.pixels(), 0,
				bodyTrackImg.getImgSize());
		bodyTrackImg.updatePixels();

		return bodyTrackImg.img;
	}

	/**
	 * Get Independent Body Index Track
	 *
	 * @param index
	 * @return
	 */
	public ArrayList getBodyTrackUser() {
		ArrayList<PImage> listBodyTack = new ArrayList<PImage>(0);
		int [] usersIds = jniGetBodyTrackIds();

		for(int i = 0; i < usersIds.length; i++){
			if( usersIds[i] == 1){
				int[] rawData = jniGetBodyIndexUser(i);
				PApplet.arrayCopy(rawData, 0, bodyTrackUsersImg[i].pixels(), 0, bodyTrackUsersImg[i].getImgSize());
				bodyTrackUsersImg[i].updatePixels();
				listBodyTack.add(bodyTrackUsersImg[i].img);
			}
		}
		return listBodyTack;
	}



	/**
	 * Get the Number of currently track users based on the Body Track frame
	 * @return Number of Users
	 */
	public int getNumOfUsers(){
		return jniGetNumberOfUsers();
	}

	/**
	 * Get Long Exposure Infrared Image as PImage 512 x 424
	 *
	 * @return PImage
	 */
	public PImage getInfraredLongExposureImage() {
		int[] longExposureData = jniGetInfraredLongExposure();
		PApplet.arrayCopy(longExposureData, 0,
				infraredLongExposureImg.pixels(), 0,
				infraredLongExposureImg.getImgSize());
		infraredLongExposureImg.updatePixels();

		return infraredLongExposureImg.img;
	}

	/**
	 * Get Skeleton as Joints with Positions and Tracking states in 3D, (x,y,z)
	 * joint and orientation, Skeleton up to 6 users
	 *
	 * @return Skeleton []
	 */
	public ArrayList<KSkeleton> getSkeleton3d() {
		ArrayList<KSkeleton> arraySkeleton = new ArrayList<KSkeleton>();
		float[] rawData = jniGetSkeleton3D();
		for (int i = 0; i < BODY_COUNT; i++) {
			int indexJoint = i * (JointType_Count+1) * 9 + (JointType_Count+1) * 9 - 1;
			if(rawData[indexJoint] == 1.0){
				skeleton3d[i].createSkeletonData(rawData, i);
				arraySkeleton.add(skeleton3d[i]);
			}
		}
		return arraySkeleton;
	}

	/**
	 * Get Skeleton as Joints with Positions and Tracking states base on Depth
	 * Image, Skeleton with only (x, y) skeleton position mapped to the depth
	 * Image, get z value from the Depth Image.
	 *
	 * @return Skeleton []
	 */
	public ArrayList<KSkeleton> getSkeletonDepthMap() {
		ArrayList<KSkeleton> arraySkeleton = new ArrayList<KSkeleton>();
		float[] rawData = jniGetSkeletonDepth();
		for (int i = 0; i < BODY_COUNT; i++) {
			int indexJoint = i * (JointType_Count+1) * 9 + (JointType_Count+1) * 9 - 1;
			if(rawData[indexJoint] == 1.0){
				skeletonDepth[i].createSkeletonData(rawData, i);
				arraySkeleton.add(skeletonDepth[i]);
			}
		}
		return arraySkeleton;
	}

	/**
	 * Get Skeleton as Joints with Positions and Tracking states base on color
	 * Image,
	 *
	 * @return Skeleton []
	 */
	public ArrayList<KSkeleton> getSkeletonColorMap() {
		ArrayList<KSkeleton> arraySkeleton = new ArrayList<KSkeleton>();
		float[] rawData = jniGetSkeletonColor();
		for (int i = 0; i < BODY_COUNT; i++) {
			int indexJoint = i * (JointType_Count+1) * 9 + (JointType_Count+1) * 9 - 1;
			if(rawData[indexJoint] == 1.0){
				skeletonColor[i].createSkeletonData(rawData, i);
				arraySkeleton.add(skeletonColor[i]);
			}
		}
		return arraySkeleton;
	}

	// FACE DATA

	/**
	 * Generate Face Data for color map and infrared map
	 */
	public void generateFaceData() {
		float[] rawFaceColorData = jniGetFaceColorData();
		float[] rawFaceInfraredData = jniGetFaceInfraredData();

		for (int i = 0; i < BODY_COUNT; i++)
			faceData[i].createFaceData(rawFaceColorData, rawFaceInfraredData, i);

	}

	/**
	 * Obtain Vertex positions corresponding the HD Color frame
	 * @return
	 */
	public ArrayList<HDFaceData> getHDFaceVertex() {
		ArrayList<HDFaceData> HDFArray = new ArrayList<HDFaceData>();
		float[] rawData = jniGetHDFaceDetection();
		for (int i = 0; i < BODY_COUNT; i++){
			HDFace[i].createHDFaceVertexData(rawData, i);
			if(HDFace[i].isTracked()){
				HDFArray.add(HDFace[i]);
			}
		}
		return HDFArray;
	}

	/**
	 * Obtain the face data, 5 face points and mode detection from each user
	 * @return ArrayList of FaceData
	 */
	public ArrayList<FaceData>  getFaceData() {
		ArrayList<FaceData> faceArray = new ArrayList<FaceData>();

		for (int i = 0; i < BODY_COUNT; i++){
			if(faceData[i].isFaceTracked()){
				faceArray.add(faceData[i]);
			}
		}
		return faceArray;
	}

	// POINT CLOUDS

	/**
	 * Get Point Cloud Depth Map as FloatBuffer, transform to a float array with .array(), or get values with get(index)
	 *
	 * @return FloatBuffer
	 */
	public FloatBuffer getPointCloudDepthPos() {
		float[] pcRawData = jniGetPointCloudDeptMap();
		pointCloudDepthPos.put(pcRawData, 0, WIDTHDepth * HEIGHTDepth * 3);
		pointCloudDepthPos.rewind();

		return pointCloudDepthPos;
	}

	/**
	 * Get Point Cloud Color Positions as a FloatBuffer, transform to a float array with .array(), or get values with get(index)
	 * @return FloatBuffer
	 */
	public FloatBuffer getPointCloudColorPos() {
		float[] pcRawData = jniGetPointCloudColorMap();
		pointCloudColorPos.put(pcRawData, 0, WIDTHColor * HEIGHTColor * 3);
		pointCloudColorPos.rewind();

		return pointCloudColorPos;
	}

	/**
	 * Get the color channel buffer, 3 channels, 1920 x 1080 x 3 from [0-1]
	 * transform to a float array with .array(), or get values with get(index)
	 * Ideal method for load level openGL calls
	 * @return FloatBuffer
	 */
	public FloatBuffer getColorChannelBuffer() {
		float[] pcRawData = jniGetColorChannel();
		colorChannelBuffer.put(pcRawData, 0, WIDTHColor * HEIGHTColor * 3);
		colorChannelBuffer.rewind();

		return colorChannelBuffer;
	}


	/**
	 * Enable point cloud capture
	 * @param toggle
	 */
	public void enablePointCloud(boolean toggle) {
		jniEnablePointCloud(toggle);
	}

	/**
	 * Get Point cloud Depth Image
	 * @return PImage
	 */
	public PImage getPointCloudDepthImage() {
		int[] rawData = jniGetPointCloudDepthImage();
		PApplet.arrayCopy(rawData, 0, pointCloudDepthImg.pixels(), 0,
				pointCloudDepthImg.getImgSize());
		pointCloudDepthImg.updatePixels();

		return pointCloudDepthImg.img;
	}

	/**
	 * Set Threshold Depth Value Z for Point Cloud
	 *
	 * @param float val
	 */
	public void setLowThresholdPC(int val) {
		jniSetLowThresholdDepthPC(val);
	}

	/**
	 * Get Threshold Depth Value Z from Point Cloud Default 1.9
	 *
	 * @return default Threshold
	 */
	public int getLowThresholdDepthPC() {
		return jniGetLowThresholdDepthPC();
	}

	/**
	 * Set Threshold Depth Value Z for Point Cloud
	 *
	 * @param float val
	 */
	public void setHighThresholdPC(int val) {
		jniSetHighThresholdDepthPC(val);
	}

	/**
	 * Get Threshold Depth Value Z from Point Cloud Default 1.9
	 *
	 * @return default Threshold
	 */
	public int getHighThresholdDepthPC() {
		return jniGetHighThresholdDepthPC();
	}


	/**
	 * Get Raw BodyTracking Data 512 x 424
	 *
	 * @return int []
	 */
	public int[] getRawBodyTrack() {
		return jniGetRawBodyTrack();
	}


	/**
	 * Enable or Disable Color Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableColorImg(boolean toggle) {
		jniEnableColorFrame(toggle);
	}

	/**
	 * Enable or disable color Point cloud.
	 * Which is used to obtain getPointCloudColorPos() and getColorChannelBuffer();
	 * The FloatBuffer getColorChannelBuffer is a 3 independent color channels of 1920 x 1080 x 3,
	 * Values form between 0 and 1,  ideally for openGL calls
	 *
	 * @param toggle
	 */
	public void enableColorPointCloud(boolean toggle) {
		jniEnableColorChannel(toggle);
	}

	/**
	 * Enable or Disable Depth Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableDepthImg(boolean toggle) {
		jniEnableDepthFrame(toggle);
	}

	/**
	 * Enable or Disable DepthMask Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableDepthMaskImg(boolean toggle) {
		jniEnableDepthMaskFrame(toggle);
	}

	/**
	 * Enable or Disable Infrared Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableInfraredImg(boolean toggle) {
		jniEnableInfraredFrame(toggle);
	}

	/**
	 * Enable or Disable BodyTrack Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableBodyTrackImg(boolean toggle) {
		jniEnableBodyTrackFrame(toggle);
	}

	/**
	 * Enable or Disable LongExposure Infrared Image Capture
	 *
	 * @param boolean toggle
	 */
	public void enableInfraredLongExposureImg(boolean toggle) {
		jniEnableInfraredLongExposure(toggle);
	}

	/**
	 * Enable or Disable Skeleton Depth Map Capture
	 *
	 * @param boolean toggle
	 */
	public void enableSkeletonDepthMap(boolean toggle) {
		jniEnableSkeletonDepth(toggle);
	}

	/**
	 * Enable or Disable Skeleton Color Map Capture
	 *
	 * @param boolean toggle
	 */
	public void enableSkeletonColorMap(boolean toggle) {
		jniEnableSkeletonColor(toggle);
	}

	/**
	 * Enable or Disable Skeleton 3D Map Capture
	 *
	 * @param boolean toggle
	 */
	public void enableSkeleton3DMap(boolean toggle) {
		jniEnableSkeleton3D(toggle);
	}

	/**
	 * Enable or Disable Face Tracking
	 *
	 * @param boolean toggle
	 */
	public void enableFaceDetection(boolean toggle) {
		jniEnableFaceDetection(toggle);
	}

	/**
	 * Enable HDFace detection
	 *
	 * @param toggle
	 */
	public void enableHDFaceDetection(boolean toggle) {
		jniEnableHDFaceDetection(toggle);
	}

	public void  enableCoordinateMapperRGBDepth(boolean toggle){
		jniEnableCoordinateMapperRGBDepth();
	}

	/*
	 * public void enableMirror(boolean toggle){ jniSetMirror(toggle); }
	 */

	//MAPPERS
	public PVector MapCameraPointToDepthSpace(PVector pos){
		float [] rawData = jniMapCameraPointToDepthSpace(pos.x, pos.y, pos.z);
		return new PVector(rawData[0], rawData[1]);
	}

	public PVector MapCameraPointToColorSpace(PVector pos){
		float [] rawData = jniMapCameraPointToColorSpace(pos.x, pos.y, pos.z);
		return new PVector(rawData[0], rawData[1]);
	}

	public float [] getMapDepthToColor(){
		return jniGetMapDethToColorSpace();
	}


	protected boolean updateDevice() {
		boolean result = jniUpdate();
		return result;
	}

	protected void stopDevice() {
		jniStopDevice();
	}

	protected boolean cleanDevice() {
		return jniStopSignal();
	}

	// ------JNI FUNCTIONS
	private native void 	jniDevice();

	private native boolean 	jniInit();

	private native String 	jniVersion();

	private native boolean 	jniUpdate();

	// STOP
	private native void 	jniStopDevice();

	private native boolean 	jniStopSignal();

	// ENABLE FRAMES
	private native void 	jniEnableColorFrame(boolean toggle);

	private native void 	jniEnableDepthFrame(boolean toggle);

	private native void 	jniEnableDepthMaskFrame(boolean toggle);

	private native void 	jniEnableInfraredFrame(boolean toggle);

	private native void 	jniEnableBodyTrackFrame(boolean toggle);

	private native void 	jniEnableInfraredLongExposure(boolean toggle);

	private native void 	jniEnableSkeletonDepth(boolean toggle);

	private native void 	jniEnableSkeletonColor(boolean toggle);

	private native void 	jniEnableSkeleton3D(boolean toggle);

	private native void 	jniEnableFaceDetection(boolean toggle);

	private native void 	jniEnableHDFaceDetection(boolean toggle);

	private native void 	jniEnablePointCloud(boolean toggle);


	// COLOR CHANNEL
	private native void     jniEnableColorChannel(boolean toggle);

	private native float[] 	jniGetColorChannel();


	private native int[] 	jniGetColorData();


	// DEPTH
	private native int[] 	jniGetDepth16Data();

	private native int[] 	jniGetDepth256Data();

	//DEPTH RAW

	private native int[] 	jniGetRawDepth16Data();

	private native int[] 	jniGetRawDepth256Data();


	private native int[] 	jniGetInfraredData();

	private native int[] 	jniGetInfraredLongExposure();

	private native int[] 	jniGetBodyTrack();

	private native int[] 	jniGetDepthMask();

	private native float[] 	jniGetSkeleton3D();

	private native float[] 	jniGetSkeletonDepth();

	private native float[] 	jniGetSkeletonColor();

	private native float[] 	jniGetFaceColorData();

	private native float[] 	jniGetFaceInfraredData();

	private native float[] 	jniGetHDFaceDetection();

	// POINT CLOUD
	private native float[] 	jniGetPointCloudDeptMap();

	private native float[] 	jniGetPointCloudColorMap();

	private native int[] 	jniGetPointCloudDepthImage();

	// PC THRESHOLDS
	private native void 	jniSetLowThresholdDepthPC(int val);

	private native int 		jniGetLowThresholdDepthPC();

	private native void 	jniSetHighThresholdDepthPC(int val);

	private native int     	jniGetHighThresholdDepthPC();

	// BODY INDEX
	private native void     jniSetNumberOfUsers(int index);

	private native int[] 	jniGetBodyIndexUser(int index);

	private native int[]   	jniGetBodyTrackIds();

	private native int[]    jniGetRawBodyTrack();

	private native int      jniGetNumberOfUsers();
	//crists

	//MAPERS
	private native float[]	jniMapCameraPointToDepthSpace(float camaraSpacePointX, float cameraSpacePointY, float cameraSpacePointZ);

	private native float[]  jniMapCameraPointToColorSpace(float camaraSpacePointX, float cameraSpacePointY, float cameraSpacePointZ);

	private native float[]  jniGetMapDethToColorSpace();

	private native void     jniEnableCoordinateMapperRGBDepth();

	public void run() {
		int fr = PApplet.round(1000.0f / parent.frameRate);
		while (runningKinect) {
			// boolean result = updateDevice();

			// if(!result){
			// System.out.println("Error updating Kinect EXIT");
			// }
			try {
				Thread.sleep(fr); // 2
			} catch (InterruptedException e) {
				e.printStackTrace();
				return;
			}
		}

	}
}
