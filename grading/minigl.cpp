/**
 * minigl.cpp
 * -------------------------------
 * Implement miniGL here.
 *
 * You may include minigl.h and any of the standard C++ libraries.
 * No other includes are permitted.  Other preprocessing directives
 * are also not permitted.  These requirements are strictly
 * enforced.  Be sure to run a test grading to make sure your file
 * passes the sanity tests.
 */

//
//  mglRgbColor.cpp
//  OpenGL-Test
//
//  Created by clock on 10/6/16.
//  Copyright © 2016 clock. All rights reserved.
//
#include "minigl.h"

mglRgbColor::mglRgbColor(MGLpixel color){
    R=MGL_GET_RED(color);
    G=MGL_GET_GREEN(color);
    B=MGL_GET_BLUE(color);
}

mglRgbColor::mglRgbColor(unsigned short _R,
                         unsigned short _G,
                         unsigned short _B){
    R=_R;
    G=_G;
    B=_B;
}

MGLpixel mglRgbColor::getMglPixel(){
    MGLpixel result;
    MGL_SET_RED(result, R);
    MGL_SET_GREEN(result, G);
    MGL_SET_BLUE(result, B);
    //printf("%08X\n",result);
    return result;
}


void mglRgbColorStepper::setStepCount(mglRgbColor color_1,
                                      mglRgbColor color_2,
                                      int step){
    if(step == 0){
        R_step=0;G_step=0;B_step=0;
    }
    else{
        R_step = float(color_2.R - color_1.R)/step;
        G_step = float(color_2.G - color_1.G)/step;
        B_step = float(color_2.B - color_1.B)/step;
    }
    
    R_stepped = color_1.R;
    G_stepped = color_1.G;
    B_stepped = color_1.B;
}

MGLpixel mglRgbColorStepper::getStepMGLColor(){
    mglRgbColor result(R_stepped, G_stepped, B_stepped);
    R_stepped+=R_step;
    G_stepped+=G_step;
    B_stepped+=B_step;
    return result.getMglPixel();
}



#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdio>

using namespace std;


/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char* description) {
	printf("%s\n", description);
	exit(1);
}

MGLpixel *frameBuffer;
MGLsize frameWidth=0, frameHeight=0;
MGLViewPoint *MGLview;

mat4 MGLcurTranslateMatrix(true), MGLcurViewMatrix(true);
bool MGLcurTranslateMatrixPushed=false, MGLcurGlBeginned=false;

