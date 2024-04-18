package com.ahmed.fun_gl;

import android.content.Context;
import android.opengl.GLSurfaceView;

import android.util.Log;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

public abstract class FunGLView extends GLSurfaceView {
    public static enum GlesVersion {
        GLES2(2), GLES3(3);

        private final int value;

        private GlesVersion(int v){
            value = v;
        }

        public int getValue(){
            return value;
        }
    }

    protected static String LOG_TAG = "FunGLView";
    abstract protected void setRendererCallback();
    abstract protected void destroyContextCallback();

    protected int redSize = 8;
    protected int greenSize = 8;
    protected int blueSize = 8;
    protected int alphaSize = 8;
    protected int numberOfSamples = 4;
    protected int depthSize = 16;

    protected GlesVersion glesVersion;

public FunGLView(Context context)
    {
        this(context,GlesVersion.GLES2);
    }

public FunGLView(Context context, GlesVersion version)
    {
        super(context);

        /* Initialize GLES version member */
        glesVersion = version;

        /* Initialize this view */
        setEGLContextFactory(new TheEGLContextFactory());
        setEGLConfigChooser(new TheConfigChooser());
        /* Allow the derived classes to set the renderer */
        setRendererCallback();
    }

    /*
     * Return the default value for depth size,
     * can be overridden by a subclass
     */
    protected int getDepthSize()
    {
        return depthSize;
    }

    /*
     * Return the default value for number or samples,
     * can be overridden by a subclass
     */
    protected int getNumberOfSamples()
    {
        return numberOfSamples;
    }

    /*
     * An implementation of "GLSurfaceView.EGLContextFactory"
     * for customizing the eglCreateContext and
     * eglDestroyContext calls.
     */
    protected class TheEGLContextFactory implements GLSurfaceView.EGLContextFactory
    {

        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config)
        {
            /* From the Khronos definitions */
            final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
            int error;

            while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
            {
                Log.e(LOG_TAG, String.format("Before TheEGLContextFactory.createContext(): EGL error: 0x%x", error));
            }

            /* Use the value of glesVersion */
            int[] attribs = {EGL_CONTEXT_CLIENT_VERSION, glesVersion.getValue(), EGL10.EGL_NONE };

            EGLContext context = egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, attribs);

            while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
            {
                Log.e(LOG_TAG, String.format("After TheEGLContextFactory.createContext(): EGL error: 0x%x", error));
            }

            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
        {
            /* Allow the derived classes to destroy their resources*/
            destroyContextCallback();
            egl.eglDestroyContext(display, context);
        }
    }

    /*
     * An implementation of "GLSurfaceView.EGLConfigChooser"
     * for choosing an EGLConfig configuration from a list of
     * potential configurations.
     */
    protected class TheConfigChooser implements GLSurfaceView.EGLConfigChooser
    {
        protected int getConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
        {
            int[] ret = new int[1];

            if (egl.eglGetConfigAttrib(display, config, attribute, ret))
            {
                return ret[0];
            }

            return defaultValue;
        }

        public EGLConfig SelectAnEGLConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
        {

            for(EGLConfig cfg : configs)
            {
                int r = getConfigAttrib(egl, display, cfg, EGL10.EGL_RED_SIZE, 0);
                int g = getConfigAttrib(egl, display, cfg, EGL10.EGL_GREEN_SIZE, 0);
                int b = getConfigAttrib(egl, display, cfg, EGL10.EGL_BLUE_SIZE, 0);
                int a = getConfigAttrib(egl, display, cfg, EGL10.EGL_ALPHA_SIZE, 0);

                if (r == redSize && g == greenSize && b == blueSize && a == alphaSize)
                {
                    return cfg;
                }
            }

            return null;
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            /* From the Khronos definitions */
            final int EGL_OPENGL_ES2_BIT = 4;

            int[] attribs =
                    {
                            EGL10.EGL_RED_SIZE, redSize,
                            EGL10.EGL_GREEN_SIZE, greenSize,
                            EGL10.EGL_BLUE_SIZE, blueSize,
                            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                            EGL10.EGL_SAMPLES, getNumberOfSamples(),
                            EGL10.EGL_DEPTH_SIZE, getDepthSize(),
                            EGL10.EGL_NONE
                    };
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, attribs, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0)
            {
                Log.e(LOG_TAG, "No EGL configs were found.");
                return null;
            }

            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, attribs, configs, numConfigs, num_config);

            return SelectAnEGLConfig(egl, display, configs);
        }
    }
}