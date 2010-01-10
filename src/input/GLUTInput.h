#ifdef API_GLUT
#ifndef GLUTINPUT_H_
#define GLUTINPUT_H_

#include "Input.h"


class GLUTInput: public Input{
public:
	GLUTInput();
	~GLUTInput();


	const void Reinitialise();

	const void PumpEvents();

	const bool hasNext() const;
	const KeyEvent& nextEvent();
};


#endif /* GLUTINPUT_H_ */
#endif /* API_GLUT */
