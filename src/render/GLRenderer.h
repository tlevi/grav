#ifdef API_GLUT
#ifndef GLRENDERER_H_
#define GLRENDERER_H_

#include "Renderer.h"
#include "../input/GLUTInput.h"


class GLRenderer: public Renderer{
private:
	static GLRenderer* instance;

	int windowid;

	static void redisplayFunc();
	const void redisplay();

public:
	GLRenderer(GLUTInput& glutinput);
	~GLRenderer();

	const void startFrame();
	const void drawObjects(const vector<physobj>& objs);
	const void updateTrails(const vector<physobj>& objs);
	const void fadeTrails();
	const void clearTrails();
	const void endFrame();

	const bool requestScreen();
};


#endif /* GLRENDERER_H_ */
#endif /* API_GLUT */
