#include "Event_Manager.h"

void EventManager::Refresh_Events(sf::Window& window)
{
	this->events.empty();

	sf::Event event;

	while (window.pollEvent(event))
	{
		this->events.push_back(event);
	}
}

sf::Event& EventManager::HasEvent(sf::Event::EventType eventType)
{
	//TODO you can make this O(nlog(n)) instead of O(n)
	for (int x = 0; x < this->events.size(); x++)
	{
		if (this->events.at(x).type == eventType)
		{
			return this->events.at(x);
		}
	}
}
