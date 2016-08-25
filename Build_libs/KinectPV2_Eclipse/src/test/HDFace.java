package test;

import java.util.ArrayList;

import KinectPV2.KinectPV2;
import KinectPV2.HDFaceData;
import processing.core.*;


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

public class HDFace extends PApplet {

	KinectPV2 kinect;
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.HDFace"});
	}
	
	public void settings(){
		size(1920, 1080, P3D);
	}

	public void setup() {
		kinect = new KinectPV2(this);
		kinect.enableColorImg(true);
		
		kinect.enableHDFaceDetection(true);
		kinect.init();
	}

	public void draw() {
		background(0);

		// DRAW COLOR IMAGE MAP
		image(kinect.getColorImage(), 0, 0);

		ArrayList<HDFaceData> hdFaceData = kinect.getHDFaceVertex();

		stroke(0, 255, 0);
		for (int i = 0; i < hdFaceData.size(); i++) {
			beginShape(POINTS);
			HDFaceData HDfaceData = (HDFaceData)hdFaceData.get(i);
			if (HDfaceData.isTracked()) {
				for (int j = 0; j < KinectPV2.HDFaceVertexCount; j++) {
					float x = HDfaceData.getX(j);
					float y = HDfaceData.getY(j);
					vertex(x, y);
				}
			}
			endShape();
		}
	}
}
