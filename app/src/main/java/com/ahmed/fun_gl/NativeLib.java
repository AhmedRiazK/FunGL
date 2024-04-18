package com.ahmed.fun_gl;

public class NativeLib {

    // Used to load the 'fun_gl' library on application startup.
    static {
        System.loadLibrary("fun_gl");
    }

    /**
     * A native method that is implemented by the 'fun_gl' native library,
     * which is packaged with this application.
     */

//    public native String stringFromJNI();

    public static native void init(int width, int height);
    public static native void step();

    public static native void parse(String fileName);

    public static native void parseInit();
    public static native void parseLine(String line);
    public static native void prepareVertices();
}
