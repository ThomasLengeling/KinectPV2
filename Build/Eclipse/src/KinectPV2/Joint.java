package KinectPV2;

public class Joint {
	
	float x;
	float y;
	float z;
	
	int state;
	
	int type;
	
	Joint(float x, float y, float z, int state){
		this.x = x;
		this.y = y;
		this.z = z;
		this.state = state;
	}
	
	Joint(){}
	
}
