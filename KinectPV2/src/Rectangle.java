package KinectPV2;

public class Rectangle {
	
	float x;
	float y;
	
	float width;
	float height;
	
	
	Rectangle(float x, float y, float width, float height){
		this.x = x;
		this.y = y;
		this.width = width;
		this.height = height;
	}
	
	public void setX(float x){
		this.x =x;
	}
	
	public void setY(float y){
		this.y =y;
	}
	
	public void setWidth(float w){
		this.width =w;
	}
	
	public void setHeight(float height){
		this.height = height;
	}
	
	public float getX(){
		return x;
	}
	
	public float getY(){
		return y;
	}
	
	public float getWidth(){
		return width;
	}
	
	public float getHeight(){
		return height;
	}
	
	
}
