#pragma once

#include <vector>
#include <iostream>

template<class... types>
class Observer
{
public:
	virtual void EventTrigger(types...) = 0;
private:

};

template<class... types>
class Event
{
public:
	// This calls the EventTrigger function in the Observer class. 
	// Via polymorphism, any class can overide EventTrigger and have it's function get called
	void Trigger(types... params)
	{
		for (int x = 0; x < this->observers.size(); x++)
		{
			if (this->observers.at(x) == nullptr)
			{
				//TODO, create a print statement if system debug mode is on
				std::cout << "TESTING; Event.cpp line 12" << std::endl;
				this->observers.erase(this->observers.begin() + x, this->observers.end() + x + 1);
				x--;
			}
			else
			{
				this->observers.at(x)->EventTrigger(params...);
			}
		}
	}

	void AddObserver(Observer<types...>* observer)
	{
		this->observers.push_back(observer);
	}
	
	//TODO
	void RemoveObserver(Observer<>* observer);
private:
	std::vector<Observer<types...>*> observers;
};

template<class... parameters>
class FunctionEvent
{
public:
	void Trigger(parameters... params)
	{
		for (int x = 0; x < this->functions.size(); x++)
		{
			void (*ptr)(parameters...) = functions.at(x);

			if (ptr != nullptr)
			{
				ptr(params...);
			}
			else
			{
				//TODO, remove pointers
				std::cout << "WE GOTTA NULL PTR HERE!" << std::endl;
			} 
		}
	}

	//Uses function pointers
	void AddFunction(void (*ptr)(parameters...))
	{
		this->functions.push_back(ptr);
	}

	void operator+=(void (*ptr)(parameters...))
	{
		this->AddFunction(ptr);
	}

	
	void RemoveFunction(void (*ptr)(parameters...))
	{
		for (auto x = functions.begin(); x != functions.end();)
		{
			if (ptr == *x)
			{
				x = functions.erase(x);
			}
			else
			{
				x++;
			}
		}
	}

	void operator-=(void (*ptr)(parameters...))
	{
		this->RemoveFunction(ptr);
	}

private:
	std::vector<void (*)(parameters...)> functions;
};