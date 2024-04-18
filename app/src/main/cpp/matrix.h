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

#endif