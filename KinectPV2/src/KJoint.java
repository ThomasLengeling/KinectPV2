package KinectPV2;

public class KJoint {
	
	protected float x;
	protected float y;
	protected float z;
	
	protected int state;
	
	protected int type;
	
	KJoint(float x, float y, float z, int state){
		this.x = x;
		this.y = y;
		this.z = z;
		this.state = state;
	}
	
	KJoint(){}
	
	public float getX(){
		return x;
	}
	
	public float getY(){
		return y;
	}
	
	public float getZ(){
		return z;
	}
	
	public int getState(){
		return state;
	}
	
	public int getType(){
		return type;
	}
	
}
