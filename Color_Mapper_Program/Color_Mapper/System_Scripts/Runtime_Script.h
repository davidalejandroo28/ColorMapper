#pragma once
#include "Event.h"

class RuntimeScript: public Observer<>
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;

	//DO NOT OVERLOAD
	void EventTrigger();

	//Constructors
	RuntimeScript();
	
private:
	//TODO: I need to create a way to specify if my script is a system script and then add to the coresponding event in the system manager
	bool _didInitialize = false;
};