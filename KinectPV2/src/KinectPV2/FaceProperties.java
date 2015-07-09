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
 * Commun Face Properties
 * @author Thomas Sanchez Lengeling
 *
 */
public interface FaceProperties  extends Constants {
	public final static int FaceProperty_Happy	= 0;
	public final static int FaceProperty_Engaged	= 1;
	public final static int FaceProperty_LeftEyeClosed		= 2;
	public final static int FaceProperty_RightEyeClosed		= 3;
	public final static int FaceProperty_LookingAway	= 4;
	public final static int FaceProperty_MouthMoved = 5;
	public final static int FaceProperty_MouthOpen = 6;
	public final static int FaceProperty_WearingGlasses = 7;
	public final static int Activity_Count	= ( FaceProperty_WearingGlasses + 1 );
	
	public final static int FACESIZE  = BODY_COUNT * (36);
	
	public final static int DetectionResult_Unknown = -1;
	public final static int DetectionResult_Yes = 1;
	public final static int DetectionResult_No = 0;
	public final static int DetectionResult_Maybe =2;
	
	public final static int Face_LeftEye = 0;
	public final static int Face_RightEye = 1;
	public final static int Face_Nose = 2;
	public final static int Face_LeftMouth = 3;
	public final static int Face_RightMouth = 4;
	
	public final static int HDFaceVertexCount = 1347;
	
	//6 MORE POINT FOR TRACKING AND NOT TRAKING
	public final static int HDFaceVertexPoints = HDFaceVertexCount * BODY_COUNT * 2 + BODY_COUNT;
	
}