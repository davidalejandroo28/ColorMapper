#pragma once

#include "System_Manager.h"
#include "Runtime_Script.h"

RuntimeScript::RuntimeScript()
{
	SystemManager* sys = SystemManager::GetInstance();
	sys->AddGameScript(this);
}

void RuntimeScript::EventTrigger()
{
	if (!_didInitialize)
	{
		this->Start();

		_didInitialize = true;
	}
	else
	{
		this->Update();
	}

}
