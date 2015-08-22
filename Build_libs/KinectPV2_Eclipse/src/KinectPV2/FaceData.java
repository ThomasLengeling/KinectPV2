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

import processing.core.PVector;


/**
 * Face Data
 * P
 * @author thomas
 *
 */
public class FaceData implements FaceProperties{
	
	PVector [] faceColorPoints;
	PVector [] faceInfraredPoints;
	
	FaceFeatures [] facefeatures;
	
	boolean faceTracked;
	
	KRectangle rectColor;
	KRectangle rectInfrared;
	
	
	float pitch;
	float yaw;
	float roll;
	
	int userIndex;
	
	FaceData(){
		faceColorPoints = new PVector[5];
		for(int i = 0; i < faceColorPoints.length; i++)
			faceColorPoints[i] =  new PVector();
		
		faceInfraredPoints = new PVector[5];
		for(int i = 0; i < faceInfraredPoints.length; i++)
			faceInfraredPoints[i] =  new PVector();
		
		rectColor = new KRectangle(0, 0, 0, 0);
		rectInfrared = new KRectangle(0, 0, 0, 0);
		
		faceTracked = false;
		
		facefeatures = new FaceFeatures[8];
		
		for(int i = 0; i < 8; i++){
			facefeatures[i] = new FaceFeatures(i, -1);
		}
	}

	
	protected void createFaceData(float [] rawFaceColorData, float [] rawFaceInfraredData, int iFace){
		int index = iFace * 36;
		if(rawFaceColorData[index + 35] == 0.0 || rawFaceInfraredData[index +32] == 0.0)
			faceTracked = false;
		else
			faceTracked = true;
		
		if(faceTracked){
			userIndex = iFace;
			for(int i = 0; i < 5; i++){
				faceColorPoints[i].x = rawFaceColorData[index + i*2 + 0];
				faceColorPoints[i].y = rawFaceColorData[index + i*2 + 1];
				faceInfraredPoints[i].x = rawFaceInfraredData[index + i*2 + 0];
				faceInfraredPoints[i].y = rawFaceInfraredData[index + i*2 + 1];
			}
			
			int index2 = index +20;
			rectColor.setX(rawFaceColorData[index2 + 0]);
			rectColor.setY(rawFaceColorData[index2 + 1]);
			rectColor.setWidth(rawFaceColorData[index2 + 2]);
			rectColor.setHeight(rawFaceColorData[index2 + 3]);
			
			rectInfrared.setX(rawFaceInfraredData[index2 + 0]);
			rectInfrared.setY(rawFaceInfraredData[index2 + 1]);
			rectInfrared.setWidth(rawFaceInfraredData[index2 + 2]);
			rectInfrared.setHeight(rawFaceInfraredData[index2 + 3]);
			
			pitch = rawFaceColorData[index2 + 4];
			yaw   = rawFaceColorData[index2 + 5];
			roll  = rawFaceColorData[index2 + 6];
			
			pitch = rawFaceInfraredData[index2 + 4];
			yaw   = rawFaceInfraredData[index2 + 5];
			roll  = rawFaceInfraredData[index2 + 6];
			
			//System.out.println(iFace+" "+pitch+" "+yaw+" "+roll);
			for(int i =0; i < 8; i++){
				facefeatures[i].setFeatureType(i);
				facefeatures[i].setState((int)rawFaceColorData[index2 + 7 + i]);
				//System.out.println(iFace+" "+facefeatures[i].getFeatureType()+" "+(int)rawData[index2 + 7 + i] );
			}
		}
	}
	
	/**
	 * Get the pitch rotation of the face
	 * @return Pitch in degrees
	 */
	public float getPitch(){
		return pitch;
	}
	
	/**
	 * Get the yaw rotation of the face
	 * @return Yaw in degrees
	 */
	public float getYaw(){
		return yaw;
	}
	
	/**
	 * Get the roll rotation of the face
	 * @return Roll in degress
	 */
	public float getRoll(){
		return roll;
	}
	
	/**
	 * Get color of the user
	 * @return
	 */
	public int getIndexColor() {
	  int col = color(255, 255, 255);
	  if (userIndex == 0)
	    col = color(255, 0, 0);
	  if (userIndex == 1)
	    col = color(0, 255, 0);
	  if (userIndex == 2)
	    col = color(0, 0, 255);
	  if (userIndex == 3)
	    col = color(255, 255, 0);
	  if (userIndex == 4)
	    col = color(0, 255, 255);
	  if (userIndex == 5)
	    col = color(255, 0, 255);

	  return col;
	}
	
	private final int color(int v1, int v2, int v3) {
	      return 0xff000000 | (v1 << 16) | (v2 << 8) | v3;
	 }
	
	/**
	 * Get Face Features
	 * @return FaceFeatures []
	 */
	public FaceFeatures [] getFaceFeatures(){
		return facefeatures;
	}
	
	/**
	 * get Bounding Face Rectangle Color
	 * @return Rectangle
	 */
	public KRectangle getBoundingRectColor(){
		return rectColor;
	}
	
	/**
	 * get Bounding Face Rectangle Infrared
	 * @return Rectangle
	 */
	public KRectangle getBoundingRectInfrared(){
		return rectInfrared;
	}
	
	/**
	 * If Face is being Tracked
	 * @return boolean
	 */
	public boolean isFaceTracked(){
		return faceTracked;
	}
	
	/**
	 * get Face Points mapped to color Space (Color Image)
	 * @return PVector []
	 */
	public PVector [] getFacePointsColorMap(){
		return faceColorPoints;
	}
	
	/**
	 * get Face Points mapped to infrared Space (Infrared Image)
	 * @return PVector []
	 */
	public PVector [] getFacePointsInfraredMap(){
		return faceInfraredPoints;
	}
}
