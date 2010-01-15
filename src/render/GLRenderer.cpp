#ifdef API_GLUT
#include "../shared.h"
#include "GLRenderer.h"
#include <GL/freeglut.h>
#include <iostream>
#include "../phys/Physics.h"
#include "../Config.h"


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

	const string* const stredges = Config::get("gl_edges");
	edges = (stredges != NULL) ? atoi(stredges->c_str()) : 0;
	if (edges == 0){
		edges = 100;
		Config::put("gl_edges", "100");
	}
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const vector2& min = Physics::getBBmin();
	const vector2& max = Physics::getBBmax();
	glOrtho(min.x, max.x, min.y, max.y, -1.0f, 1.0f);

	GLfloat mat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, &mat[0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 0);

	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POLYGON_SMOOTH);

	cout << "samples: " << glutGet(GLUT_WINDOW_NUM_SAMPLES) << endl;

	return true;
};


const void GLRenderer::drawObjects(const vector<physobj>& objs){
	const int max = objs.size();
	for (int i=0; i < max; i++){
		//	const float scale = width/height;
		const float x = objs[i].get_p().x;
		const float y = objs[i].get_p().y;
		const float r = objs[i].get_radius();
		const unsigned int color = objs[i].get_color();
		const float red = ((color>>16)&0xff)/255.0f;
		const float g = ((color>>8)&0xff)/255.0f;
		const float b = (color&0xff)/255.0f;

		glBegin(GL_POLYGON);
		glColor3f(red, g, b);
		const int parts = edges;
		for (int k=0; k < parts; k++){
			const float angle = DEG2RAD(k)*float(360.0/parts);
			glVertex2f(x + r*cos(angle), y + r*sin(angle));
		}
		glEnd();
	}
};


const void GLRenderer::startFrame(){
	glClear(GL_COLOR_BUFFER_BIT);
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
	drawObjects(objs);
	endFrame();
};


const void GLRenderer::updateTrails(const vector<physobj>& objs){
	//TODO GLRenderer::updateTrails()
};


#endif /* API_GLUT */
