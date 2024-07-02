#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

//TODO, let's move the events to the system manager because I can't make vectors static (no dynamic allocation) and let's use maps
//Basically getInput(inputType, keyCode)
static class EventManager
{
public:
	void Refresh_Events(sf::Window& window);

	sf::Event& HasEvent(sf::Event::EventType eventType);

private:
	std::vector<sf::Event> events;
};