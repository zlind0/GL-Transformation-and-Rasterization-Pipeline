//
//  mglRgbColor.cpp
//  OpenGL-Test
//
//  Created by clock on 10/6/16.
//  Copyright © 2016 clock. All rights reserved.
//

#include "mglRgbColor.h"

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
