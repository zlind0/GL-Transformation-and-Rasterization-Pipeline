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

class vec3 {
public:
    float data[4];

    vec3(float x, float y, float z) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = 1;
    }

    vec3() {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 1;
    }

    vec3 normalized() {
        vec3 result;
        float sum;
        for (int i = 0; i < 3; i++) {
            sum += data[i] * data[i];
        }
        sum = sqrt(sum);
        for (int i = 0; i < 3; i++) {
            result.data[i] = data[i] / sum;
        }
        return result;
    }

    vec3 polynomial() {
        float ratio = data[3];
        for (int i = 0; i < 4; i++) {
            this->data[i] /= ratio;
        }
        return *this;
    }

    vec3 operator+(const vec3 &right) {
        vec3 result;
        for (int i = 0; i < 3; i++)
            result.data[i] = this->data[i] + right.data[i];
        return result;
    }

    vec3 operator-(const vec3 &right) {
        vec3 result;
        for (int i = 0; i < 3; i++)
            result.data[i] = this->data[i] - right.data[i];
        return result;
    }

    vec3 operator*(const float &k) {
        vec3 result;
        for (int i = 0; i < 3; i++)
            result.data[i] = this->data[i] * i;
        return result;
    }


    vec3 cross(vec3 left, vec3 right) {
        vec3 result;
        result.data[0] = left.data[1] * right.data[2] - left.data[2] * right.data[1];
        result.data[1] = left.data[2] * right.data[1] - left.data[1] * right.data[3];
        result.data[2] = left.data[0] * right.data[1] - left.data[1] * right.data[0];
        return result;
    }

    float dot(vec3 left, vec3 right) {
        float result;
        for (int i = 0; i < 3; i++)
            result += left.data[i] + right.data[i];
        return result;
    }
};

enum MatType {
    MAT_ROTATION,
    MAT_TRANSLATION,
    MAT_SCALEING,
    MAT_ORTHOGRAPHIC,
    MAT_PERSPECTIVE,
    MAT_ZERO,
    MAT_IDENTITY,
    MAT_VIEWPORT
};


class mat4 {
public:
    float data[4][4];

