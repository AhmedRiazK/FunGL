/* Copyright (c) 2012-2017, ARM Limited and Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SHADER_H
#define SHADER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define GL_CHECK(x)                                                                             \
    x;                                                                                          \
    {                                                                                           \
        GLenum glError = glGetError();                                                          \
        if(glError != GL_NO_ERROR)                                                              \
        {                                                                                       \
            LOGE("glGetError() = %i (%#.8x) at %s:%i\n", glError, glError, __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                                 \
        }                                                                                       \
    }

namespace MaliSDK
{
    class Shader
    {
    private:
        static char *loadShader(const char *filename);
        static char *loadShaderFromAsset(const char *filename, AAssetManager* manager);
    public:
        static void processShader(GLuint *shader, const char *filename, GLint shaderType, AAssetManager* manager);
    };
}
#endif /* SHADER_H */