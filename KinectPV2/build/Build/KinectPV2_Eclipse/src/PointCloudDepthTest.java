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

import KinectPV2.KinectPV2;
import processing.core.PApplet;
import processing.opengl.PGL;
import processing.opengl.PShader;

public class PointCloudDepthTest extends PApplet {

	private KinectPV2 kinect;

	float a = 0;
	int zval = 50;
	float scaleVal = 260;

	// Distance Threashold
	int maxD = 5000; // 4m
	int minD = 50;  //  1m
	//max 8000cm and min 5cm
	
	PGL pgl;
	PShader sh;

	public void setup() {
		size(1280, 720, P3D);

		kinect = new KinectPV2(this);
		kinect.enableDepthImg(true);
		
		kinect.activateRawDepth(true);
		kinect.enablePointCloud(true);

		kinect.setLowThresholdPC(minD);
		kinect.setHighThresholdPC(maxD);

		kinect.init();
		
		sh = loadShader("frag.glsl", "vert.glsl");
	}

	public void draw() {
		background(0);

		image(kinect.getDepthImage(), 0, 0, 320, 240);
		image(kinect.getPointCloudDepthImage(), 320, 0, 320, 240);
		 
		// Threahold of the point Cloud.
		kinect.setLowThresholdPC(minD);
		kinect.setHighThresholdPC(maxD);

		FloatBuffer pointCloudBuffer = kinect.getPointCloudDepthPos();
		
		pgl = beginPGL();
		sh.bind();
		
		int  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");
		  
		pgl.enableVertexAttribArray(vertLoc);
		
		int vertData = kinect.WIDTHColor * kinect.HEIGHTColor;
		  
		pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, pointCloudBuffer);

		pgl.drawArrays(PGL.POINTS , 0, vertData);
		
		sh.unbind(); 
		endPGL();
		
		
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