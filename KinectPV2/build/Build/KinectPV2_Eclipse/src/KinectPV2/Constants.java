package KinectPV2;

public interface Constants {
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

	public final static int BODY_COUNT = 6;

	public final static int WIDTHColor  = 1920;
	public final static int HEIGHTColor = 1080;
	
	public final static int WIDTHDepth  = 512;
	public final static int HEIGHTDepth = 424;
	
	public final static int TrackingState_NotTracked = 0;
	public final static int TrackingState_Inferred	 = 1;
	public final static int TrackingState_Tracked	 = 2;
	
	public final static int Activity_EyeLeftClosed	= 0;
	public final static int Activity_EyeRightClosed	= 1;
	public final static int Activity_MouthOpen		= 2;
	public final static int Activity_MouthMoved		= 3;
	public final static int Activity_LookingAway	= 4;
	public final static int Activity_Count	= ( Activity_LookingAway + 1 );
	
	public final static int HandState_Unknown		= 0;
    public final static int HandState_NotTracked	= 1;
    public final static int HandState_Open			= 2;
    public final static int HandState_Closed		= 3;
    public final static int HandState_Lasso			= 4;
}
