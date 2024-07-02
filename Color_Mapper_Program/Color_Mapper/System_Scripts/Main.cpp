////CONFINGURATIONS////
//To configure, go to project->properties->preprocessor->preprocessor definitions->set 1 or 0's accordingly

#if TESTMODE 1
    
#else
//don't run this main if tests are enabled

    #if DEBUGMODE 1
        #include <vld.h>
    #endif

#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../Libraries/Image_Loader/stb_image.h"

#include "Shader.h"
#include "Event.h"
#include "System_Manager.h"

#include "../Playground.h"
#include "../Assets/Game_Scripts/TestGameScript.h"

using namespace std;

static float deltaTime;

void CalculateDeltaTime()
{

}

void DeleteScripts(vector<Observer<>*> scripts)
{
    for (Observer<>* observer: scripts)
    {
        delete observer;
    }
}

int main()
{
    sf::Context context;

    //Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)(context.getFunction)))
    {
        cout << "Could not initialize GLAD!!!!" << endl;
        return -1;
    }
    //

    //Initialize the system
    SystemManager* sysManager = SystemManager::GetInstance();
    sysManager->PrintSystemInfomation();

    vector<Observer<>*> scripts = { new TestGameScript, new Playground };

    // create the window
    sf::ContextSettings windowSettings(24);
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, windowSettings);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // Call scripts
        sysManager->UpdateSystemScripts();
        sysManager->UpdateGameScripts();
        
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
    DeleteScripts(scripts);
    sysManager->DeleteInstance();

    //TODO destroy scripts and get valgrind to tell me about memory leaks

    return 0;
}
#endif