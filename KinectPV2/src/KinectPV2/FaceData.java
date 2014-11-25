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
	
	PVector [] facePointsColor;
	PVector [] facePointsInfrared;
	
	FaceFeatures [] facefeatures;
	
	boolean faceTracked;
	
	Rectangle rect;
	
	float pitch;
	float yaw;
	float roll;
	
	FaceData(){
		facePointsColor = new PVector[5];
		for(int i = 0; i < facePointsColor.length; i++)
			facePointsColor[i] =  new PVector();
		
		facePointsInfrared = new PVector[5];
		for(int i = 0; i < facePointsInfrared.length; i++)
			facePointsInfrared[i] =  new PVector();
		
		rect = new Rectangle(0, 0, 0, 0);
		faceTracked = false;
		
		facefeatures = new FaceFeatures[8];
		
		for(int i = 0; i < 8; i++){
			facefeatures[i] = new FaceFeatures(i, -1);
		}
	}

	
	protected void createFaceData(float [] rawData, int iFace){
		int index = iFace * 36;
		if(rawData[index + 35] == 0.0)
			faceTracked = false;
		else
			faceTracked = true;

		for(int i = 0; i < 5; i++){
			facePointsColor[i].x = rawData[index + i*2 + 0];
			facePointsColor[i].y = rawData[index + i*2 + 1];
		}
		
		for(int i = 0; i < 5; i++){
			facePointsInfrared[i].x = rawData[index + i*2 + 10 + 0];
			facePointsInfrared[i].y = rawData[index + i*2 + 10 + 1];
		}
		int index2 = index +20;
		rect.setX(rawData[index2 + 0]);
		rect.setY(rawData[index2 + 1]);
		rect.setWidth(rawData[index2 + 2]);
		rect.setHeight(rawData[index2 + 3]);
		
		pitch = rawData[index2 + 4];
		yaw   = rawData[index2 + 5];
		roll  = rawData[index2 + 6];
		
		//System.out.println(iFace+" "+pitch+" "+yaw+" "+roll);
		for(int i =0; i < 8; i++){
			facefeatures[i].setFeatureType(i);
			facefeatures[i].setState((int)rawData[index2 + 7 + i]);
			//System.out.println(iFace+" "+facefeatures[i].getFeatureType()+" "+(int)rawData[index2 + 7 + i] );
		}
	}
	
	/**
	 * Get Face Features
	 * @return FaceFeatures []
	 */
	public FaceFeatures [] getFaceFeatures(){
		return facefeatures;
	}
	
	/**
	 * get Bounding Face Rectangle
	 * @return Rectangle
	 */
	public Rectangle getBoundingRect(){
		return rect;
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
		return facePointsColor;
	}
	
	/**
	 * get Face Points mapped to Infrared Space (InFrared Image)
	 * @return
	 */
	public PVector [] getFacePointsInfraredMap(){
		return facePointsInfrared;
	}
}
