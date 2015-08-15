/*
Simple class that manager saving each FloatBuffer and writes the data into a OBJ file
*/
class FrameBuffer {

  FloatBuffer frame;
  
  //id of the frame
  int frameId;

  FrameBuffer(FloatBuffer f) {
    frame = clone(f);
  }

  void setFrameId(int fId) {
    frameId = fId;
  }

  /*
  Writing of the obj file,
  */
  void saveOBJFrame() {
    int vertData = 512 * 424;
    String[] points = new String[vertData];

    //Iterate through all the XYZ points
    for (int i = 0; i < vertData; i++) {
      float x =  frame.get(i*3 + 0);
      float y =  frame.get(i*3 + 1);
      float z =  frame.get(i*3 + 2);
      points[i] = "v "+x+" "+y+" "+z;
    }
    
    saveStrings("data/frame0"+frameId+".obj", points);
    println("Done Saving Frame "+frameId);
  }

  //Simple function that copys the FloatBuffer to another FloatBuffer
  public  FloatBuffer clone(FloatBuffer original) {
    FloatBuffer clone = FloatBuffer.allocate(original.capacity());
    original.rewind();//copy from the beginning
    clone.put(original);
    original.rewind();
    clone.flip();
    return clone;
  }
  
}