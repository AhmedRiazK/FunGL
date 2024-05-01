#include <jni.h>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "shader.h"
#include "matrix.h"

#include <iostream>

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

extern int get_indices_size();
extern float* get_indices();
extern unsigned short* get_vertices();
extern int get_stride_size();
extern ObjectList* get_objects_info();

extern float get_scene_width();
extern float get_scene_height();
extern float get_scene_depth();

float camera_dist_z;
float camera_dist_y;
float camera_dist_x;

/* [setupGraphics] */
bool setupGraphics(int width, int height)
{
    float temp = 0;

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
    float z_near = 0.1f;
    float z_far = sqrt(((temp * 2) * (temp * 2)) + ((temp * 2) * (temp * 2)));
    matrixPerspective(projectionMatrix, 60, (float)width / (float)height, z_near, z_far);
    glEnable(GL_DEPTH_TEST);

    if (get_scene_width() > get_scene_height())
    {
        temp = get_scene_width();
    }
    else
    {
        temp = get_scene_height();
    }

    if (temp < get_scene_depth())
    {
        temp = get_scene_depth();
    }

    camera_dist_y = camera_dist_z = (temp + temp / 2);

    glViewport(0, 0, width, height);

    return true;
}
/* [setupGraphics] */


/* [renderFrame] */
void renderFrame()
{
    std::cout << __LINE__ << std::endl;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    std::cout << __LINE__ << std::endl;
    matrixIdentityFunction(modelViewMatrix);
    std::cout << __LINE__ << std::endl;
    matrixRotateX(modelViewMatrix, angle);
    matrixRotateY(modelViewMatrix, angle);
    std::cout << __LINE__ << std::endl;
    matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f);
    mat4LookAt(modelViewMatrix, camera_dist_x, camera_dist_y, camera_dist_z, \
             /*lookat position*/0.0f, get_scene_height()/2, get_scene_depth() / 2, \
                                0.0f, 1.0f, 0.0f);

    glUseProgram(meshProgram);

    ObjectList* list = get_objects_info();
    while(list)
    {
        std::cout << __LINE__ << std::endl;
        float* ptr_vertices =  list->obj_data->vertices_info.data();
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, \
                        list->obj_data->stride * sizeof(float), ptr_vertices);
        glEnableVertexAttribArray(vertexLocation);
        std::cout << __LINE__ << std::endl;
        glVertexAttribPointer(vertexColourLocation, 3, GL_FLOAT, GL_FALSE,\
                        list->obj_data->stride * sizeof(float), \
                        (ptr_vertices+ (list->obj_data->stride - 3)));
        glEnableVertexAttribArray(vertexColourLocation);
        std::cout << __LINE__ << std::endl;
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
        glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix);
        std::cout << __LINE__ << std::endl;
        glDrawElements(GL_TRIANGLES, list->obj_data->indices_size, GL_UNSIGNED_SHORT, list->obj_data->indices_info.data());
        std::cout << __LINE__ << std::endl;
        list = list->next;
        std::cout << __LINE__ << std::endl;
    }

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
