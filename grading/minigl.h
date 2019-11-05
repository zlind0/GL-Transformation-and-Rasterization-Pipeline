/**
 * minigl.h
 * -------------------------------
 * This file defines the API to be implemented for MiniGL.
 */

#ifndef __MINIGL_H__
#define __MINIGL_H__



//===============================================Original mat.h
//
//  mat.h
//  OpenGL-Test
//
//  Created by clock on 10/13/16.
//  Copyright © 2016 clock. All rights reserved.
//

#ifndef mat_h
#define mat_h
#include <cmath>
#include <cstring>
class vec3{
public:
    float data[4];
    vec3(float x, float y, float z){
        data[0]=x;
        data[1]=y;
        data[2]=z;
        data[3]=1;
    }
    vec3(){
        data[0]=0;
        data[1]=0;
        data[2]=0;
        data[3]=1;
    }
    vec3 normalized(){
        vec3 result;
        float sum;
        for(int i=0; i<3;i++){
            sum+=data[i]*data[i];
        }
        sum=sqrt(sum);
        for(int i=0; i<3;i++){
            result.data[i]=data[i]/sum;
        }
        return result;
    }
    
    vec3 operator+(const vec3 &right){
        vec3 result;
        for(int i=0; i<3; i++)
            result.data[i]=this->data[i]+right.data[i];
        return result;
    }
    vec3 operator-(const vec3 &right){
        vec3 result;
        for(int i=0; i<3; i++)
            result.data[i]=this->data[i]-right.data[i];
        return result;
    }
    vec3 operator*(const float &k){
        vec3 result;
        for(int i=0; i<3; i++)
            result.data[i]=this->data[i]*i;
        return result;
    }
    
    
    vec3 cross(vec3 left, vec3 right){
        vec3 result;
        result.data[0]=left.data[1]*right.data[2]-left.data[2]*right.data[1];
        result.data[1]=left.data[2]*right.data[1]-left.data[1]*right.data[3];
        result.data[2]=left.data[0]*right.data[1]-left.data[1]*right.data[0];
        return result;
    }
    
    float dot(vec3 left, vec3 right){
        float result;
        for(int i=0; i<3; i++)
            result+=left.data[i]+right.data[i];
        return result;
    }
    
};

enum MatType{
    MAT_ROTATION,
    MAT_TRANSLATION,
    MAT_SCALEING,
    MAT_ORTHOGRAPHIC,
    MAT_PERSPECTIVE,
    MAT_ZERO,
    MAT_IDENTITY,
    MAT_VIEWPORT
};

class mat4{
public:
    float data[4][4];
    
    mat4(bool setToIdentityMatrix){
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                data[i][j]=0;
        if(setToIdentityMatrix)
            for(int i=0; i<4;i++) data[i][i]=1;
    }
    
    mat4(MatType type, int x, int y){
        if(type == MAT_VIEWPORT){
            for(int i=0; i<4; i++)
                for(int j=0; j<4; j++)
                    data[i][j]=0;
            data[0][0]=(float)(x)/2.0;
            data[1][1]=(float)(y)/2.0;
            data[0][3]=(float)(x-1)/2.0;
            data[1][3]=(float)(y-1)/2.0;
            data[2][2]=1;
            data[3][3]=1;
        }
    }
    mat4(MatType type,
         float l,
         float r,
         float b,
         float t,
         float n,
         float f)
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                data[i][j]=0;
        if(type == MAT_PERSPECTIVE){
            data[0][0] = 2.0*n/(r-l);
            data[1][1] = 2.0*n/(t-b);
            data[2][2] = (n+f)/(n-f);
            data[0][2] = (r+l)/(r-l);
            data[1][2] = (t+b)/(t-b);
            data[2][3] = 2.0*f*n/(f-n);
            data[3][2] = 1;
        }
        else if(type == MAT_ORTHOGRAPHIC){
            data[0][0] = 2.0/(r-l);
            data[1][1] = 2.0/(t-b);
            data[2][2] = 2.0/(n-f);
            data[0][3] = -(r+l)/(r-l);
            data[1][3] = -(t+b)/(t-b);
            data[2][3] = -(n+f)/(n-f);
            data[3][3] = 1;
        }
    }
    
    mat4(MatType type,
         float x,
         float y,
         float z,
         float rad)
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                data[i][j]=0;
        
        if(type == MAT_ROTATION){
            //the matrix is from Wikipedia
            //https://en.wikipedia.org/wiki/Rotation_matrix
            float length = sqrtf(x*x + y*y + z*z);
            x=x/length; y=y/length; z=z/length;
            float s = sinf(rad), c = cos(rad), c_ = 1-c;
            data[0][0] = c+x*x*c_;
            data[0][1] = x*y*c_-z*s;
            data[0][2] = x*z*c_+y*s;
            data[1][0] = y*z*c_+z*s;
            data[1][1] = c+y*y*c_;
            data[1][2] = y*z*c_-x*s;
            data[2][0] = z*x*c_-y*s;
            data[2][1] = z*y*c_+x*s;
            data[2][2] = c+z*z*c_;
            data[3][3] = 0;
        }
    }
    mat4(MatType type, float x, float y, float z){
        switch (type) {
            case MAT_SCALEING:
                memset(data, sizeof(data), 0);
                data[0][0] = x;
                data[1][1] = y;
                data[2][2] = z;
                data[3][3] = 1;
                break;
            case MAT_TRANSLATION:
                memset(data, sizeof(data), 0);
                data[0][0] = x;
                data[1][1] = y;
                data[2][2] = z;
                data[3][3] = 1;
            default:
                break;
        }
    }
    mat4 operator+ (const mat4 &right){
        mat4 result(false);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++){
                result.data[i][j] = this->data[i][j] + right.data[i][j];
            }
        return result;
    }
    mat4 operator- (const mat4 &right){
        mat4 result(false);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++){
                result.data[i][j] = this->data[i][j] + right.data[i][j];
            }
        return result;
    }
    mat4& operator+= (const mat4 &right){
        mat4 result(false);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++){
                this->data[i][j] += right.data[i][j];
            }
        return *this;
    }
    mat4& operator-= (const mat4 &right){
        mat4 result(false);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++){
                this->data[i][j] += right.data[i][j];
            }
        return *this;
    }
    
    mat4 operator* (const mat4 &right){
        mat4 result(false);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                for(int k=0; k<4; k++)
                    result.data[i][j] += this->data[i][k] * right.data[k][j];
        return result;
    }
    
    vec3 operator* (const vec3 &right){
        vec3 result;
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                result.data[i]+=right.data[j]*this->data[i][j];
        return result;
    }
    
    
    
};


