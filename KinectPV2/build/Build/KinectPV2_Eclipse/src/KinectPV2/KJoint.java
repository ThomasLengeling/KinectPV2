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
 * Simple Joint Class with (x, y, z) position, Orientation, state and type
 * @author thomas
 *
 */
public class KJoint {
	
	protected PVector pos;
	
	protected int state;
	
	protected int type;
	
	protected  KQuaternion  orientation;
	
	
	
	KJoint(float x, float y, float z, KQuaternion ori, int state){
		pos = new PVector(x, y, z);
		orientation = ori;
		this.state = state;
	}
	
	KJoint(){}
	
	/**
	 * Get orientation of a single Joint
	 * @return Quartenion
	 */
	public KQuaternion getOrientation() {
		return orientation;
	}
	
	/**
	 * get PVector Position
	 * @return
	 */
	public PVector getPosition(){
		return pos;
	}
	/**
	 * get X position
	 * @return float x
	 */
	public float getX(){
		return pos.x;
	}
	
	/**
	 * get Y position
	 * @return float y
	 */
	public float getY(){
		return pos.y;
	}
	
	/**
	 * get Z position
	 * @return float z
	 */
	public float getZ(){
		return pos.z;
	}
	
	/**
	 * get State of a single Joint
	 * @return state
	 */
	public int getState(){
		return state;
	}
	
	/**
	 * get Joint Type 
	 * @return int
	 */
	public int getType(){
		return type;
	}
	
}
