#include "Renderer.h"
#include "../Config.h"
#include <cstdlib>
#include <iostream>


Renderer* Renderer::instance = NULL;


Renderer::Renderer(){
	width = Renderer::config_width();
	height = Renderer::config_height();
	fullscreen = Renderer::config_fullscreen();
};


#ifdef API_SDL
#include "SDLRenderer.h"
Renderer* const Renderer::getRenderer(Input& input){
	cout << "Renderr: Using SDL/Software rendering\n";
	if (instance == NULL) instance = new SDLRenderer(static_cast<SDLInput&>(input));
	return instance;
};
#endif


#ifdef API_GLUT
#include "GLRenderer.h"
Renderer* const Renderer::getRenderer(Input& input){
	cout << "Renderr: Using GLUT/OpenGL rendering\n";
	if (instance == NULL) instance = new GLRenderer(static_cast<GLUTInput&>(input));
	return instance;
};
#endif


const bool Renderer::config_fullscreen(){
	return Config::get("r_fullscreen") == "true";
};


const short Renderer::config_height(){
	return atoi(Config::get("r_height").c_str());
};


const short Renderer::config_width(){
	return atoi(Config::get("r_width").c_str());
};