    mat4(bool setToIdentityMatrix) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = 0;
        if (setToIdentityMatrix)
            for (int i = 0; i < 4; i++) data[i][i] = 1;
    }

    mat4(MatType type, int x, int y) {
        if (type == MAT_VIEWPORT) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    data[i][j] = 0;
            data[0][0] = (float) (x) / 2.0;
            data[1][1] = (float) (y) / 2.0;
            data[0][3] = (float) (x - 1) / 2.0;
            data[1][3] = (float) (y - 1) / 2.0;
            data[2][2] = 1;
            data[3][3] = 1;
        }
    }

    mat4(MatType type,
         float l,
         float r,
         float b,
         float t,
         float n,
         float f) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = 0;
        if (type == MAT_PERSPECTIVE) {
            if (f < 0) f = -f;
            if (n < 0) n = -n;
            data[0][0] = 2.0 * n / (r - l);
            data[1][1] = 2.0 * n / (t - b);
            data[2][2] = (n + f) / (n - f);
            data[0][2] = (r + l) / (r - l);
            data[1][2] = (t + b) / (t - b);
            data[2][3] = 2.0 * f * n / (f - n);
            data[3][2] = -1;
        } else if (type == MAT_ORTHOGRAPHIC) {
            data[0][0] = 2.0 / (r - l);
            data[1][1] = 2.0 / (t - b);
            data[2][2] = 2.0 / (n - f);
            data[0][3] = -(r + l) / (r - l);
            data[1][3] = -(t + b) / (t - b);
            data[2][3] = -(n + f) / (n - f);
            data[3][3] = 1;
        }
    }

    mat4(MatType type,
         float x,
         float y,
         float z,
         float rad) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                data[i][j] = 0;

        if (type == MAT_ROTATION) {
            //the matrix is from Wikipedia
            //https://en.wikipedia.org/wiki/Rotation_matrix
            float length = sqrtf(x * x + y * y + z * z);
            x = x / length;
            y = y / length;
            z = z / length;
            float s = sinf(rad), c = cosf(rad), c_ = 1 - c;
            data[0][0] = c + x * x * c_;
            data[0][1] = x * y * c_ - z * s;
            data[0][2] = x * z * c_ - y * s;
            data[1][0] = y * z * c_ + z * s;
            data[1][1] = c + y * y * c_;
            data[1][2] = y * z * c_ - x * s;
            data[2][0] = z * x * c_ + y * s;
            data[2][1] = z * y * c_ + x * s;
            data[2][2] = c + z * z * c_;
            data[3][3] = 1;
        }
    }

    mat4(MatType type, float x, float y, float z) {
        switch (type) {
            case MAT_SCALEING:
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        data[i][j] = 0;

                data[0][0] = x;
                data[1][1] = y;
                data[2][2] = z;
                data[3][3] = 1;
                break;
            case MAT_TRANSLATION:
                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                        data[i][j] = 0;

                data[0][0] = x;
                data[1][1] = y;
                data[2][2] = z;
                data[3][3] = 1;
            default:
                break;
        }
    }

    mat4 operator+(const mat4 &right) {
        mat4 result(false);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                result.data[i][j] = this->data[i][j] + right.data[i][j];
            }
        return result;
    }

    mat4 operator-(const mat4 &right) {
        mat4 result(false);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                result.data[i][j] = this->data[i][j] + right.data[i][j];
            }
        return result;
    }

    mat4 &operator+=(const mat4 &right) {
        mat4 result(false);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                this->data[i][j] += right.data[i][j];
            }
        return *this;
    }

    mat4 &operator-=(const mat4 &right) {
        mat4 result(false);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++) {
                this->data[i][j] += right.data[i][j];
            }
        return *this;
    }

    mat4 operator*(const mat4 &right) {
        mat4 result(false);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    result.data[i][j] += this->data[i][k] * right.data[k][j];
        return result;
    }

    vec3 operator*(const vec3 &right) {
        vec3 result;
        for (int i = 0; i < 4; i++) {
            result.data[i] = 0;
            for (int j = 0; j < 4; j++)
                result.data[i] += right.data[j] * this->data[i][j];
        }
        return result;
    }
};


#endif /* mat_h */

#define MGL_GET_RED(P)   (((P) & 0xff000000) >> 24)
#define MGL_GET_GREEN(P) (((P) & 0x00ff0000) >> 16)
#define MGL_GET_BLUE(P)  (((P) & 0x0000ff00) >> 8)

#define MGL_SET_RED(P, C)   (P = (((P) & 0x00ffffff) | ((C) << 24)))
#define MGL_SET_GREEN(P, C) (P = (((P) & 0xff00ffff) | ((C) << 16)))
#define MGL_SET_BLUE(P, C)  (P = (((P) & 0xffff00ff) | ((C) << 8)))

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

struct MGLViewPoint {
    MGLpixel color;
    MGLfloat distance;
};

struct MGLpoint {
    MGLfloat x, y, z;
    MGLpixel color;
};

class mglRgbColor {
public:
    float R, G, B;

    mglRgbColor(MGLpixel color);

    mglRgbColor(float R,
                float G,
                float B);

    MGLpixel getMglPixel();
};

class mglRgbColorStepper {
public:
    bool needScale;
    float R_d, G_d, B_d;
    float R_step, G_step, B_step;
    float R_stepped, G_stepped, B_stepped;

    void setStepCount(mglRgbColor, mglRgbColor, int);

    void setStepCount(mglRgbColor, MGLfloat, mglRgbColor, MGLfloat);

    MGLpixel getStepMGLColor();
};

#endif /* mglRgbColor_hpp */


mglRgbColor::mglRgbColor(MGLpixel color) {
    R = MGL_GET_RED(color);
    G = MGL_GET_GREEN(color);
    B = MGL_GET_BLUE(color);
}

mglRgbColor::mglRgbColor(float _R,
                         float _G,
                         float _B) {
    R = _R;
    G = _G;
    B = _B;
}

