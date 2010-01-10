#ifdef API_GLUT
#include "../shared.h"
#include "GLRenderer.h"
#include <GL/freeglut.h>
#include <iostream>


GLRenderer* GLRenderer::instance = NULL;


void GLRenderer::redisplayFunc(){
	if (instance) instance->redisplay();
	else{
		cerr << "redisplay called but no GLRenderer instance!\n";
		exit(1);
	}
};


void nothingFunc(){
	return;
};


GLRenderer::GLRenderer(GLUTInput& glutinput){
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


const void GLRenderer::drawObjects(const vector<physobj>& objs){
};


const void GLRenderer::updateTrails(const vector<physobj>& objs){
};


const void GLRenderer::fadeTrails(){
};


const void GLRenderer::clearTrails(){
};


const bool GLRenderer::requestScreen(){
	if (windowid > 0) glutDestroyWindow(windowid);

	windowid = glutCreateWindow(GRAV_WINDOW_TITLE);
	if (windowid <= 0) return false;

	glutReshapeWindow(width, height);
	glutSetCursor(GLUT_CURSOR_NONE);
	if (fullscreen) glutFullScreen();

	return true;
};


const void GLRenderer::startFrame(){
	glClear(GL_COLOR_BUFFER_BIT);
	cout << "frame start\n";
	return;
};


const void GLRenderer::endFrame(){
	glutSwapBuffers();
	cout << "frame end\n";
};


const void GLRenderer::redisplay(){
	startFrame();
	endFrame();
};


#endif /* API_GLUT */
