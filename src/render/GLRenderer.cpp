#ifdef API_GLUT
#include "../shared.h"
#include "GLRenderer.h"
#include <GL/freeglut.h>
#include <iostream>


GLRenderer* GLRenderer::instance = NULL;


void GLRenderer::redisplayFunc(){
	if (instance) instance->redisplay();
	else fatalError("GLRenderer: redisplay called but no GLRenderer instance!\n");
};


static void nothingFunc(){
	return;
};


GLRenderer::GLRenderer(GLUTInput& glutinput) : glutinput(glutinput){
	windowid = 0;
	width = Renderer::config_width();
	height = Renderer::config_height();
	fullscreen = Renderer::config_fullscreen();

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	GLRenderer::instance = this;

	glutDisplayFunc(GLRenderer::redisplayFunc);
};


GLRenderer::~GLRenderer(){
	if (windowid > 0) glutDestroyWindow(windowid);
	glutDisplayFunc(nothingFunc);
};


const bool GLRenderer::requestScreen(){
	if (windowid > 0) glutDestroyWindow(windowid);

	windowid = glutCreateWindow(GRAV_WINDOW_TITLE);
	if (windowid <= 0) return false;

	glutReshapeWindow(width, height);
	glutSetCursor(GLUT_CURSOR_NONE);
	if (fullscreen) glutFullScreen();

	glutinput.Reinitialise();

	return true;
};


const void GLRenderer::startFrame(){
	glClear(GL_COLOR_BUFFER_BIT);
	return;
};


const void GLRenderer::endFrame(){
	glutSwapBuffers();
};


const void GLRenderer::redisplay(){
	startFrame();
	endFrame();
};


const void GLRenderer::doDrawing(const vector<physobj>& objs){
	startFrame();
	endFrame();
};


#endif /* API_GLUT */
