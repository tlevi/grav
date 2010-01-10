#include "Config.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>


#define MAX_LINE_SZ 4096
#define MAX_KEY_SZ 128
#define MAX_VAL_SZ 128


Config Config::instance;


static const void error(const string& msg){
	cerr << msg << endl;
	exit(1);
};


static const void formatError(){
	error("Error: unexpected format in config file.");
};


Config::Config(){
	ifstream cfgfile("config.cfg");
	char* const data = new char[MAX_LINE_SZ];
	char key[MAX_KEY_SZ];
	char val[MAX_VAL_SZ];

	if (!cfgfile.is_open() || !cfgfile.good())
		error("Error: could not open config.cfg");

	cout << "Reading config file...";
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
//		cout << key << " : " << val << endl;
	}
	cout << "Done.\n";

	delete[] data;
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


string* const Config::mget(const string& key) const {
	const map<string, string>::const_iterator it = store.find(key);
	return (it == store.end()) ? NULL : const_cast<string*>(&it->second);
};


const void Config::mput(const string& key, const string& value){
	store.insert(pair<string, string>(key, value));
};
