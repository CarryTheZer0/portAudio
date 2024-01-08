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

	// SineWave mySine;
    // SineWave mySine2 = SineWave(330.0f, 0.2f);
    // SineWave mySine3 = SineWave(440.0f, 0.2f);
    // SineWave mySine4 = SineWave(550.0f, 0.2f);
    // SineWave mySine5 = SineWave(660.0f, 0.2f);
	
	// KickTest myKick;

    // Mixer<2> myMixer;
    // Mixer<2> myMixer2;
    Mixer<2> myMixer3;
    // myMixer.setInput(&mySine2, 0);
    // myMixer.setInput(&mySine3, 1);

    // myMixer2.setInput(&mySine4, 0);
    // myMixer2.setInput(&mySine5, 1);
    
	// myKick.setDecayTime(0.1f);

    Note myNote(440.0f);
    Note myNote2(550.0f);
    Note myNote2(660.0f);

    AdsrEnvelope myEnvelope(myNote.get_pOutput());

	myEnvelope.setAttackTime(0.1f);
	myEnvelope.setDecayTime(2.0f);
	myEnvelope.setReleaseTime(0.05f);

    AdsrEnvelope myEnvelope2(myNote2.get_pOutput());

	myEnvelope2.setAttackTime(0.1f);
	myEnvelope2.setDecayTime(2.0f);
	myEnvelope2.setReleaseTime(0.05f);

    myMixer3.setInput(&myEnvelope, 0);
    myMixer3.setInput(&myEnvelope2, 1);

	AudioStream myStream = AudioStream(&myMixer3);

	myStream.openStream();
	myStream.startPlayback();

	// mySine.setFrequency(220.0f);

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
    button1.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(10.0f, 10.0f, 20.0f, 20.0f));
    Button button2 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button2);
    button2.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(40.0f, 10.0f, 20.0f, 20.0f));
    Button button3 = Button(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(&button3);
    button3.setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(60.0f, 60.0f, 20.0f, 20.0f));

    // auto hitDrum = std::bind(&KickTest::hit, &myKick);
    auto hitNote = std::bind(&AdsrEnvelope::noteDown, &myEnvelope);
    auto releaseNote = std::bind(&AdsrEnvelope::noteUp, &myEnvelope);
    auto hitNote2 = std::bind(&AdsrEnvelope::noteDown, &myEnvelope2);
    auto releaseNote2 = std::bind(&AdsrEnvelope::noteUp, &myEnvelope2);
    button1.setCallback(hitNote);
    button1.setCallbackUp(releaseNote);
    button2.setCallback(hitNote2);
    button2.setCallbackUp(releaseNote2);
    // button3.setCallback(hitDrum);

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
