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

/**
 * Skeleton Class
 * @author Thomas Sanchez Lengeling
 *
 */
public class Skeleton implements SkeletonProperties{
	
	
	protected KJoint [] kJoints;
	
	protected int leftHandState;
	protected int rightHandState;
	
	private boolean tracked;
	
	Skeleton(){
		kJoints  = new KJoint[JointType_Count + 1];
		for(int i = 0; i < JointType_Count + 1; i++){
			kJoints[i] = new KJoint(0,0,0, new KQuaternion(), 0);
		}
	}
	
	/*
	 * if the current skeleton is being tracked
	 */
	public boolean isTracked(){
		return tracked;
	}
	
	/**
	 * get the array of joints of the skeleton
	 * @return  KJoint []
	 */
	public KJoint [] getJoints(){
		return kJoints;
	}
	
	/**
	 * get Left Hand State of the skeleton
	 * @return int leftHandState
	 */
	public int getLeftHandState(){
		return leftHandState;
	}
	
	/**
	 * get Right Hand State of the skeleton
	 * @return int rightHandState
	 */
	public int getRightHandState(){
		return rightHandState;
	}
	
	protected void createSkeletonData(float [] rawData, int i){
		int index2 = i * (JointType_Count+1) * 9;
		int indexJoint = index2 + (JointType_Count+1) * 9 - 1;
		if(rawData[indexJoint] == 1.0){
			tracked = true;
		}else{
			tracked = false;
		}
		
		if(tracked){
			for(int j = 0; j < JointType_Count; ++j){			
				int index1 = j * 9;
				kJoints[j].pos.x = rawData[index2 + index1 + 0];
				kJoints[j].pos.y = rawData[index2 + index1 + 1];
				kJoints[j].pos.z = rawData[index2 + index1 + 2];
				
				kJoints[j].orientation.w = rawData[index2 + index1 + 3];
				kJoints[j].orientation.x = rawData[index2 + index1 + 4];
				kJoints[j].orientation.y = rawData[index2 + index1 + 5];
				kJoints[j].orientation.z = rawData[index2 + index1 + 6];
				
				int state =   (int)rawData[index2 + index1 + 7];
				int type  =   (int)rawData[index2 + index1 + 8];
				
				kJoints[j].state = state;
				kJoints[j].type  = type;
				if(type == JointType_HandLeft)
					leftHandState = state;
				if(type == JointType_HandRight)
					rightHandState = state;
			}
		}
	}
}
