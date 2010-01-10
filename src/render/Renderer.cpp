#include "Renderer.h"
#include "../Config.h"
#include <string>
#include <cstdlib>


Renderer* Renderer::instance = NULL;


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
	else if (*val == "1") return true;
	else return false;
};


const short Renderer::config_height(){
	const string* const str = Config::get("r_height");
	if (str == NULL){
		Config::put("r_height", "480");
		return 480;
	}
	else{
		const short num = atoi(str->c_str());
		if (num > 0) return num;
		else{
			Config::put("r_height", "480");
			return 480;
		}
	}
};


const short Renderer::config_width(){
	const string* const str = Config::get("r_height");
	if (str == NULL){
		Config::put("r_height", "640");
		return 640;
	}
	else{
		const short num = atoi(str->c_str());
		if (num > 0) return num;
		else{
			Config::put("r_height", "640");
			return 640;
		}
	}
};

