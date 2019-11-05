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
