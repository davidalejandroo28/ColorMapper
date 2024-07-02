#pragma once

#include <iostream>
#include <iomanip>

#include <glad/glad.h>

#include "Event.h"
#include "System_Manager.h"

//Defining statics (Prevents duplication)
SystemManager* SystemManager::_instance = nullptr;
Event<> SystemManager::systemScripts;
Event<> SystemManager::gameScripts;

void SystemManager::PrintSystemInfomation()
{
	std::cout << "---- System Infomation ----" << std::endl;
	std::cout << "Version of GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << std::setw(28) << std::setfill('-') << "" << std::endl;
}