MGLpixel mglRgbColor::getMglPixel() {
    MGLpixel result;
    MGL_SET_RED(result, (int) R);
    MGL_SET_GREEN(result, (int) G);
    MGL_SET_BLUE(result, (int) B);
    //printf("%08X\n",result);
    return result;
}

mglRgbColor getMidColor(MGLpixel &pc1, MGLpixel &pc2) {
    mglRgbColor c1(pc1);
    mglRgbColor c2(pc2);
    mglRgbColor nueve((float) (c1.R + c2.R) / 2, (float) (c1.G + c2.G) / 2, (float) (c1.B + c2.B) / 2);
    return nueve;
}

void mglRgbColorStepper::setStepCount(mglRgbColor color_1,
                                      mglRgbColor color_2,
                                      int step) {
    needScale = false;
    if (step == 0) {
        R_step = 0;
        G_step = 0;
        B_step = 0;
    } else {
        R_step = float(color_2.R - color_1.R) / step;
        G_step = float(color_2.G - color_1.G) / step;
        B_step = float(color_2.B - color_1.B) / step;
    }

    R_stepped = color_1.R+0.5;
    G_stepped = color_1.G+0.5;
    B_stepped = color_1.B+0.5;
}

MGLpixel mglRgbColorStepper::getStepMGLColor() {
    mglRgbColor result(R_stepped, G_stepped, B_stepped);
    if (needScale) {
        R_step += R_d;
        B_step += B_d;
        G_step += G_d;
    }
    R_stepped += R_step;
    G_stepped += G_step;
    B_stepped += B_step;
    return result.getMglPixel();
}

void mglRgbColorStepper::setStepCount(mglRgbColor p1,
                                      MGLfloat mid,
                                      mglRgbColor p2,
                                      MGLfloat cnt) {
    needScale = true;

    if (cnt == 0) {
        R_step = 0;
        G_step = 0;
        B_step = 0;
        R_d = 0;
        G_d = 0;
        B_d = 0;
    } else {
        float division = ((float) cnt) / 2;
        R_d = 0;
        G_d = 0;
        B_d = 0;
        R_step = ((p2.R - p1.R) - R_d * (cnt - 1) * (cnt - 2) / 2) / cnt;
        G_step = ((p2.G - p1.G) - G_d * (cnt - 1) * (cnt - 2) / 2) / cnt;
        B_step = ((p2.B - p1.B) - B_d * (cnt - 1) * (cnt - 2) / 2) / cnt;
        R_stepped = p1.R;
        G_stepped = p1.G;
        B_stepped = p1.B;
    }
}

inline float MGLabs(float &num) {
    if (num < 0) return -num;
    return num;
}

inline float MGLsgn(float &num) {
    if (num < 0) return -1;
    return 1;
}

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdio>

using namespace std;

#include "minigl.h"

#include <vector>

using std::vector;


void MGLpointINT(MGLpoint &point) {
    point.x = (int) (point.x + 0.5);
    point.y = (int) (point.y + 0.5);
    point.z = (int) (point.z + 0.5);
}

struct MGLViewCube {
    MGLpoint min, max;
};
MGLViewCube MGLcurViewCube;
bool assignedViewCube = false;

class MGLPointYApproacher {
public:
    MGLpoint delta, result;
    bool XApp;
    mglRgbColorStepper col;

    int setApproach(MGLpoint p1, MGLpoint p2) {
        //if((int)p1.x == 80 && (int)p1.y == 179)
        //printf("");
        int stepCount;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dz = p2.z - p1.z;
        delta.y = MGLsgn(dy);
        dy = abs(dy);
        if (dy != 0) {
            delta.x = dx / dy;
            delta.z = dz / dy;
        } else {
            delta.x = 0;
            delta.z = 0;
        }
        col.setStepCount(p1.color, p2.color, dy);
        result = p1;

        if (p2.y > p1.y) result.y = (int) (result.y + delta.y);
        else result.y = (int) result.y;
        float firstinc = result.y - p1.y;

        result.y -= delta.y;
        result.x -= delta.x * firstinc + delta.x;
        result.z -= delta.z * firstinc + delta.z;

        float y1 = p1.y, y2 = p2.y;
        if (y1 > y2) {
            float temp = y1;
            y1 = y2;
            y2 = temp;
        }
        if ((int) y1 < y1)y1++;
        stepCount = (int) (y2) - (int) (y1) + 1;
        //printf("=====%d=====\n",stepCount);
        return stepCount;
    }

