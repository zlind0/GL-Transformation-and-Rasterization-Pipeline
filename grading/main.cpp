/**
 * main.cpp
 * -------------------------------
 * This is simple testbed for your miniGL implementation.
 *
 * The program can be used in one of three ways:
 *
 * Option 1:  minigl -c 00.txt
 *
 * This will run the commands in 00.txt using your solution and OpenGL.  The
 * program will draw the solution (using OpenGL) in the left half of the window
 * and your result in the right half of the window.  This is good for getting a
 * quick idea if your program is working.
 *
 * Option 2:  minigl -d 00.txt
 *
 * This will run the commands in 00.txt using your solution and OpenGL.  The
 * program generates three png files.  gl.png is the OpenGL solution, mgl.png is
 * your result, and diff.png is a file pointing out the differences pixel by
 * pixel.  The more white a pixel is, the greater the difference.  This version
 * is useful for getting your pixels in exactly the right places.
 *
 * This version also computes the total error.  The output will look something
 * like "pjp2FQRDtCqwZh5r0MP7Vf3h 0.39".  This indicates that about 0.39% of the pixels in
 * the image are completely wrong.  Pixels can be approximate; a pixel whose
 * color is slightly off contributes little to the total error.
 *
 * Option 3:  minigl -t 00.txt
 *
 * This will run the commands in 00.txt using your solution only, timing how
 * long your solution took to run.  This will be useful if you intend to
 * optimize your code.  The output is a single number, the runtime in
 * milliseconds.
 */
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include "minigl.h"
#include <functional>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

// Change these to test MiniGL at different resolutions
int width=320,height=240;

std::vector<std::function<void()> > commands_gl,commands_mgl;

void Parse(const char* test_file);
void Dump_png(MGLpixel* data,int width,int height,const char* filename);

void Execute_Commands()
{
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for(size_t i=0;i<commands_mgl.size();i++)
		commands_mgl[i]();

	for(size_t i=0;i<commands_gl.size();i++)
		commands_gl[i]();

	MGLpixel pixel_data[width*height];
	for(int i=0;i<width*height;i++) pixel_data[i]=0;

	mglReadPixels(width,height,pixel_data);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2f(1,0);
	glDrawPixels(width,height,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,(GLvoid*)pixel_data);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glFlush();
}

void Opengl_Setup(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(2*width, height);
	glutCreateWindow("Test OpenGL Program");
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_FRONT);
}

void Side_By_Side(int argc, char** argv)
{
	Opengl_Setup(argc,argv);
	glutDisplayFunc(Execute_Commands);
	glutMainLoop();
}

void Compare(int argc, char** argv)
{
	Opengl_Setup(argc,argv);
	Execute_Commands();

	std::string base=argv[2],file_gl=base+"-gl.png",file_mgl=base+"-mgl.png",file_diff=base+"-diff.png";
	MGLpixel pixel_data_gl[width*height];
	MGLpixel pixel_data_mgl[width*height];
	MGLpixel pixel_data_diff[width*height];
	for(int i=0;i<width*height;i++) pixel_data_gl[i]=0;
	for(int i=0;i<width*height;i++) pixel_data_mgl[i]=0;
	for(int i=0;i<width*height;i++) pixel_data_diff[i]=0;
	glReadPixels(0,0,width,height,GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,(GLvoid*)pixel_data_gl);
	glReadPixels(width,0,width,height,GL_BGRA,GL_UNSIGNED_INT_8_8_8_8_REV,(GLvoid*)pixel_data_mgl);
	int total_diff = 0;
    
    //printf("[%08X,%08X]\n",pixel_data_gl[0],pixel_data_mgl[0]);
	for(int i=0;i<width*height;i++)
	{
        
        MGLpixel A=pixel_data_gl[i];
        MGLpixel B=pixel_data_mgl[i];
        //if(MGL_GET_GREEN(A)==255)printf("(%d,%d)",i%width, i/width);
		int r=abs(MGL_GET_RED(A)-MGL_GET_RED(B));
		int g=abs(MGL_GET_GREEN(A)-MGL_GET_GREEN(B));
		int b=abs(MGL_GET_BLUE(A)-MGL_GET_BLUE(B));
		int diff=std::max(std::max(r,g),b);
		total_diff += diff;
		MGLpixel diff_color=0xFF000000|(diff*0x10101);
		pixel_data_diff[i]=diff_color;
        //if(diff>0)printf("(%d,%d)",i%width, i/width);
	}
	Dump_png(pixel_data_gl,width,height,"gl.png");
	Dump_png(pixel_data_mgl,width,height,"mgl.png");
	Dump_png(pixel_data_diff,width,height,"diff.png");
	printf("pjp2FQRDtCqwZh5r0MP7Vf3h %.2f\n",total_diff/(2.55*width*height));
}

void Benchmark()
{
	timespec t0,t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	MGLpixel pixel_data[width*height];
	for(int i=0;i<width*height;i++) pixel_data[i]=0;
	for(size_t i=0;i<commands_mgl.size();i++)
		commands_mgl[i]();
	mglReadPixels(width,height,pixel_data);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	long long a0 = (long long)t0.tv_sec*1000000000+t0.tv_nsec;
	long long a1 = (long long)t1.tv_sec*1000000000+t1.tv_nsec;
	printf("pjp2FQRDtCqwZh5r0MP7Vf3h %.2f\n",(a1-a0)*1e-6);
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		std::cerr<<"Usage: "<<argv[0]<<" <-c|-d|-t> <test-file>"<<std::endl;
		exit(EXIT_FAILURE);
	}
	Parse(argv[2]);

	assert(argv[1][0]=='-' && argv[1][2]==0);
	switch(argv[1][1])
	{
		case 'c': Side_By_Side(argc,argv); break;
		case 'd': Compare(argc,argv); break;
		case 't': Benchmark(); break;
		default: printf("unrecognized option: %s\n",argv[1]); return EXIT_FAILURE;
	}

	return 0;
}

int _main(){
    char* test_arg[3]={"self", "-c", "02.txt"};
    Parse(test_arg[2]);
    Compare(3, test_arg);
	MGLpixel pixel_data_mgl[width*height];
	for(int i=0;i<width*height;i++) pixel_data_mgl[i]=0xff000000;
	mglReadPixels(width,height,pixel_data_mgl);
	Dump_png(pixel_data_mgl,width,height,"mgl.png");
}
