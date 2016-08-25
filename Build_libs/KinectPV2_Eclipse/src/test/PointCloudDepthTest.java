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

package test;

import java.nio.FloatBuffer;


import KinectPV2.KinectPV2;
import processing.core.*;


public class PointCloudDepthTest extends PApplet {

	private KinectPV2 kinect;

	float a = 0;
	int zval = 50;
	float scaleVal = 260;

	// Distance Threashold
	int maxD = 5000; // 4m
	int minD = 50;  //  1m
	//max 8000cm and min 5cm
	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.PointCloudDepthTest"});
	}
	
	public void settings(){
		size(1280, 720, P3D);
	}
	

	public void setup() {
		

		kinect = new KinectPV2(this);
		
		kinect.enableDepthImg(true);
		
		kinect.enablePointCloud(true);

		kinect.setLowThresholdPC(minD);
		kinect.setHighThresholdPC(maxD);

		kinect.init();
		
	}

	public void draw() {
		background(0);

		image(kinect.getDepthImage(), 0, 0, 320, 240);
		image(kinect.getPointCloudDepthImage(), 320, 0, 320, 240);
		 
		// Threahold of the point Cloud.
		kinect.setLowThresholdPC(minD);
		kinect.setHighThresholdPC(maxD);

		FloatBuffer pointCloudBuffer = kinect.getPointCloudDepthPos();
		
	
	/*
		PJOGL pgl = (PJOGL)beginPGL();
		GL2 gl2 = pgl.gl.getGL2();

		gl2.glEnable( GL2.GL_BLEND );
		gl2.glEnable(GL2.GL_POINT_SMOOTH);      

	 	gl2.glEnableClientState(GL2.GL_VERTEX_ARRAY);
	 	gl2.glVertexPointer(3, GL2.GL_FLOAT, 0, pointCloudBuffer);

	 	gl2.glTranslatef(width/2, height/2, zval);
	 	gl2.glScalef(scaleVal, -1*scaleVal, scaleVal);
	  	gl2.glRotatef(a, 0.0f, 1.0f, 0.0f);

	  	gl2.glDrawArrays(GL2.GL_POINTS, 0, kinect.WIDTHDepth * kinect.HEIGHTDepth);
	  	gl2.glDisableClientState(GL2.GL_VERTEX_ARRAY);
	  	gl2.glDisable(GL2.GL_BLEND);
	  	endPGL();
	*/

		stroke(255, 0, 0);
		text(frameRate, 50, height - 50);
	}

	public void mousePressed() {

		println(frameRate);
		// saveFrame();
	}

	public void keyPressed() {
		if (key == 'a') {
			zval += 1;
			println(zval);
		}
		if (key == 's') {
			zval -= 1;
			println(zval);
		}

		if (key == 'z') {
			scaleVal += 0.1;
			println(scaleVal);
		}
		if (key == 'x') {
			scaleVal -= 0.1;
			println(scaleVal);
		}

		if (key == 'q') {
			a += 1;
			println(a);
		}
		if (key == 'w') {
			a -= 1;
			println(a);
		}

		if (key == '1') {
			minD += 1;
			println("Change min: " + minD);
		}

		if (key == '2') {
			minD -= 1;
			println("Change min: " + minD);
		}

		if (key == '3') {
			maxD += 1;
			println("Change max: " + maxD);
		}

		if (key == '4') {
			maxD -= 1;
			println("Change max: " + maxD);
		}

		if (key == '2') {
			minD -= 1;
			println("Change min: " + minD);
		}

		if (key == '3') {
			maxD += 1;
			println("Change max: " + maxD);
		}

		if (key == '4') {
			maxD -= 1;
			println("Change max: " + maxD);
		}
	}
}