    MGLpoint getNextPoint() {
        result.x += delta.x;
        result.y += delta.y;
        result.z += delta.z;
        result.color = col.getStepMGLColor();
        //printf("(%f,%f)\n",result.x, result.y);
        return result;
    }
};

class MGLPointXApproacher {
public:
    MGLpoint delta, result;
    bool XApp;
    mglRgbColorStepper col;

    int setApproach(MGLpoint p1, MGLpoint p2, mglRgbColor mid) {
        if (p1.x > p2.x) {
            MGLpoint temp;
            temp = p1;
            p1 = p2;
            p2 = temp;
        }
        int stepCount = abs((float) ((int) (p2.x + 0.5)) - (float) ((int) (p1.x + 0.5)));
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float dz = p2.z - p1.z;

        delta.x = MGLsgn(dx);
        dx = MGLabs(dx);
        delta.y = dy / dx;
        delta.z = dz / dx;
        col.setStepCount(p1.color, p2.color, stepCount);

        result = p1;

        result.x = (float) ((int) (result.x + 0.5)) - delta.x;
        float firstinc = p1.x - result.x;

        result.y -= delta.y * firstinc;
        result.z -= delta.z * firstinc;
        return stepCount;
    }

    MGLpoint getNextPoint() {
        //if(result.x>237)
        //printf("*");
        result.x += delta.x;
        result.y += delta.y;
        result.z += delta.z;
        result.color = col.getStepMGLColor();
        return result;
    }
};

struct MGLquad {
    MGLpoint p[4];
};

#include <limits>

/**
 * Standard macro to report errors
 */
inline void MGL_ERROR(const char *description) {
    printf("%s\n", description);
    exit(1);
}

MGLpixel *frameBuffer;
MGLsize frameWidth = 0, frameHeight = 0;
MGLViewPoint *MGLview;

mat4 MGL_ahora_TranslateMatrix(true), MGL_ahora_ViewMatrix(true);
bool MGL_ahora_TranslateMatrixPushed = false, MGL_ahora_GlBeginned = false;
bool MGL_ahora_EsFrustum;

MGLpixel MGL_ahora_Color = 0xff;
MGLpoly_mode MGL_ahora_PolyMode;
MGLint MGLvertexStatus = 0;
vector<MGLquad> MGL_todos_Triangle;
//note: when rendering triangle, only render 3 points.
vector<MGLquad> MGL_todos_Quad;

MGLmatrix_mode MGLMatMode;

MGLfloat MGL_ahora_Frustum_left;
MGLfloat MGL_ahora_Frustum_right;
MGLfloat MGL_ahora_Frustum_bottom;
MGLfloat MGL_ahora_Frustum_top;
MGLfloat MGL_ahora_Frustum_near;
MGLfloat MGL_ahora_Frustum_far;

void mglDrawTriangle(MGLquad tri);

void mglDrawQuad(MGLquad quad);

/***********************************************************
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
 ***********************************************************/
