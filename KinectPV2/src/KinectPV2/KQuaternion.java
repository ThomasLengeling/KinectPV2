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
 * Simple KQuartenion class
 * @author Thomas Sanchez Lengeling
 *
 */
public class KQuaternion {
	
	float x;
	float y;
	float z;
	
	float w;
	
	KQuaternion(float w, float x, float y, float z){
		this.x = x;
		this.y = y;
		this.z = z;
		this.w = w;
	}
	
	KQuaternion(){
		this.x = 0;
		this.y = 0;
		this.z = 0;
		this.w = 0;
	}
	
	
	 public PVector rotate(float x1, float y1, float z1)
     {
		 KQuaternion q = new KQuaternion(0.0f, x1, y1, z1);
		 KQuaternion r =  mult(new KQuaternion(w, x, y, z), q);
		 KQuaternion conj = Conj();
		 KQuaternion retult = mult(r, conj);
         return new PVector( retult.x, retult.y, retult.z);
     }
	 
	 public KQuaternion Conj()
     {
         return new KQuaternion(w, -x, -y, -z);
     }
	 
	 public KQuaternion mult(KQuaternion q1, KQuaternion q2)
	 {
		 return new KQuaternion(q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
	                , q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
	                , q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z
	                , q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x);
	 }
	 
	public float getX(){
		return x;
	}
	
	public float getY(){
		return y;
	}
	
	public float getZ(){
		return z;
	}
	
	public float getW(){
		return w;
	}
	
}
