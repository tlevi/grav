#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>

using namespace std;


class Config{
private:
	static Config instance;

	Config();
	Config(const Config&);
	~Config();
	map<string, string> store;


public:
	static Config& getInstance(){
		return instance;
	}

	/**
	 * Get a string pointer to the value associated with key.
	 * If there is no associated value, returns NULL.
	 * @param key find value associated with this key
	 * @return pointer to value, or NULL if key/value does not exist
	 */
	string* get(const string& key) const;


	/**
	 * Put a key/value pair, replacing any existing value.
	 * @param key key to store
	 * @param value value to store
	 */
	const void put(const string& key, const string& value);
};


#endif /* CONFIG_H_ */
