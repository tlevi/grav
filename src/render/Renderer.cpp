#include "Renderer.h"
#include "../Config.h"
#include <string>
#include <cstdlib>


Renderer* Renderer::instance = NULL;


Renderer::Renderer(){
	width = Renderer::config_width();
	height = Renderer::config_height();
	fullscreen = Renderer::config_fullscreen();
};


#ifdef API_SDL
#include "SDLRenderer.h"
Renderer* const Renderer::getRenderer(Input& input){
	if (instance == NULL) instance = new SDLRenderer(static_cast<SDLInput&>(input));
	return instance;
};
#endif


#ifdef API_GLUT
#include "GLRenderer.h"
Renderer* const Renderer::getRenderer(Input& input){
	if (instance == NULL) instance = new GLRenderer(static_cast<GLUTInput&>(input));
	return instance;
};
#endif


const bool Renderer::config_fullscreen(){
	const string* const val = Config::get("r_fullscreen");
	if (val == NULL){
		Config::put("r_fullscreen", "false");
		return false;
	}
	else if (*val == "true") return true;
	else if (*val == "1"){
		Config::put("r_fullscreen", "true");
		return true;
	}
	else{
		Config::put("r_fullscreen", "false");
		return false;
	}
};


const short Renderer::config_height(){
	const string* const str = Config::get("r_height");
	const short num = (str != NULL) ? atoi(str->c_str()) : -1;

	if (num > 0) return num;
	else{
		Config::put("r_height", "480");
		return 480;
	}
};


const short Renderer::config_width(){
	const string* const str = Config::get("r_width");
	const short num = (str != NULL) ? atoi(str->c_str()) : -1;

	if (num > 0) return num;
	else{
		Config::put("r_width", "640");
		return 640;
	}
};

