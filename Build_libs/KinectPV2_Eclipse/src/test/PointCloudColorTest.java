package test;

/*
 * Copyright (C) 2014 Thomas Sanchez Lengeling. KinectPV2, Kinect for
 * Windows v2 library for processing
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

import java.nio.*;

import KinectPV2.KinectPV2;
import processing.core.*;
import processing.opengl.PGL;
import processing.opengl.PShader;


public class PointCloudColorTest extends PApplet {


	private KinectPV2 kinect;


	//values for the 3d scene
	//rotation
	float a = 3;
	//z scale value
	int   zval     = 350;
	//value to scale the point cloud
	float scaleVal = 990;

	//rotation
	float rotY = 0;
	float rotZ = 0;
	float rotX = PI;

	float depthVal = 0;

	int vertLoc;
	int colorLoc;
	
	//VBO buffer location in the GPU
	int vertexVboId;
	int colorVboId;


	//openGL instances
	//render ot openGL object
	PGL pgl;

	//create a shader
	PShader sh;

	
	public static void main(String[] args) {
		PApplet.main(new String[] { "test.PointCloudColorTest"});
	}
	
	public void settings(){
		size(1280, 720,  P3D);
	}
	


	public void setup() {

		  kinect = new KinectPV2(this);

		  kinect.enableDepthImg(true);
		  kinect.enableColorImg(true);
		  kinect.enableColorPointCloud(true);
		
		  kinect.init();
		 
		  sh = loadShader("frag.glsl", "vert.glsl");
		  
		  PGL pgl = beginPGL();

		  // allocate buffer big enough to get all VBO ids back
		  IntBuffer intBuffer = IntBuffer.allocate(2);
		  pgl.genBuffers(2, intBuffer);

		  //memory location of the VBO
		  vertexVboId = intBuffer.get(0);
		  colorVboId = intBuffer.get(1);

		  endPGL();
	}

	public void draw() {

		  background(0);

		  image(kinect.getColorImage(), 0, 0, 320, 240);


		  // The geometric transformations will be automatically passed to the shader
		  pushMatrix();
		  translate(width / 2, height / 2, zval);
		  scale(scaleVal, -1 * scaleVal, scaleVal);
		  rotate(a, 0.0f, 1.0f, 0.0f);

		  //render to the openGL object
		  pgl = beginPGL();
		  sh.bind();


		  //obtain the point cloud positions
		  FloatBuffer pointCloudBuffer = kinect.getPointCloudColorPos();

		  //get the color for each point of the cloud Points
		  FloatBuffer colorBuffer      = kinect.getColorChannelBuffer();

		  //send the the vertex positions (point cloud) and color down the render pipeline
		  //positions are render in the vertex shader, and color in the fragment shader
		  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");
		  pgl.enableVertexAttribArray(vertLoc);
		  
		  
		  //enable drawing to the vertex and color buffer
		  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");
		  pgl.enableVertexAttribArray(colorLoc);

		  int vertData = kinect.WIDTHColor * kinect.HEIGHTColor * 3;

		  //vertex
		  {
		    pgl.bindBuffer(PGL.ARRAY_BUFFER, vertexVboId);
		    // fill VBO with data
		    pgl.bufferData(PGL.ARRAY_BUFFER, Float.BYTES * vertData, pointCloudBuffer, PGL.DYNAMIC_DRAW);
		    // associate currently bound VBO with shader attribute
		    pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, Float.BYTES * 3, 0);
		  }

		  // color
		  {
		    pgl.bindBuffer(PGL.ARRAY_BUFFER, colorVboId);
		    // fill VBO with data
		    pgl.bufferData(PGL.ARRAY_BUFFER, Float.BYTES * vertData, colorBuffer, PGL.DYNAMIC_DRAW);
		    // associate currently bound VBO with shader attribute
		    pgl.vertexAttribPointer(colorLoc, 3, PGL.FLOAT, false, Float.BYTES * 3, 0);
		  }

		  // unbind VBOs
		  pgl.bindBuffer(PGL.ARRAY_BUFFER, 0);

		  //draw the point cloud as a set of points
		  pgl.drawArrays(PGL.POINTS, 0, vertData);

		  //disable drawing
		  pgl.disableVertexAttribArray(vertLoc);
		  pgl.disableVertexAttribArray(colorLoc);

		  //close the shader
		  sh.unbind();
		  //close the openGL object
		  endPGL();

		  popMatrix();

		  stroke(255, 0, 0);
		  text(frameRate, 50, height- 50);
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
			scaleVal += 1;
			println(scaleVal);
		}
		if (key == 'x') {
			scaleVal -= 1;
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

		if (key == 'c') {
			depthVal -= 0.01;
			println(depthVal);
		}

		if (key == 'v') {
			depthVal += 0.01;
			println(depthVal);
		}
	}

}
