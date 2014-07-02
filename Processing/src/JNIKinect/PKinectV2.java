/**
 * It is a Processing library for the Kinect One V2.0
 *
 * ##copyright##
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 * 
 * @author		Thomas Sanchez Lengeling
 * @modified	21-03-2014
 * @version		0.1
 */

package JNIKinect;

import processing.core.PApplet;
import processing.core.PImage;

public class PKinectV2 implements Runnable {
	static {
		int arch = Integer.parseInt(System.getProperty("sun.arch.data.model"));
		System.out.println(arch);
		if (arch == 64) {
			System.loadLibrary("PKinectV2");
			System.out.println("PKinect 0.1 by Thomas Sanchez Lengeling");
		} else {
			System.loadLibrary("PKinectV2");
			System.out.println("ERROR CANOT LOAD 32bits ");
		}
	}

	public final static String VERSION = "version 1.2.0";

	private final static int CNT = 5;
	private PApplet parent;
	
	private final static int WIDTHColor  = 1920;
	private final static int HEIGHTColor = 1080;
	
	private final static int WIDTHDepth  = 512;
	private final static int HEIGHTDepth = 424;
	
	private long ptr;

	public PKinectV2(PApplet _p) {
		parent = _p;
		
		init();
		
		String load = Nui_load();
		System.out.println(load);

		// parent.registerDispose(this);

		welcome();
		(new Thread(this)).start();
	}
	
	public void updateKinect(){
		update();
	}

	private native void init();
	
	private native void update();

	private native int[] Nui_GetImage();

	private native int[] Nui_GetDepth();

	private native int[] Nui_GetMask();
	
	private native String Nui_load();

	private void welcome() {
		System.out.println("Starting");
	}

	public static String version() {
		return VERSION;
	}

	public PImage GetImage() {
		PImage img = parent.createImage(WIDTHColor, HEIGHTColor, parent.ARGB);
		PApplet.arrayCopy(Nui_GetImage(), img.pixels);
		img.updatePixels();
		return img;
	}

	public PImage GetDepth() {
		PImage img = parent.createImage(WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		img.loadPixels();
		int[] grayImg = Nui_GetDepth();
		for(int i=0; i < grayImg.length; i++){
			img.pixels[i] = parent.color(grayImg[i]);
		}
		img.updatePixels();
		return img;
	}
	
	public PImage GetMask() {
		PImage img = parent.createImage(WIDTHDepth, HEIGHTDepth, parent.ALPHA);
		img.loadPixels();
		int[] grayImg = Nui_GetMask();
		for(int i=0; i < grayImg.length; i++){
			img.pixels[i] = parent.color(grayImg[i]);
		}
		img.updatePixels();
		return img;
	}

	public void run() {
		int fr = PApplet.round(1000.0f / parent.frameRate);
		while (true) {
			try {
				Thread.sleep(fr);
			} catch (InterruptedException e) {
				e.printStackTrace();
				return;
			}
			update();
		}
		
	}

	public void dispose() {
	}
}
