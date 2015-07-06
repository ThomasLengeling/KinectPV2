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

import processing.core.PApplet;
import processing.core.PImage;

/**
 * Image class helper
 * @author Thomas Sanchez Lengeling
 *
 */
public class Image{
	private    float fps;
	private    int imgPixelSize;
	private    PApplet parent;
	
	protected  PImage    img;
	private    boolean   processRawData;
	
	protected  byte  []  rawByteData;
	
	protected  float []  rawFloatData;
	protected  int   []  rawIntData;
	
	public Image(PApplet p, int width, int height, int MODE){
		parent = p;
		img    =  parent.createImage(width, height, MODE);
		imgPixelSize = width * height;
		rawByteData  = new byte[imgPixelSize];
		
		rawIntData   = new int[imgPixelSize];
		rawFloatData = new float[imgPixelSize];
		processRawData= false;
	}
	
	/**
	 * Get Image Size in int
	 * @return int
	 */
	public int getImgSize(){
		return imgPixelSize;
	}
	
	/**
	 * update Pixels, img.updatePixels()
	 */
	public void updatePixels(){
		img.updatePixels();
	}
	
	/**
	 * load Pixels, img.loadPixels()
	 */
	public void loadPixels(){
		img.loadPixels();
	}
	
	/**
	 * get Pixels Image,  img.pixels
	 * @return int []
	 */
	public int [] pixels(){
		return img.pixels;
	}
	
	/**
	 * get Image
	 * @return PImage
	 */
	public PImage getImage() {		
		return img;
	}
	
	/**
	 * get Fps of the current Image
	 * @return float
	 */
	public float getFPS() {
		return fps;
	}
	
	/**
	 * Process Raw Data
	 * @return boolean
	 */
	public boolean isProcessRawData(){
		return processRawData;
	}
	
	/**
	 * Activate Process Rae Data
	 * @param rawData
	 */
	public void activateRawData(boolean rawData){
		processRawData = rawData;
	}
}
