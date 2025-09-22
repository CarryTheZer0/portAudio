
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <UI.h>
#include <UIRenderer.h>

#include "portaudio.h"
#include "audioStream.h"
#include "sine_wave.h"
#include "adsrEnvelope.h"

int main() {
	PaError err = Pa_Initialize();
	if( err != paNoError )
	{
	    std::cout << "PortAudio error: %s\n" << Pa_GetErrorText( err ) << std::endl;
		return -1;
	}

    int numDevices;
    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        std::cout << "ERROR: Pa_CountDevices returned 0x%x\n" << numDevices << std::endl;
    }

    const PaDeviceInfo* inDeviceInfo;
    const PaDeviceInfo* outDeviceInfo;
    PaDeviceIndex inputDevice, outputDevice;
    std::cout << "selecting input device" << std::endl;
    for( PaDeviceIndex i = 0; i < numDevices; i++ )
    {
        inDeviceInfo = Pa_GetDeviceInfo( i );
        std::cout << inDeviceInfo->name << std::endl;
        std::cout << inDeviceInfo->maxInputChannels << std::endl;
        std::cout << inDeviceInfo->maxOutputChannels << std::endl;
        std::cout << inDeviceInfo->hostApi << std::endl;
        const PaHostApiInfo* info = Pa_GetHostApiInfo(inDeviceInfo->hostApi);
        std::cout << info->name << std::endl;

        char ch;
        std::cin >> ch;
        if (ch == 'Y') {
            std::cout << "selected " << inDeviceInfo->name << std::endl;
            inputDevice = i;
            break;
        }

        std::cout << std::endl;
    }

    std::cout << "selecting output device" << std::endl;
    for( PaDeviceIndex i = 0; i < numDevices; i++ )
    {
        outDeviceInfo = Pa_GetDeviceInfo( i );
        std::cout << outDeviceInfo->name << std::endl;
        std::cout << outDeviceInfo->maxInputChannels << std::endl;
        std::cout << outDeviceInfo->maxOutputChannels << std::endl;
        std::cout << outDeviceInfo->hostApi << std::endl;
        const PaHostApiInfo* info = Pa_GetHostApiInfo(outDeviceInfo->hostApi);
        std::cout << info->name << std::endl;

        char ch;
        std::cin >> ch;
        if (ch == 'Y') {
            std::cout << "selected " << outDeviceInfo->name << std::endl;
            outputDevice = i;
            break;
        }

        std::cout << std::endl;
    }

    std::cout << "running" << std::endl;

    SineWave mySine2 = SineWave(440.0f, 0.2f);

	AudioStream myStream = AudioStream(&mySine2);

    myStream.openStream(2, inputDevice, inDeviceInfo->defaultLowInputLatency, 2, outputDevice, outDeviceInfo->defaultLowOutputLatency);
	// myStream.openStream();
    myStream.startPlayback();

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

    DebugRenderer renderer = DebugRenderer(s, 1024, 768);
    DebugPainter painter = DebugPainter(&renderer, "myPainter");
    Face root = Face(1024.0f, 768.0f);

    std::shared_ptr<Panel> panel = std::make_shared<Panel>(glm::vec3(1.0f, 1.0f, 0.0f));
    root.addChild(panel);
    panel->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 100.0f, 100.0f));

    bool running = true;

    while (running)
    {
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