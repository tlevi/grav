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

	string* mget(const string& key) const;
	const void mput(const string& key, const string& value);

public:
	/**
	 * Get a string pointer to the value associated with key.
	 * If there is no associated value, returns NULL.
	 * @param key find value associated with this key
	 * @return pointer to value, or NULL if key/value does not exist
	 */
	static inline string* get(const string& key){
		return instance.mget(key);
	};


	/**
	 * Put a key/value pair, replacing any existing value.
	 * @param key key to store
	 * @param value value to store
	 */
	static inline const void put(const string& key, const string& value){
		return instance.mput(key, value);
	};
};


#endif /* CONFIG_H_ */
