//
// drum_synth.cpp main file for testing DSP
//

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "DebugRenderer.h"
#include "InputHandler.h"
#include "Shader.h"
#include "DebugPainter.h"

#include "Panel.h"
#include "Slider.h"
#include "Button.h"

#include "portaudio.h"

#include "AudioStream.h"
#include "SineWave.h"
#include "SquareWave.h"
#include "AdsrEnvelope.h"
#include "KickTest.h"
#include "Mixer.h"
#include "Chain.h"
#include "AllPassFilter.h"
#include "CombFilter.h"
#include "Reverb.h"

int main() {
	PaError err = Pa_Initialize();
	if( err != paNoError )
	{
	    printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}

	glewExperimental=true;
    if (!glfwInit() )
    {
        fprintf( stderr, "Failed to initialise GLFW\n" );
        return -1;
    }

    GLFWwindow* pWindow;
	pWindow = glfwCreateWindow( 1024, 768, "Test 01", NULL, NULL);
    glfwMakeContextCurrent(pWindow);

    glewExperimental=true;
    if (glewInit() != GLEW_OK) {
        fprintf( stderr, "Failed to initialize GLEW\n" );
        return -1;
    }

	Shader s = Shader();
    s.loadShaderFromFile("C:/Users/mchlp/Documents/vsCode/UI/src/graphics/shaders/LineVertexShader.glsl", "C:/Users/mchlp/Documents/vsCode/UI/src/graphics/shaders/LineFragmentShader.glsl");

    DebugRenderer renderer = DebugRenderer(s, s, 1024, 768);

    SquareWave mySqaure = SquareWave(440.0f, 0.2f);
    SquareWave mySqaure1 = SquareWave(660.0f, 0.2f);

    Chain myChain1;
    Mixer myMixer;
    Reverb myReverb(2);

    AllPassFilter myAllPass = AllPassFilter(441, 0.1f);

    myChain1.addInput(&mySqaure);
    myChain1.addInput(&myReverb);

    myMixer.addInput(&myChain1);
	AudioStream myStream = AudioStream(&myMixer);

	myStream.openStream();
	myStream.startPlayback();

    DebugPainter painter = DebugPainter(&renderer, "myPainter");
    Face root = Face(1024.0f, 768.0f);

    std::shared_ptr<Panel> panel = std::make_shared<Panel>(glm::vec3(1.0f, 1.0f, 0.0f));
    root.addChild(panel);
    panel->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 100.0f, 100.0f));

	std::shared_ptr<Button> button1 = std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel->addChild(button1);
    button1->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(10.0f, 10.0f, 20.0f, 20.0f));
    
    std::shared_ptr<Button> button2 = std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel->addChild(button2);
    button2->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(40.0f, 10.0f, 20.0f, 20.0f));
    
    std::shared_ptr<Button> button3 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel->addChild(button3);
    button3->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(60.0f, 60.0f, 20.0f, 20.0f));

    std::shared_ptr<Slider> slider1 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f);
    panel->addChild(slider1);
    slider1->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 31.0f, 40.0f, 0.0f));

    auto hitNote = std::bind(&Oscillator::setFrequency, &mySqaure, 100.0f);
    auto releaseNote = std::bind(&Oscillator::setFrequency, &mySqaure, 660.0f);
    auto hitNote2 = std::bind(&AudioObject::setBypass, &myReverb, true);
    auto releaseNote2 = std::bind(&AudioObject::setBypass, &myReverb, false);
    button1->setCallbackDown(hitNote);
    button1->setCallbackUp(releaseNote);
    button2->setCallbackDown(hitNote2);
    button2->setCallbackUp(releaseNote2);
    // button3->setCallbackDown(hitDrum);

	temp_UI::InputHandler input = temp_UI::InputHandler(&root);

    glfwSetWindowUserPointer( pWindow, &input );

    glfwSetCursorPosCallback( pWindow, []( GLFWwindow* window, double x, double y)
    {
        temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );

        // x = -1.0f + 2 * x / 1024.0f;
        // y = -1.0f + 2 * (768.0f - y) / 768.0f;
        y = (768.0f - y);

        glm::mat4 view = glm::lookAt(
            glm::vec3(1,1,1),  // location
            glm::vec3(0,0,0),  // target
            glm::vec3(0,1,0)   // orientation (up direction)
        );

        // ortho camera :
        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f); // In world coordinates

        glm::vec4 pos = glm::inverse(projection) * glm::inverse(view) * glm::vec4(x, y, 0.0f, 1.0f);

        input->mouseMoveCallback(x, y);   
    });

    glfwSetMouseButtonCallback( pWindow, []( GLFWwindow* window, int button, int action, int mods)
    {
        temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );
        input->mouseCallback(button, action, mods);
    });

    glfwSetScrollCallback( pWindow, [](GLFWwindow* window, double x, double y)
    {
        temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );
        input->scrollCallback(x, y);
    });

	bool running = true;

    while(running) {
        glfwPollEvents();

		// render
		glClearColor(0.18f, 0.38f, 0.37f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        root.draw(&painter);
        renderer.draw();

        if (glfwWindowShouldClose(pWindow)) {
            running = false;
        }
        glfwSwapBuffers(pWindow);
    }

	glfwTerminate();

	myStream.stopPlayback();
	myStream.closeStream();

	err = Pa_Terminate();
	if( err != paNoError )
	   printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	return 0;
}
