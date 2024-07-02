#pragma once

#include<iostream>

#include "Event.h"

//void test123()
//{
//	std::cout << "test" << std::endl;
//}

//template<class... types> void Event<types...>::Trigger()
//{
//	for (int x = 0; x < this->observers.size(); x++)
//	{
//		if (this->observers.at(x) == nullptr)
//		{
//			//TODO, create a print statement if system debug mode is on
//			std::cout << "TESTING; Event.cpp line 12" << std::endl;
//			this->observers.erase(this->observers.begin() + x, this->observers.end() + x + 1);
//			x--;
//		}
//		else
//		{
//			this->observers.at(x)->EventTrigger();
//		}
//	}
//}

//TODO put methods in .cpp file (I'm getting linker errors
//template<typename... types> typename void Event<types...>::AddObserver(Observer<types...>* observer)
//{
//	this->observers.push_back(observer);
//}


//template<class... types> void Event<types...>::RemoveObserver(Observer<>* observer)
//{
//	//TODO: Think about what depedences this has
//}