#endif /* mat_h */


#include <vector>
using std::vector;

typedef int           MGLint;
typedef unsigned char MGLbyte;
typedef unsigned int  MGLsize;
typedef unsigned int  MGLpixel;
typedef float         MGLfloat;
typedef bool          MGLbool;

typedef enum {
    MGL_MODELVIEW,
    MGL_PROJECTION
} MGLmatrix_mode;

typedef enum {
    MGL_TRIANGLES,
    MGL_QUADS
} MGLpoly_mode;

/**
 * Macros to get and set color values from an individual
 * pixel.
 */
#define MGL_GET_RED(P)   (((P) & 0xff000000) >> 24)
#define MGL_GET_GREEN(P) (((P) & 0x00ff0000) >> 16)
#define MGL_GET_BLUE(P)  (((P) & 0x0000ff00) >> 8)

#define MGL_SET_RED(P, C)   (P = (((P) & 0x00ffffff) | ((C) << 24)))
#define MGL_SET_GREEN(P, C)  (P = (((P) & 0xff00ffff) | ((C) << 16)))
#define MGL_SET_BLUE(P, C) (P = (((P) & 0xffff00ff) | ((C) << 8)))
struct MGLViewPoint{
    MGLpixel color;
    MGLfloat distance;
};

struct MGLpoint{
    MGLfloat x,y,z;
    MGLpixel color;
};

struct MGLquad{
    MGLpoint p[4];
};




/**
 * Read pixel data starting with the pixel at coordinates
 * (0, 0), up to (width,  height), into the array
 * pointed to by data.  The boundaries are lower-inclusive,
 * that is, a call with width = height = 1 would just read
 * the pixel at (0, 0).
 *
 * Rasterization and z-buffering should be performed when
 * this function is called, so that the data array is filled
 * with the actual pixel values that should be displayed on
 * the two-dimensional screen.
 */

void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data);

inline void mglSetPixel(int x, int y, MGLpixel color);

/**
 * Draw a straight line with color.
 */
void mglDrawLine(int x0, int y0,
                 int x1, int y1,
                 MGLpixel stColor,
                 MGLpixel endColor);

/**
 * Draw a straight triangle with color.
 */
void mglDrawTriangle(int x0, int y0,
                     int x1, int y1,
                     int x2, int y2,
                     MGLpixel color0,
                     MGLpixel color1,
                     MGLpixel color2,
                     int drawLeft);
/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode);

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd();

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y);

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z);

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode);

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix();

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix();

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity();

/**
 * Replace the current matrix with an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglLoadMatrix(const MGLfloat *matrix);

/**
 * Multiply the current matrix by an arbitrary 4x4 matrix,
 * specified in column-major order.  That is, the matrix
 * is stored as:
 *
 *   ( a0  a4  a8  a12 )
 *   ( a1  a5  a9  a13 )
 *   ( a2  a6  a10 a14 )
 *   ( a3  a7  a11 a15 )
 *
 * where ai is the i'th entry of the array.
 */
void mglMultMatrix(const MGLfloat *matrix);

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z);

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
               MGLfloat x,
               MGLfloat y,
               MGLfloat z);

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
              MGLfloat y,
              MGLfloat z);

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
                MGLfloat right,
                MGLfloat bottom,
                MGLfloat top,
                MGLfloat near,
                MGLfloat far);

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
              MGLfloat right,
              MGLfloat bottom,
              MGLfloat top,
              MGLfloat near,
              MGLfloat far);

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
              MGLfloat green,
              MGLfloat blue);

//=====================================================Original mglRgbColor.h
//
//  mglRgbColor.hpp
//  OpenGL-Test
//
//  Created by clock on 10/6/16.
//  Copyright © 2016 clock. All rights reserved.
//

#ifndef mglRgbColor_hpp
#define mglRgbColor_hpp

#include <stdio.h>
#include "minigl.h"

class mglRgbColor{
public:
    unsigned short R,G,B;
    mglRgbColor(MGLpixel color);
    mglRgbColor(unsigned short R,
                unsigned short G,
                unsigned short B);
    MGLpixel getMglPixel();
};

class mglRgbColorStepper{
public:
    float R_step, G_step, B_step;
    float R_stepped, G_stepped, B_stepped;
    
    void setStepCount(mglRgbColor, mglRgbColor, int);
    MGLpixel getStepMGLColor();
};

void setStepCount(mglRgbColor color_1,
                  mglRgbColor color_2,
                  int step);

MGLpixel getStepMGLColor();

#endif /* mglRgbColor_hpp */

#endif
