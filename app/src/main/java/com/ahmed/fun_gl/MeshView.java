package com.ahmed.fun_gl;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MeshView extends FunGLView {

    public MeshView(Context context)
    {
        super(context);
    }

    @Override protected void setRendererCallback()
    {
        setRenderer(new Renderer());
    }

    @Override protected void destroyContextCallback()
    {

    }

    protected class Renderer implements GLSurfaceView.Renderer
    {

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            NativeLib.init(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            NativeLib.step();
        }
    }
}