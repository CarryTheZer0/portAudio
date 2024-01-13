//
// drum_synth.cpp main file for testing DSP
//

#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "UIRenderer.h"
#include "InputHandler.h"
#include "Shader.h"
#include "Painter.h"

#include "Panel.h"
#include "Slider.h"
#include "Button.h"

#include "portaudio.h"

#include "audioStream.h"
#include "sine_wave.h"
#include "adsrEnvelope.h"
#include "kickTest.h"
#include "mixer.h"
#include "note.h"

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
    s.loadShaderFromFile("C:/Users/mchlp/Documents/Mikes/C++Libs/UI/src/graphics/shaders/SimpleVertexShader.glsl", "C:/Users/mchlp/Documents/Mikes/C++Libs/UI/src/graphics/shaders/SimpleFragmentShader.glsl");

    UIRenderer renderer = UIRenderer(s);

    for (int i = 0; i < 1; i++) {
        float fundamental_frequency = 100.0f;
    }

    Mixer<5> myMixer;

    Note myNote(130.81f);
    Note myNote2(146.83f);
    Note myNote3(164.81f);
    Note myNote4(174.61f);
    Note myNote5(196.00f);

    myMixer.setInput(myNote.get_pOutput(), 0);
    myMixer.setInput(myNote2.get_pOutput(), 1);
    myMixer.setInput(myNote3.get_pOutput(), 2);
    myMixer.setInput(myNote4.get_pOutput(), 3);
    myMixer.setInput(myNote5.get_pOutput(), 4);

	AudioStream myStream = AudioStream(&myMixer);

	myStream.openStream();
	myStream.startPlayback();

	glm::mat4 view = glm::lookAt(
        glm::vec3(0,0,1),  // location
        glm::vec3(0,0,0),  // target
        glm::vec3(0,1,0)   // orientation (up direction)
    );
    renderer.setView(view);

    // ortho camera :
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f); // In world coordinates
    // glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

    renderer.setProjection(projection); 

    Painter painter = Painter(&renderer, "myPainter");
    Face root = Face(1024.0f, 768.0f);

    Panel panel = Panel(glm::vec3(1.0f, 1.0f, 0.0f));
    root.addChild(&panel);
    panel.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 100.0f, 100.0f));

	Button button1 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button1);
    button1.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(10.0f, 10.0f, 10.0f, 70.0f));
    
    Button button2 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button2);
    button2.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(22.0f, 10.0f, 10.0f, 70.0f));
    
    Button button3 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button3);
    button3.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(34.0f, 10.0f, 10.0f, 70.0f));
    
    Button button4 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button4);
    button4.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(46.0f, 10.0f, 10.0f, 70.0f));
    
    Button button5 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button5);
    button5.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(58.0f, 10.0f, 10.0f, 70.0f));

    auto hitNote = std::bind(&Note::noteDown, &myNote);
    auto releaseNote = std::bind(&Note::noteUp, &myNote);
    auto hitNote2 = std::bind(&Note::noteDown, &myNote2);
    auto releaseNote2 = std::bind(&Note::noteUp, &myNote2);
    auto hitNote3 = std::bind(&Note::noteDown, &myNote3);
    auto releaseNote3 = std::bind(&Note::noteUp, &myNote3);
    auto hitNote4 = std::bind(&Note::noteDown, &myNote4);
    auto releaseNote4 = std::bind(&Note::noteUp, &myNote4);
    auto hitNote5 = std::bind(&Note::noteDown, &myNote5);
    auto releaseNote5 = std::bind(&Note::noteUp, &myNote5);
    button1.setCallback(hitNote);
    button1.setCallbackUp(releaseNote);
    button2.setCallback(hitNote2);
    button2.setCallbackUp(releaseNote2);
    button3.setCallback(hitNote3);
    button3.setCallbackUp(releaseNote3);
    button4.setCallback(hitNote4);
    button4.setCallbackUp(releaseNote4);
    button5.setCallback(hitNote5);
    button5.setCallbackUp(releaseNote5);

	InputHandler input = InputHandler(&root);

    glfwSetWindowUserPointer( pWindow, &input );

    glfwSetCursorPosCallback( pWindow, []( GLFWwindow* window, double x, double y)
    {
        InputHandler* input = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );

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
        InputHandler* input = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );
        input->mouseCallback(button, action, mods);
    });

    glfwSetScrollCallback( pWindow, [](GLFWwindow* window, double x, double y)
    {
        InputHandler* input = static_cast<InputHandler*>( glfwGetWindowUserPointer( window ) );
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
