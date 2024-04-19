#include <jni.h>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "shader.h"
#include "matrix.h"

std::string resourceDirectory = "/data/data/com.ahmed.fun_gl/";
std::string vertexShaderFileName = "mesh.vert";
std::string fragmentShaderFileName = "mesh.frag";

#if 0
GLuint loadShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader != 0)
    {
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 0)
            {
                char * logBuffer = (char*) malloc(infoLen);

                if (logBuffer != NULL)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, logBuffer);
                    LOGE("Could not Compile Shader %d:\n%s\n", shaderType, logBuffer);
                    free(logBuffer);
                    logBuffer = NULL;
                }

                glDeleteShader(shader);
                shader = 0;
            }
        }
    }

    return shader;
}

GLuint createProgram(const char* vertexSource, const char * fragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0)
    {
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0)
    {
        return 0;
    }

    GLuint program = glCreateProgram();

    if (program != 0)
    {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if(linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength > 0)
            {
                char* logBuffer = (char*) malloc(bufLength);

                if (logBuffer != NULL)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, logBuffer);
                    LOGE("Could not link program:\n%s\n", logBuffer);
                    free(logBuffer);
                    logBuffer = NULL;
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}
#endif
GLuint simpleCubeProgram;
GLuint meshProgram;

GLuint vertexLocation;
GLuint vertexColourLocation;
GLuint projectionLocation;
GLuint modelViewLocation;

float projectionMatrix[16];
float modelViewMatrix[16];
float angle = 0;

AAssetManager* asset_manager = nullptr;

extern float* get_indices();
extern unsigned short* get_vertices();

/* [setupGraphics] */
bool setupGraphics(int width, int height)
{
    /*Shader path*/
#if 0
    const std::string vShaderFileName = resourceDirectory+vertexShaderFileName;
    const std::string fShaderFileName = resourceDirectory+fragmentShaderFileName;
#else
    const std::string vShaderFileName = "glsl/"+vertexShaderFileName;
    const std::string fShaderFileName = "glsl/"+fragmentShaderFileName;
#endif
    /*Process Shader*/
    GLuint vShaderID, fShaderID;
    MaliSDK::Shader::processShader(&vShaderID, vShaderFileName.c_str(), \
    GL_VERTEX_SHADER, asset_manager);
    MaliSDK::Shader::processShader(&fShaderID, fShaderFileName.c_str(), \
    GL_FRAGMENT_SHADER, asset_manager);

    /*Create program*/
    meshProgram = GL_CHECK(glCreateProgram());

    /*Attach shader*/
    GL_CHECK(glAttachShader(meshProgram, vShaderID));
    GL_CHECK(glAttachShader(meshProgram, fShaderID));

    /*Link shader*/
    GL_CHECK(glLinkProgram(meshProgram));

    GL_CHECK(glUseProgram(meshProgram));


    vertexLocation = glGetAttribLocation(meshProgram, "vertexPosition");
    vertexColourLocation = glGetAttribLocation(meshProgram, "vertexColour");
    projectionLocation = glGetUniformLocation(meshProgram, "projection");
    modelViewLocation = glGetUniformLocation(meshProgram, "modelView");

    /* Setup the perspective */
    matrixPerspective(projectionMatrix, 45, (float)width / (float)height, 0.1f, 100);
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, width, height);

    return true;
}
/* [setupGraphics] */


/* [renderFrame] */
void renderFrame()
{
    float* ptr_vertices= reinterpret_cast<float *>(get_vertices());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    matrixIdentityFunction(modelViewMatrix);

    matrixRotateX(modelViewMatrix, angle);
    matrixRotateY(modelViewMatrix, angle);

    matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f);

    glUseProgram(meshProgram);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 13* sizeof(float), ptr_vertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation, 3, GL_FLOAT, GL_FALSE, 13* sizeof(float), (ptr_vertices+10));
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, get_indices());

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}
/* [renderFrame] */
extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_init(JNIEnv *env, jclass clazz, jint width, jint height) {
        setupGraphics(width, height);
    }

    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_step(JNIEnv *env, jclass clazz) {
        renderFrame();
    }

    JNIEXPORT void JNICALL
    Java_com_ahmed_fun_1gl_NativeLib_importAssets(JNIEnv *env, jclass clazz, jobject object) {
        asset_manager = AAssetManager_fromJava(env, object);
    }
}