MGLpixel MGLcurColor=0xff;
MGLpoly_mode MGLcurPolyMode;
MGLint MGLvertexStatus=0;
vector<MGLquad> MGLallTriangle;
//note: when rendering triangle, only render 3 points.
vector<MGLquad> MGLallQuad;

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
		MGLpixel *data)
{
    MGLcurViewMatrix=mat4(MAT_VIEWPORT, width, height)*MGLcurViewMatrix;
	frameBuffer = new MGLpixel[width*height];
	frameWidth = width;
	frameHeight = height;
    for(int i=0;i<width*height;i++){
        frameBuffer[i]=0xff;
    }
    MGLview = new MGLViewPoint[width*height];
    

	//mglDrawLine(200,200,100,100,0xff0000ff,0x00ff00ff);
    for(auto &entry: MGLallTriangle){
        vec3 point_in_view1(entry.p[0].x, entry.p[0].y, entry.p[0].z);
        point_in_view1=MGLcurViewMatrix*point_in_view1;
        vec3 point_in_view2(entry.p[1].x, entry.p[1].y, entry.p[1].z);
        point_in_view2=MGLcurViewMatrix*point_in_view2;
        vec3 point_in_view3(entry.p[2].x, entry.p[2].y, entry.p[2].z);
        point_in_view3=MGLcurViewMatrix*point_in_view3;
        mglDrawTriangle(
        	point_in_view1.data[0]+0.5,
        	point_in_view1.data[1]+0.5,
        	point_in_view2.data[0]+0.5,
        	point_in_view2.data[1]+0.5,
        	point_in_view3.data[0]+0.5,
        	point_in_view3.data[1]+0.5,
        	entry.p[0].color,
        	entry.p[1].color,
        	entry.p[2].color,2);
        /*mglDrawLine(
            point_in_view1.data[0],
	    	point_in_view1.data[1],
	    	point_in_view2.data[0],
	    	point_in_view2.data[1],
	    	entry.p[i].color,
	    	entry.p[(i+1)%3].color);*/
    }
    for(auto &entry: MGLallQuad){
        vec3 point_in_view1(entry.p[0].x, entry.p[0].y, entry.p[0].z);
        point_in_view1=MGLcurViewMatrix*point_in_view1;
        vec3 point_in_view2(entry.p[1].x, entry.p[1].y, entry.p[1].z);
        point_in_view2=MGLcurViewMatrix*point_in_view2;
        vec3 point_in_view3(entry.p[2].x, entry.p[2].y, entry.p[2].z);
        point_in_view3=MGLcurViewMatrix*point_in_view3;
        vec3 point_in_view4(entry.p[3].x, entry.p[3].y, entry.p[3].z);
        point_in_view4=MGLcurViewMatrix*point_in_view4;
        mglDrawTriangle(
                        point_in_view1.data[0]+0.5,
                        point_in_view1.data[1]+0.5,
                        point_in_view2.data[0]+0.5,
                        point_in_view2.data[1]+0.5,
                        point_in_view3.data[0]+0.5,
                        point_in_view3.data[1]+0.5,
                        entry.p[0].color,
                        entry.p[1].color,
                        entry.p[2].color,2);
        mglDrawTriangle(
                        point_in_view4.data[0]+0.5,
                        point_in_view4.data[1]+0.5,
                        point_in_view1.data[0]+0.5,
                        point_in_view1.data[1]+0.5,
                        point_in_view3.data[0]+0.5,
                        point_in_view3.data[1]+0.5,
                        entry.p[3].color,
                        entry.p[0].color,
                        entry.p[2].color,2);
        /*mglDrawLine(
         point_in_view1.data[0],
         point_in_view1.data[1],
         point_in_view2.data[0],
         point_in_view2.data[1],
         entry.p[i].color,
         entry.p[(i+1)%3].color);*/
    }
	
    
	for(int i=0;i<width*height;i++){
		data[i]=frameBuffer[i];
	}
    MGLallTriangle.clear();
    MGLallQuad.clear();
    //printf("[[%08X]]\n",data[0]);
}
inline void mglDrawPixelWithDis(int x, int y, float dis, MGLpixel color){
    if(x<0 || x>=frameWidth || y<0 || y>= frameHeight) return;
    if(MGLview[x*frameWidth+y].distance>dis){
        MGLview[y*frameWidth+x].color = color;
    }
}

inline void mglSetPixel(int x, int y, MGLpixel color){
    //if(x==280) printf("*");
	if(x<0 || x>=frameWidth || y<0 || y>= frameHeight) return;
    frameBuffer[y*frameWidth+x]=color;//printf("[%08X]\n",color);
	//printf("(%d,%d)-",x,y);
}

void mglDrawLine(int x0, int y0, int x1, int y1, MGLpixel stColor, MGLpixel endColor){
	float dx = x1 - x0;
	float dy = y1 - y0;

	float m = dy/dx;
	float b, y;

	mglRgbColor mglcolor_st(stColor), mglcolor_end(endColor);

	mglRgbColorStepper stepCol;


	if(m>=-1 && m<=1){
		if(x0>x1){
			float t=x0;x0=x1; x1=t;
			t=y0;y0=y1;y1=t;
			stepCol.setStepCount(mglcolor_end, mglcolor_st, x1-x0);
		}
		else{
			stepCol.setStepCount(mglcolor_st, mglcolor_end, x1-x0);
		}
		b = y0 - m*x0;
		for(int x = x0; x < x1; ++x){
			y=m*(float)x + b;
            mglSetPixel(x, y, stepCol.getStepMGLColor());
		}
	}else{
		if(y0>y1){
			float t=x0;x0=x1; x1=t;
			t=y0;y0=y1;y1=t;
			stepCol.setStepCount(mglcolor_end, mglcolor_st, y1-y0);
		}
        else{
            stepCol.setStepCount(mglcolor_st, mglcolor_end, y1-y0);
        }
		m=1/m;
		b=x0-m*y0;
		for(int y = y0; y < y1; ++y){
			mglSetPixel(m*(float)y + b, y, stepCol.getStepMGLColor());
		}
	}

}