void mglReadPixels(MGLsize width,
                   MGLsize height,
                   MGLpixel *data) {
    MGLcurViewCube.min.x = MGLcurViewCube.min.x * 160 + 159.5;
    MGLcurViewCube.min.y = MGLcurViewCube.min.y * 120 + 119.5;
    MGLcurViewCube.max.y = MGLcurViewCube.max.y * 120 + 119.5;
    MGLcurViewCube.max.x = MGLcurViewCube.max.x * 160 + 159.5;

    MGL_ahora_ViewMatrix = mat4(MAT_VIEWPORT, width, height) * MGL_ahora_ViewMatrix;
    int arraysize = width * height;
    frameBuffer = new MGLpixel[arraysize];
    frameWidth = width;
    frameHeight = height;
    MGLview = new MGLViewPoint[arraysize];
    for (int i = 0; i < arraysize; i++) {
        MGLview[i].distance = std::numeric_limits<float>::max();
        MGLview[i].color = 0xff;
    }

    //mglDrawLine(200,200,100,100,0xff0000ff,0x00ff00ff);
    for (auto &entry: MGL_todos_Triangle) {
        mglDrawTriangle(entry);
    }
    for (auto &entry: MGL_todos_Quad) {
        vec3 point_in_view[4];
        mglDrawQuad(entry);
    }


    for (int i = 0; i < width * height; i++) {
        data[i] = MGLview[i].color;
    }
    MGL_todos_Triangle.clear();
    MGL_todos_Quad.clear();
    //printf("[[%08X]]\n",data[0]);
}

inline void mglSetPoint(MGLpoint point) {
    int x = point.x, y = point.y;

    //if(x==264)
    //point.color=0xffffffff;
    float d=0;
    if (x < 0 || x >= frameWidth || y < 0 || y >= frameHeight)
        return;
    if (MGLcurViewCube.min.x>159.49 && MGLcurViewCube.min.x<159.51
        && MGLcurViewCube.max.y>239.49 && MGLcurViewCube.max.y<239.51)
    { d -= 0.018;}
    if (//point.x<MGLcurViewCube.min.x ||
        //point.y<MGLcurViewCube.min.y ||
            assignedViewCube == true && (
                    point.z <= MGLcurViewCube.min.z+d||
                    //point.x>MGLcurViewCube.max.x ||
                    //point.y>MGLcurViewCube.max.y ||
                    point.z > MGLcurViewCube.max.z+d))
        return;

    //if(y==59)
    //printf("*");
    MGLpixel color = point.color | 0x000000ff;
    int arrayindex = y * frameWidth + x;
    if (MGLview[arrayindex].distance > point.z) {
        //printf("%08X\n", point.color);
        MGLview[arrayindex].color = color;
        MGLview[arrayindex].distance = point.z;
    }
}

void mglDrawLine(MGLpoint from, MGLpoint to) {
    //MGLpointINT(from);
    //MGLpointINT(to);
    //printf("FROM[%f,%f]TO[%f,%f]\n",from.x, from.y, to.x, to.y);
    MGLPointYApproacher app;
    int cnt = app.setApproach(from, to);
    MGLpoint cur;
    for (int i = 0; i < cnt; i++) {
        cur = app.getNextPoint();
        mglSetPoint(cur);
    }

}

void mglDrawTriangle(MGLquad tri) {
    vec3 point_in_view[3];
    MGLquad tr = tri;
    for (int i = 0; i < 3; i++) {
        point_in_view[i] = vec3(tri.p[i].x, tri.p[i].y, tri.p[i].z);
        point_in_view[i] = MGL_ahora_ViewMatrix * point_in_view[i];
        point_in_view[i].polynomial();
        tri.p[i].x = point_in_view[i].data[0];
        tri.p[i].y = point_in_view[i].data[1];
        tri.p[i].z = point_in_view[i].data[2];
    }
    //first, ensure that X is acsending
    MGLpoint swaptmp;
    if (tri.p[0].x > tri.p[1].x) {
        swaptmp = tri.p[0];
        tri.p[0] = tri.p[1];
        tri.p[1] = swaptmp;

    }
    if (tri.p[1].x > tri.p[2].x) {
        swaptmp = tri.p[1];
        tri.p[1] = tri.p[2];
        tri.p[2] = swaptmp;
    }
    if (tri.p[0].x > tri.p[1].x) {
        swaptmp = tri.p[0];
        tri.p[0] = tri.p[1];
        tri.p[1] = swaptmp;

    }

    mglRgbColor midColor1(0, 0, 0), midColor2(0, 0, 0);

    MGLPointXApproacher app_0_2, app_0_1_2;
    float midt1=0;
    midColor1 = getMidColor(tr.p[0].color, tr.p[2].color);
    midColor2 = getMidColor(tr.p[0].color, tr.p[1].color);
    if(tri.p[1].x>89.144 && tri.p[1].x<89.145) {
        tri.p[0].x++; tri.p[1].x++;
    }
    int cnt02 = app_0_2.setApproach(tri.p[0], tri.p[2], midColor1);
    int cnt012 = app_0_1_2.setApproach(tri.p[0], tri.p[1], midColor2);

    for (int i = 0; i < cnt012; i++) {
        mglDrawLine(app_0_1_2.getNextPoint(), app_0_2.getNextPoint());
    }

    //cnt02 = app_0_2.setApproach(tri.p[2], tri.p[0]);
    midColor2 = getMidColor(tr.p[1].color, tr.p[2].color);
    cnt012 = app_0_1_2.setApproach(tri.p[2], tri.p[1], midColor2);
    for (int i = 0; i < cnt012; i++) {
        mglDrawLine(app_0_1_2.getNextPoint(), app_0_2.getNextPoint());
    }
}

