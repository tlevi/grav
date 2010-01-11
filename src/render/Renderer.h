#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>
#include "../phys/physobj.h"
#include "../input/Input.h"


class Renderer{
private:
	static Renderer* instance;


protected:
	static const short config_width();
	static const short config_height();
	static const bool config_fullscreen();

	Renderer();

	short width;
	short height;
	bool fullscreen;


public:
	virtual ~Renderer(){};

	virtual const void startFrame() = 0;
	virtual const void drawObjects(const vector<physobj>& objs) = 0;
	virtual const void updateTrails(const vector<physobj>& objs) = 0;
	virtual const void fadeTrails() = 0;
	virtual const void clearTrails() = 0;
	virtual const void endFrame() = 0;

	virtual const bool requestScreen() = 0;


	#ifdef API_SDL
	static Renderer* const getRenderer(Input& input);
	#endif
	#ifdef API_GLUT
	static Renderer* const getRenderer(Input& input);
	#endif
};


#endif /* RENDERER_H_ */
