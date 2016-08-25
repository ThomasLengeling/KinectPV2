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
 * Face Features class, with type Feature and State.
 * @author Thomas Sanchez Lengeling
 *
 */
public class FaceFeatures {
	
	int feature;
	int state;
	
	/**
	 * Create Feature
	 * @param feature
	 * @param state
	 */
	FaceFeatures(int feature, int state){
		this.feature = feature;
		this.state = state;
	}
	
	/**
	 * Set Feature
	 * @param feature
	 */
	public void setFeatureType(int feature){
		this.feature = feature;
	}
	
	/**
	 * Set State
	 * @param state
	 */
	public void setState(int state){
		this.state = state;
	}
	
	
	/**
	 * Get State
	 * @return
	 */
	public int getState(){
		return state;
	}
	
	/**
	 * Get Feature Type
	 * @return
	 */
	public int getFeatureType(){
		return feature;
	}
	
}
