#include "Config.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "shared.h"


#define MAX_LINE_SZ 4096
#define MAX_KEY_SZ 128
#define MAX_VAL_SZ 128


Config Config::instance;


static const void formatError(){
	fatalError("Error: unexpected format in config file.\n");
};


Config::Config(){
	ifstream cfgfile("config.cfg");
	char* const data = new char[MAX_LINE_SZ];
	char key[MAX_KEY_SZ];
	char val[MAX_VAL_SZ];

	//if file DNE that is OK as defaults exist and get saved
	if (!cfgfile.is_open()){
		setdefaults();
		return;
	}

	//this is some other weird problem
	if (!cfgfile.good()) fatalError("Error: could not open config.cfg\n");

	cout << "Config:: Reading config file...\n";
	while (!cfgfile.eof() && cfgfile.good()){
		cfgfile.getline(data, 4096);

		if (cfgfile.gcount() == 0) continue;
		if (cfgfile.gcount() >= MAX_LINE_SZ) formatError();

		const char* const pKeyEnd = strrchr(data, ' ');
		if (pKeyEnd == NULL) formatError();

		const char* pVal = strpbrk(pKeyEnd, "\"");
		if (pVal != NULL) pVal++;
		else formatError();

		const char* const pValEnd = strrchr(pVal, '"');

		const int keylen = pKeyEnd - data;
		strncpy(key, data, keylen);
		key[keylen] = '\0';

		const int vallen = pValEnd - pVal;
		strncpy(val, pVal, vallen);
		val[vallen] = '\0';

		put(key, val);
		cout << "Config:: " << key << " = \"" << val << "\"\n";
	}
	cout << "Config:: Done reading config.\n";

	delete[] data;
	setdefaults();
};


Config::~Config(){
	map<string, string>::const_iterator it = store.begin();
	ofstream cfgfile("./config.cfg", ios::out|ios::trunc);

	while (it != store.end()){
		const string& key = it->first;
		const string& val = it->second;
		cfgfile << key << " \"" << val << "\"\n" ;
		it++;
	}

	cfgfile.close();
};


const string& Config::mget(const string& key) const {
	const map<string, string>::const_iterator it = store.find(key);
	if (it == store.end()) fatalError("requested unavailable cvar!\n");
	return it->second;
};


const void Config::mput(const string& key, const string& value){
	store.insert(pair<string, string>(key, value));
};


const void Config::setdefaults(){
	mputdefault("r_width", "640");
	mputdefault("r_height", "480");
	mputdefault("r_fullscreen", "false");
	mputdefault("sdl_fps", "false");
	mputdefault("objs", "50");
	mputdefault("gl_edges", "64");
};


const void Config::mputdefault(const string& key, const string& value){
	const map<string, string>::const_iterator it = store.find(key);
	if (it == store.end()) store.insert(pair<string, string>(key, value));
};
