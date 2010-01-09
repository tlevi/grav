#ifdef API_GLUT
#ifndef GLRENDERER_H_
#define GLRENDERER_H_

#include "Renderer.h"
#include "../input/GLUTInput.h"


class GLRenderer: Renderer{
public:
	GLRenderer(GLUTInput& glutinput);
	~GLRenderer();
};


#endif /* GLRENDERER_H_ */
#endif /* API_GLUT */
