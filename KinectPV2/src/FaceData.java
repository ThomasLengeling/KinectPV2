package KinectPV2;

/*
Copyright (C) 2014  Thomas Sanchez Lengeling.
KinectPV2, Kinect one library for processing

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

public class FaceData {
	
	PVector [] facePoints ;
	
	boolean faceTracked;
	
	Rectangle rect;
	
	FaceData(){
		facePoints = new PVector[5];
		for(int i = 0; i <facePoints.length; i++)
			facePoints[i] =  new PVector();
		
		rect = new Rectangle(0, 0, 0, 0);
		faceTracked = false;
	}

	
	void createFaceData(float [] rawData, int iFace){
		if(rawData[iFace * 26 + 25] == 0.0)
			faceTracked = false;
		else
			faceTracked = true;
	System.out.println(rawData[iFace * 26 + 25]+" iF "+iFace);
		//if(faceTracked){
			for(int i = 0; i < 5; i++){
				int index = iFace * 26 + i*2;
				facePoints[i].x = rawData[index + 0];
				facePoints[i].y = rawData[index + 1];
				//System.out.println(facePoints[i].x+" "+facePoints[i].y);
			}
			
			rect.setX(rawData[iFace * 26 + 10 + 0]);
			rect.setY(rawData[iFace * 26 + 10 + 1]);
			rect.setWidth(rawData[iFace * 26 + 10 + 2]);
			rect.setHeight(rawData[iFace * 26 + 10 + 3]);
		//}
	}
	
	public Rectangle getBoundingRect(){
		return rect;
	}
	
	public boolean isFaceTracked(){
		return faceTracked;
	}
	
	public PVector [] getFacePoints(){
		return facePoints;
	}
}
