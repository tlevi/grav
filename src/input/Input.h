#ifndef INPUT_H_
#define INPUT_H_


#define KEYMOD_NONE		(0<<0)
#define KEYMOD_CTRL		(1<<1)
#define KEYMOD_ALT		(1<<2)
#define KEYMOD_SHIFT	(1<<3)


class KeyEvent{
public:
	inline KeyEvent(const char aKey, const char someMods) : key(aKey), mods(someMods){};
	const char key;
	const int mods;
};


class Input{
protected:
	Input(){};


public:
	static Input* const getInput();


	virtual ~Input(){};

	/* might need to call if you change windows, resolution etc */
	virtual const void Reinitialise() = 0;

	/* clear old events and collect new ones */
	virtual const void PumpEvents() = 0;

	/* test for more events and get the next one */
	virtual const bool hasNext() const = 0;
	virtual const KeyEvent& nextEvent() = 0;
};


#endif /* INPUT_H_ */