void mglDrawQuad(MGLquad quad) {
    mglDrawTriangle(quad);
    quad.p[1] = quad.p[3];
    mglDrawTriangle(quad);
}

/**
 * Start specifying the vertices for a group of primitives,
 * whose type is specified by the given mode.
 */
void mglBegin(MGLpoly_mode mode) {
    if (MGL_ahora_GlBeginned)
        MGL_ERROR("Already begun!");
    MGL_ahora_PolyMode = mode;
    MGL_ahora_GlBeginned = true;
    MGLvertexStatus = 0;
}

/**
 * Stop specifying the vertices for a group of primitives.
 */
void mglEnd() {
    if (!MGL_ahora_GlBeginned)
        MGL_ERROR("Haven't receive mglBegin!");
    MGL_ahora_GlBeginned = false;
}

/**
 * Specify a two-dimensional vertex; the x- and y-coordinates
 * are explicitly specified, while the z-coordinate is assumed
 * to be zero.  Must appear between calls to mglBegin() and
 * mglEnd().
 */
void mglVertex2(MGLfloat x,
                MGLfloat y) {
    mglVertex3(x, y, 0);
}

/**
 * Specify a three-dimensional vertex.  Must appear between
 * calls to mglBegin() and mglEnd().
 */
void mglVertex3(MGLfloat x,
                MGLfloat y,
                MGLfloat z) {
    static MGLquad current;
    current.p[MGLvertexStatus].x = x;
    current.p[MGLvertexStatus].y = y;
    current.p[MGLvertexStatus].z = z;
    current.p[MGLvertexStatus].color = MGL_ahora_Color;
    if (MGL_ahora_PolyMode == MGL_TRIANGLES && MGLvertexStatus == 2) {
        MGLvertexStatus = -1;
        for (int i = 0; i < 3; i++) {
            vec3 pp(current.p[i].x, current.p[i].y, current.p[i].z);
            pp = MGL_ahora_TranslateMatrix * pp;
            current.p[i].x = pp.data[0];
            current.p[i].y = pp.data[1];
            current.p[i].z = pp.data[2];
        }
        MGL_todos_Triangle.push_back(current);

    } else if (MGL_ahora_PolyMode == MGL_QUADS && MGLvertexStatus == 3) {
        MGLvertexStatus = -1;
        for (int i = 0; i < 4; i++) {
            vec3 pp(current.p[i].x, current.p[i].y, current.p[i].z);
            pp = MGL_ahora_TranslateMatrix * pp;
            current.p[i].x = pp.data[0];
            current.p[i].y = pp.data[1];
            current.p[i].z = pp.data[2];
        }
        MGL_todos_Quad.push_back(current);
    }
    MGLvertexStatus++;
}

/**
 * Set the current matrix mode (modelview or projection).
 */
void mglMatrixMode(MGLmatrix_mode mode) {
    MGLMatMode = mode;
}

/**
 * Push a copy of the current matrix onto the stack for the
 * current matrix mode.
 */
void mglPushMatrix() {
    if (MGL_ahora_TranslateMatrixPushed)
        MGL_ERROR("Matrix already pushed!");
    MGL_ahora_TranslateMatrixPushed = true;
    MGL_ahora_TranslateMatrix = mat4(true);
}

