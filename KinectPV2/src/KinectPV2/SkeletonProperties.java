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
 * Skeleton Properties
 * @author Thomas Sanchez Lengeling
 *
 */
public interface SkeletonProperties extends Constants{
	public final static int JointType_SpineBase		= 0;
	public final static int JointType_SpineMid		= 1;
	public final static int JointType_Neck			= 2;
	public final static int JointType_Head			= 3;
	public final static int JointType_ShoulderLeft  = 4;
	public final static int JointType_ElbowLeft		= 5;
	public final static int JointType_WristLeft	    = 6;
	public final static int JointType_HandLeft		= 7;
	public final static int JointType_ShoulderRight	= 8;
	public final static int JointType_ElbowRight	= 9;
	public final static int JointType_WristRight	= 10;
	public final static int JointType_HandRight		= 11;
	public final static int JointType_HipLeft		= 12;
	public final static int JointType_KneeLeft		= 13;
	public final static int JointType_AnkleLeft		= 14;
	public final static int JointType_FootLeft		= 15;
	public final static int JointType_HipRight		= 16;
	public final static int JointType_KneeRight		= 17;
	public final static int JointType_AnkleRight	= 18;
	public final static int JointType_FootRight		= 19;
	public final static int JointType_SpineShoulder	= 20;
	public final static int JointType_HandTipLeft	= 21;
	public final static int JointType_ThumbLeft		= 22;
	public final static int JointType_HandTipRight	= 23;
	public final static int JointType_ThumbRight	= 24;
	public final static int JointType_Count	= ( JointType_ThumbRight + 1 );
	
	public final static int JOINTSIZE = BODY_COUNT * (JointType_Count + 1) * 9;
	
	public final static int TrackingState_NotTracked = 0;
	public final static int TrackingState_Inferred	 = 1;
	public final static int TrackingState_Tracked	 = 2;
	
	
	public final static int HandState_Unknown		= 0;
    public final static int HandState_NotTracked	= 1;
    public final static int HandState_Open			= 2;
    public final static int HandState_Closed		= 3;
    public final static int HandState_Lasso			= 4;
}