void mglDrawTriangle(int x0,
					 int y0,
					 int x1,
					 int y1,
					 int x2,
					 int y2,
					 MGLpixel color0,
					 MGLpixel color1,
					 MGLpixel color2,
					 int drawLeft){
    x2--;y2--;
	float dx1 = x1 - x0, dx2 = x2 - x0;
	float dy1 = y1 - y0, dy2 = y2 - y0;

	float m1 = dy1/dx1, m2 = dy2/dx2;
	float b1, b2;

	mglRgbColor mglcolor_0(color0), mglcolor_1(color1), mglcolor_2(color2);

	mglRgbColorStepper stepCol1, stepCol2;



	if(m1>=-1 && m1<=1){
		if(x0>x1){
			float t=x0;x0=x1; x1=t;
			t=y0;y0=y1;y1=t;
			stepCol1.setStepCount(color1, color0, x1-x0);
		}
		else{
			stepCol1.setStepCount(color0, color1, x1-x0);
		}
		b1 = y0 - m1*x0;
		float _y;

		for(int x_1 = x0; x_1 <= x1; ++x_1){
			_y = m1*x_1 + b1;
			mglDrawLine(x_1, _y, x2, y2, stepCol1.getStepMGLColor(),color2);
		}
	}else{
		if(y0>y1){
			float t=x0;x0=x1; x1=t;
			t=y0;y0=y1;y1=t;
			stepCol1.setStepCount(color1, color0, y1-y0);
		}
		else{
			stepCol1.setStepCount(color0, color1, y1-y0);
		}
		m1=1/m1;
		b1=x0-m1*y0;

		for(int y_1 = y0; y_1 <= y1; ++y_1){
			//mglSetPixel(m*y + b, y, stepCol.getStepMGLColor());
			mglDrawLine(m1*y_1+b1, y_1, x2, y2, stepCol1.getStepMGLColor(), color2);
		}
	}
	//if(drawLeft>0)mglDrawTriangle(x1, y1, x2, y2, x0, y0, color1, color2, color0, drawLeft-1);
}
/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode)
{
	if(MGLcurGlBeginned)
		MGL_ERROR("Already begun!");
	MGLcurPolyMode = mode;
	MGLcurGlBeginned = true;
    MGLvertexStatus = 0;
}

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd()
{
	if(!MGLcurGlBeginned)
		MGL_ERROR("Haven't receive mglBegin!");
	MGLcurGlBeginned = false;
}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
		MGLfloat y)
{
	mglVertex3(x, y, 0);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
		MGLfloat y,
		MGLfloat z)
{
    static MGLquad current;
    current.p[MGLvertexStatus].x=x;
    current.p[MGLvertexStatus].y=y;
    current.p[MGLvertexStatus].z=z;
    current.p[MGLvertexStatus].color=MGLcurColor;
    if(MGLcurPolyMode == MGL_TRIANGLES && MGLvertexStatus==2){
        MGLvertexStatus=-1;
    	for(int i=0;i<3;i++){
    		vec3 pp(current.p[i].x, current.p[i].y, current.p[i].z);
    		pp=MGLcurTranslateMatrix*pp;
            current.p[i].x=pp.data[0];
            current.p[i].y=pp.data[1];
            current.p[i].z=pp.data[2];
    	}
        MGLallTriangle.push_back(current);
        
    }
    else if(MGLcurPolyMode == MGL_QUADS && MGLvertexStatus==3){
        MGLvertexStatus=-1;
    	for(int i=0;i<4;i++){
    		vec3 pp(current.p[i].x, current.p[i].y, current.p[i].z);
    		pp=MGLcurTranslateMatrix*pp;
            current.p[i].x=pp.data[0];
            current.p[i].y=pp.data[1];
            current.p[i].z=pp.data[2];
    	}
        MGLallQuad.push_back(current);
    }
    MGLvertexStatus++;
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode)
{
    
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix()
{
	if(MGLcurTranslateMatrixPushed) 
		MGL_ERROR("Matrix already pushed!");
	MGLcurTranslateMatrixPushed=true;
	MGLcurTranslateMatrix = mat4(true);
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix()
{
	if(!MGLcurTranslateMatrixPushed)
		MGL_ERROR("Matrix haven't been pushed!");
	MGLcurTranslateMatrixPushed = false;
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity()
{
    MGLcurTranslateMatrix = mat4(true);
}

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
void mglLoadMatrix(const MGLfloat *matrix)
{
    int count = 0;
    for(int i=0;i<4;i++)
        for(int j=0; j<4; j++)
            MGLcurTranslateMatrix.data[i][j]=matrix[count++];
}

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
void mglMultMatrix(const MGLfloat *matrix)
{
    mat4 inputMatrix(false);
    int count = 0;
    for(int i=0;i<4;i++)
        for(int j=0; j<4; j++)
            inputMatrix.data[i][j]=matrix[count++];
    MGLcurTranslateMatrix = MGLcurTranslateMatrix*inputMatrix;
}

inline void mglMultMatrix(const mat4 m){
	MGLcurTranslateMatrix = MGLcurTranslateMatrix*m;
}
/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
		MGLfloat y,
		MGLfloat z)
{
	mat4 matTranslate(true);
	matTranslate.data[0][3]=x;
	matTranslate.data[1][3]=y;
	matTranslate.data[2][3]=z;
	mglMultMatrix(matTranslate);
}

/**
 * Multiply the current matrix by the rotation matrix
 * for a rotation of (angle) degrees about the vector
 * from the origin to the point (x, y, z).
 */
void mglRotate(MGLfloat angle,
		MGLfloat x,
		MGLfloat y,
		MGLfloat z)
{
    mat4 matRotate(MAT_ROTATION, x, y, z, angle);
    mglMultMatrix(matRotate);
    //first, assume that the rotate vector is the Z axis
    //this require rotate the original axises to new axises
    
}

/**
 * Multiply the current matrix by the scale matrix
 * for the given scale factors.
 */
void mglScale(MGLfloat x,
		MGLfloat y,
		MGLfloat z)
{
    mat4 matScale(MAT_SCALEING);
    mglMultMatrix(matScale);
}

/**
 * Multiply the current matrix by the perspective matrix
 * with the given clipping plane coordinates.
 */
void mglFrustum(MGLfloat left,
		MGLfloat right,
		MGLfloat bottom,
		MGLfloat top,
		MGLfloat near,
		MGLfloat far)
{
    mat4 perMat(MAT_PERSPECTIVE, left, right, bottom, top, near, far);
    MGLcurViewMatrix = perMat;
}

/**
 * Multiply the current matrix by the orthographic matrix
 * with the given clipping plane coordinates.
 */
void mglOrtho(MGLfloat left,
		MGLfloat right,
		MGLfloat bottom,
		MGLfloat top,
		MGLfloat near,
		MGLfloat far)
{
    mat4 orthMat(MAT_ORTHOGRAPHIC, left, right, bottom, top, near, far);
    MGLcurViewMatrix = orthMat;
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
		MGLfloat green,
		MGLfloat blue)
{
    MGL_SET_RED(MGLcurColor, (unsigned int)(red*255));
    MGL_SET_BLUE(MGLcurColor, (unsigned int)(blue*255));
    MGL_SET_GREEN(MGLcurColor, (unsigned int)(green*255));
}
