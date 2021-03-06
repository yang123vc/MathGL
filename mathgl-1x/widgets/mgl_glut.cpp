/***************************************************************************
 * mgl_glut.cpp is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#define GLUT_NO_LIB_PRAGMA
#define GLUT_NO_WARNING_DISABLE

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#else
	#ifdef _MSC_VER
		#include <windows.h>
		#include <GL/gl.h>
		#include "glut.h"
	#else
		#include <GL/gl.h>
		#include <GL/glut.h>
	#endif
#endif

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mgl/mgl_glut.h"
#include "mgl/mgl_c.h"
#include "mgl/mgl_f.h"

mglGraphGLUT *_mgl_glwnd;
void _mgl_key_up(unsigned char ch,int ,int );
//-----------------------------------------------------------------------------
void _mgl_timer(int)
{
	if(!_mgl_glwnd)	return;
	if(_mgl_glwnd->tt)
	{
		_mgl_glwnd->curr_fig++;
		if(_mgl_glwnd->curr_fig > _mgl_glwnd->NumFig)
			_mgl_glwnd->curr_fig = 1;
		glutPostRedisplay();
	}
	glutTimerFunc(int(_mgl_glwnd->Delay*1000),_mgl_timer,0);
}
//-----------------------------------------------------------------------------
void mglGraphGLUT::ToggleAlpha()	{	_mgl_key_up('r',0,0);	}
void mglGraphGLUT::ToggleLight()	{	_mgl_key_up('f',0,0);	}
void mglGraphGLUT::ToggleNo()		{	_mgl_key_up('n',0,0);	}
void mglGraphGLUT::Update()			{	_mgl_key_up(' ',0,0);	}
void mglGraphGLUT::ReLoad(bool o)	{	_mgl_key_up(o?']':'[',0,0);	}
void mglGraphGLUT::NextFrame()		{	_mgl_key_up('.',0,0);	}
void mglGraphGLUT::PrevFrame()		{	_mgl_key_up(',',0,0);	}
void mglGraphGLUT::Animation()		{	_mgl_key_up('m',0,0);	}
//-----------------------------------------------------------------------------
void _mgl_key_up(unsigned char ch,int ,int )
{
	if(!_mgl_glwnd)	return;
	static bool Alpha=false;
	static bool Light=false;
	static mreal rL=3,tL=0,pL=0;

	if(ch=='h')
	{
		printf("Use 'a', 'd', 'w', 's', 'q', 'e' for changing view angles\n");
		printf("Use 'j', 'l', 'i', 'k' for changing light angles\n");
		printf("Use 'u', 'o' for changing distance to light\n");
		printf("Use 'r' for switching transparency\n");
		printf("Use 'f' for switching lightning\n");
		printf("Use 'T' for exporting to TIFF file\n");
		printf("Use 'J' for exporting to JPEG file\n");
		printf("Use 'P' for exporting to PNG file\n");
		printf("Use ',', '.' for show other frames\n");
		printf("Use 'm' for view movie\n");
		printf("Use 'h' for view this text\n");
		printf("Use 'x' for exit\n");

	}
	if(ch=='w')	_mgl_glwnd->View(-10,0,0);
	if(ch=='s')	_mgl_glwnd->View(10,0,0);
	if(ch=='a')	_mgl_glwnd->View(0,-10,0);
	if(ch=='d')	_mgl_glwnd->View(0,10,0);
	if(ch=='q')	_mgl_glwnd->View(0,0,-10);
	if(ch=='e')	_mgl_glwnd->View(0,0,10);
	if(ch=='n')	_mgl_glwnd->Identity();
	if(ch==',')
		_mgl_glwnd->curr_fig = _mgl_glwnd->curr_fig == 1 ? _mgl_glwnd->NumFig : _mgl_glwnd->curr_fig-1;
	if(ch=='.')
		_mgl_glwnd->curr_fig = _mgl_glwnd->curr_fig == _mgl_glwnd->NumFig ? 1 : _mgl_glwnd->curr_fig+1;
	if(ch=='r')	Alpha = !Alpha;
	if(ch=='f')	Light = !Light;
	if(ch=='u')	rL += 0.1;
	if(ch=='o')	rL -= 0.1;
	if(ch=='i')	tL += M_PI*0.1;
	if(ch=='k')	tL -= M_PI*0.1;
	if(ch=='l')	pL += 2*M_PI*0.1;
	if(ch=='j')	pL -= 2*M_PI*0.1;
	if(ch=='[' && _mgl_glwnd->LoadFunc)
	{
		glDeleteLists(1,_mgl_glwnd->NumFig);
		_mgl_glwnd->LoadFunc(false, 0);
		(_mgl_glwnd->DrawFunc)(_mgl_glwnd,_mgl_glwnd->FuncPar);
		_mgl_glwnd->Finish();
	}
	if(ch==']' && _mgl_glwnd->LoadFunc)
	{
		glDeleteLists(1,_mgl_glwnd->NumFig);
		_mgl_glwnd->LoadFunc(true, 0);
		(_mgl_glwnd->DrawFunc)(_mgl_glwnd,_mgl_glwnd->FuncPar);
		_mgl_glwnd->Finish();
	}
	if(ch=='P')
	{
		char str[128];
		sprintf(str,"%s_%d.png",_mgl_glwnd->PlotId,_mgl_glwnd->curr_fig);
		_mgl_glwnd->WritePNG(str, "Math GL");
	}
	if(ch=='J')
	{
		char str[128];
		sprintf(str,"%s_%d.jpg",_mgl_glwnd->PlotId,_mgl_glwnd->curr_fig);
		_mgl_glwnd->WriteJPEG(str, "Math GL");
	}
	if(ch=='E')
	{
		char str[128];
		sprintf(str,"%s_%d.eps",_mgl_glwnd->PlotId,_mgl_glwnd->curr_fig);
		_mgl_glwnd->WriteEPS(str, "Math GL");
	}
	if(ch==' ')	_mgl_glwnd->Clf();
	if(ch=='m')	_mgl_glwnd->tt = 1-_mgl_glwnd->tt;
	rL = rL<0 ? 0 : (rL>5 ? 5 : rL);
	_mgl_glwnd->Light(0,mglPoint(rL*cos(pL)*sin(tL), rL*sin(pL)*sin(tL), rL*cos(tL)),false);
	_mgl_glwnd->Alpha(Alpha);
	_mgl_glwnd->Light(Light);
//	glEnable(GL_BLEND);
	if(strchr("ijkl",ch))
		printf("Light: %g, %g, %g\n",rL*cos(pL)*sin(tL), rL*sin(pL)*sin(tL), rL*cos(tL));

	if(ch=='x')	exit(0);
	else		glutPostRedisplay();
}
//-----------------------------------------------------------------------------
void _mgl_display()
{
	if(!_mgl_glwnd)	return;
//	glEnable(GL_LINE_SMOOTH);
	_mgl_glwnd->CurFrameId = 1;
	if(_mgl_glwnd->AutoClf)	_mgl_glwnd->Clf();
	_mgl_glwnd->SubPlot(1,1,0);
	if(_mgl_glwnd->NumFig>0)	glCallList(_mgl_glwnd->curr_fig);
	else
	{
		(_mgl_glwnd->DrawFunc)(_mgl_glwnd,_mgl_glwnd->FuncPar);
		_mgl_glwnd->Finish();
	}
	if(_mgl_glwnd->AutoClf)	glFinish();
	if(_mgl_glwnd->AutoClf)	glutSwapBuffers();
}
//-----------------------------------------------------------------------------
mglGraphGLUT::~mglGraphGLUT()
{
	_mgl_glwnd = 0;
}
//-----------------------------------------------------------------------------
void mglGraphGLUT::Window(int argc, char **argv,int (*draw)(mglGraph *gr, void *p),const char *title, void *par, void (*reload)(int next, void *p), bool maximize)
{
	NumFig=0;	curr_fig=1;	tt=0;
	_mgl_glwnd = this;
	CurFrameId = 1;

	char *tmp[1];	tmp[0]=new char[1];	tmp[0][0]=0;
	glutInit(&argc, argv ? argv:tmp);
	delete []tmp[0];
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("MathPlotLibrary");

	Light(0,mglPoint(0,0,3),false);
	NumFig = draw(this,par)-1;	Finish();
	DrawFunc = draw;	FuncPar = par;
	LoadFunc = reload;
	glutSetWindowTitle(title);

	glutDisplayFunc(_mgl_display);
	glutKeyboardUpFunc(_mgl_key_up);
	glutTimerFunc(int(1000*Delay),_mgl_timer,0);

	// TODO Add window maximazing at start up ???

	glutMainLoop();
	glDeleteLists(1,NumFig);
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_glut(HMDR dr, const char *title)
{
	mglGraphGLUT *g = new mglGraphGLUT;
	g->Window(0,0,title,dr);
	return g;
}
//-----------------------------------------------------------------------------
HMGL mgl_create_graph_glut(int (*draw)(HMGL gr, void *p), const char *title, void *par)
{
	mglGraphGLUT *g = new mglGraphGLUT;
	g->Window(0,0,draw,title,par);
	return g;
}
//-----------------------------------------------------------------------------
mglGraphGLUT::mglGraphGLUT() : mglGraphGL()	{}
//-----------------------------------------------------------------------------
/*uintptr_t mgl_create_graph_glut_(int (*draw)(uintptr_t *gr), const char *title, int l)
{
	mglGraphGLUT *g = new mglGraphGLUT;
	char *s = new char[l+1];	memcpy(s,title,l);	s[l]=0;
	g->Window(0,0,mgl_fortran_func,s,(void*)draw);
	delete []s;
	return uintptr_t(g);
}*/
//-----------------------------------------------------------------------------
