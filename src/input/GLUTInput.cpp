#ifdef API_GLUT
#include "../shared.h"
#include "GLUTInput.h"
#include <GL/freeglut.h>


GLUTInput* GLUTInput::instance = NULL;


void GLUTInput::keyboardFunc(unsigned char key, int x, int y){
	if (instance != NULL) instance->receiveKey(key);
	else fatalError("keyboardFunc called but no GLUTInput instance!\n");
};


GLUTInput::GLUTInput(){
	instance = this;
	glutKeyboardFunc(GLUTInput::keyboardFunc);
};


GLUTInput::~GLUTInput(){
	glutKeyboardFunc(NULL);
	instance = NULL;
};


const void GLUTInput::Reinitialise(){
	glutKeyboardFunc(GLUTInput::keyboardFunc);
};


const void GLUTInput::PumpEvents(){
};


const bool GLUTInput::hasNext() const{
	return !kevlist.empty();
};


const KeyEvent* const GLUTInput::nextEvent(){
	static KeyEvent ev('\0', 0);

	ev = kevlist.front();
	kevlist.pop_front();

	return &ev;
};


const void GLUTInput::receiveKey(unsigned char key){
	const int glutmods = glutGetModifiers();

	int mods = 0;
	if (glutmods == 0) mods = KEYMOD_NONE;
	else{
		if (glutmods & GLUT_ACTIVE_CTRL) mods |= KEYMOD_CTRL;
		if (glutmods & GLUT_ACTIVE_ALT) mods |= KEYMOD_ALT;
		if (glutmods & GLUT_ACTIVE_SHIFT) mods |= KEYMOD_SHIFT;
	}

	kevlist.push_back(KeyEvent(key, mods));
};


#endif /* API_GLUT */
