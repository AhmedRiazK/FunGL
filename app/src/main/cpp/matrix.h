/* Copyright (c) 2013-2017, ARM Limited and Contributors
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

#ifndef MATRIX_H
#define MATRIX_H
#include <math.h>
#include <string>
#include <vector>

void matrixIdentityFunction(float* matrix);

void matrixTranslate(float* matrix, float x, float y, float z);

void matrixMultiply(float* destination, float* operand1, float* operand2);

void matrixFrustum(float* matrix, float left, float right, float bottom, float top, float zNear, float zFar);

void matrixPerspective(float* matrix, float fieldOfView, float aspectRatio, float zNear, float zFar);

void matrixRotateX(float* matrix, float angle);

void matrixRotateY(float* matrix, float angle);

void matrixRotateZ(float* matrix, float angle);

void matrixScale(float* matrix,float x, float y, float z);

float matrixDegreesToRadians(float degrees);

void  mat4LookAt(float* matrix,
                 float eyeX, float eyeY, float eyeZ,
                 float centerX, float centerY, float centerZ,
                 float upX, float upY, float upZ);

struct ObjectData
{
    std::string obj_name;
    std::vector<float> vertices_info;
    std::vector<unsigned short> indices_info;
    int vertices_size;
    int indices_size;
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z_min;
    float z_max;
    float cx;
    float cy;
    float cz;
    int stride;


    ObjectData(std::string obj_name, std::vector<float> vertices_info, \
                std::vector<unsigned short> indices_info, int vertices_size, \
                int indices_size, int stride): obj_name(obj_name), vertices_info(vertices_info), \
                indices_info(indices_info), vertices_size(vertices_size),
                indices_size(indices_size), stride(stride)
    {}

    void set_object_boundary(float xMin, float yMin, float zMin, \
                                float xMax, float yMax, float zMax)
    {
        x_min = xMin;
        x_max = xMax;
        y_min = yMin;
        y_max = yMax;
        z_min = zMin;
        z_max = zMax;
    }

    void set_object_center(float xCenter, float yCenter, float zCenter)
    {
        cx = xCenter;
        cy = yCenter;
        cz = zCenter;
    }
};

struct ObjectList
{
    ObjectData* obj_data;
    ObjectList* next;
};

#endif