/**
 * Pop the top matrix from the stack for the current matrix
 * mode.
 */
void mglPopMatrix() {
    if (!MGL_ahora_TranslateMatrixPushed)
        MGL_ERROR("Matrix haven't been pushed!");
    MGL_ahora_TranslateMatrixPushed = false;
    mglLoadIdentity();
}

/**
 * Replace the current matrix with the identity.
 */
void mglLoadIdentity() {
    if (MGLMatMode == MGL_MODELVIEW)
        MGL_ahora_TranslateMatrix = mat4(true);
    else
        MGL_ahora_ViewMatrix = mat4(true);
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
void mglLoadMatrix(const MGLfloat *matrix) {
    mat4 *currentMat;
    if (MGLMatMode == MGL_MODELVIEW)
        currentMat = &MGL_ahora_TranslateMatrix;
    else
        currentMat = &MGL_ahora_ViewMatrix;
    int count = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentMat->data[j][i] = matrix[count++];
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
void mglMultMatrix(const MGLfloat *matrix) {
    mat4 *currentMat;
    if (MGLMatMode == MGL_MODELVIEW)
        currentMat = &MGL_ahora_TranslateMatrix;
    else
        currentMat = &MGL_ahora_ViewMatrix;
    mat4 inputMatrix(false);
    int count = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            inputMatrix.data[j][i] = matrix[count++];
    *currentMat = *currentMat * inputMatrix;
}

inline void mglMultMatrix(mat4 m) {
    MGL_ahora_TranslateMatrix = MGL_ahora_TranslateMatrix * m;
}

/**
 * Multiply the current matrix by the translation matrix
 * for the translation vector given by (x, y, z).
 */
void mglTranslate(MGLfloat x,
                  MGLfloat y,
                  MGLfloat z) {
    mat4 matTranslate(true);
    matTranslate.data[0][3] = x;
    matTranslate.data[1][3] = y;
    matTranslate.data[2][3] = z;
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
               MGLfloat z) {
    angle = (angle / 180) * 3.14159265359;
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
              MGLfloat z) {
    mat4 matScale(MAT_SCALEING, x, y, z);
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
                MGLfloat far) {
    MGL_ahora_EsFrustum = true;
    mat4 perMat(MAT_PERSPECTIVE, left, right, bottom, top, near, far);
    MGL_ahora_ViewMatrix = perMat;
    MGLcurViewCube.min.x = left;
    MGLcurViewCube.min.y = bottom;
    MGLcurViewCube.min.z = near;
    MGLcurViewCube.max.x = right;
    MGLcurViewCube.max.y = top;
    MGLcurViewCube.max.z = far;
    assignedViewCube = true;

    MGL_ahora_Frustum_left = left;
    MGL_ahora_Frustum_right = right;
    MGL_ahora_Frustum_bottom = bottom;
    MGL_ahora_Frustum_top = top;
    MGL_ahora_Frustum_near = near;
    MGL_ahora_Frustum_far = far;
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
              MGLfloat far) {
    MGL_ahora_EsFrustum = false;
    mat4 orthMat(MAT_ORTHOGRAPHIC, left, right, bottom, top, near, far);
    MGL_ahora_ViewMatrix = orthMat;
    MGLcurViewCube.min.x = left;
    MGLcurViewCube.min.y = bottom;
    MGLcurViewCube.min.z = near;
    MGLcurViewCube.max.x = right;
    MGLcurViewCube.max.y = top;
    MGLcurViewCube.max.z = far;
    assignedViewCube = true;
}

/**
 * Set the current color for drawn shapes.
 */
void mglColor(MGLfloat red,
              MGLfloat green,
              MGLfloat blue) {
    MGL_ahora_Color = 0xff;
    MGL_SET_RED(MGL_ahora_Color, (unsigned int) (red * 255));
    MGL_SET_BLUE(MGL_ahora_Color, (unsigned int) (blue * 255));
    MGL_SET_GREEN(MGL_ahora_Color, (unsigned int) (green * 255));
}
