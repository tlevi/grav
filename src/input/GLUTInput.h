#ifdef API_GLUT
#ifndef GLUTINPUT_H_
#define GLUTINPUT_H_

#include "Input.h"
#include <list>

using namespace std;


class GLUTInput: public Input{
private:
	static GLUTInput* instance;

	static void keyboardFunc(unsigned char key, int x, int y);


	list<KeyEvent> kevlist;

	const void receiveKey(unsigned char key);


public:
	GLUTInput();
	~GLUTInput();


	const void Reinitialise();

	const void PumpEvents();

	const bool hasNext() const;
	const bool nextEvent(KeyEvent& kev);
};


#endif /* GLUTINPUT_H_ */
#endif /* API_GLUT */
