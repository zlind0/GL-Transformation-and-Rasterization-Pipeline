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
