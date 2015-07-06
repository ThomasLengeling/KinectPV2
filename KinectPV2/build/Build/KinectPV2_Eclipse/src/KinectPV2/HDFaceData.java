package KinectPV2;

import processing.core.PVector;

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

/**
 * Face Features class, with type Feature and State.
 * @author Thomas Sanchez Lengeling
 *
 */
public class HDFaceData implements FaceProperties{
	
	PVector [] HDFaceVertex;
	boolean faceTracked;
	
	HDFaceData(){
		HDFaceVertex = new PVector[ HDFaceVertexCount];
		for(int i = 0; i < HDFaceVertexCount; i++)
			HDFaceVertex[i] = new PVector(0, 0);
	}
	
	protected void createHDFaceVertexData(float [] rawData, int iFace){
		int index = HDFaceVertexCount * 2;
		
		if(rawData[BODY_COUNT * HDFaceVertexCount * 2 + iFace] == 1)
			faceTracked = true;
		else
			faceTracked = false;
		for(int i = 0; i < HDFaceVertexCount; i++) {
			HDFaceVertex[i].x = rawData[index * iFace + i * 2 + 0];
			HDFaceVertex[i].y = rawData[index * iFace + i * 2 + 1];
		}	
	}
	
	public PVector [] getHDFaceVertex(){
		return HDFaceVertex;
	}
	
	public boolean isTracked() {
		return faceTracked;
	}
	
	public float getX(int index) {
		return HDFaceVertex[index].x;
	}
	
	public float getY(int index) {
		return HDFaceVertex[index].y;
	}
	
	public PVector getPVector(int index) {
		return HDFaceVertex[index];
	}


}
