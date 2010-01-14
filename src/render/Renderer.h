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

	virtual const void doDrawing(const vector<physobj>& objs) = 0;
	virtual const void updateTrails(const vector<physobj>& objs) = 0;
	virtual const bool requestScreen() = 0;

	static Renderer* const getRenderer(Input& input);
};


#endif /* RENDERER_H_ */
