#ifdef API_GLUT
#include "../shared.h"
#include "GLUTInput.h"


GLUTInput::GLUTInput(){
	// TODO Auto-generated constructor stub
};


GLUTInput::~GLUTInput(){
	// TODO Auto-generated destructor stub
};


const void GLUTInput::Reinitialise(){
	//TODO
};


const void GLUTInput::PumpEvents(){
	//TODO
};


const bool GLUTInput::hasNext() const{
	//TODO
	return false;
};


const KeyEvent* const GLUTInput::nextEvent(){
	static KeyEvent ev('\0', 0);
	//TODO
	return &ev;
};


#endif /* API_GLUT */
