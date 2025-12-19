//
// drum_synth.cpp main file for testing DSP
//

#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <portaudio.h>
#include <fstream>

#include "DebugRenderer.h"
#include "InputHandler.h"
#include "Shader.h"
#include "DebugPainter.h"

#include "Panel.h"
#include "Slider.h"
#include "Button.h"

#include "AudioStream.h"
#include "Routing/Graph.h"
#include "Routing/Chain.h"
#include "Sources/SineWave.h"
#include "Sources/SquareWave.h"
#include "AdsrEnvelope.h"
#include "Sources/WhiteNoise.h"
#include "Sources/Sampler.h"
#include "Filters/AllPassFilter.h"
#include "Filters/CombFilter.h"
#include "Filters/Biquad.h"
#include "Synthesizer/Synthesizer.h"
#include "Synthesizer/SynthTemp.h"
#include "Synthesizer/SampleSequencer.h"

#include "Utility/WavReader.h"

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

    FILE* file = fopen("C:/Users/mchlp/Documents/vsCode/synth/resources/02 Under Lies.wav", "rb");
    RIFFHeader header = readHeader(file);
    ChunkInfo formatInfo = readFormatInfo(file);
    FormatChunk formatChunk = readFormatChunk(file);
    int16_t data[10];
    ChunkInfo dataInfo = readDataInfo(file);
    readDataChunk<int16_t>(data, 10, file);

    std::cout << "audio_format " << formatChunk.audioFormat << std::endl;
    std::cout << "channel_count " << formatChunk.channelCount << std::endl;
    std::cout << "sample_rate " << formatChunk.sampleRate << std::endl;
    std::cout << "byte_rate " << formatChunk.byteRate << std::endl;
    std::cout << "step_size " << formatChunk.stepSize << std::endl;
    std::cout << "bits_per_sample " << formatChunk.bitsPerSample << std::endl;
    std::cout << "test " << header.chunkSize << std::endl;

    fclose(file);

    // Reverb myReverb(2);
    std::shared_ptr<Biquad> myBiquad = Biquad::makeLowPass();
    std::shared_ptr<Biquad> myBiquad2 = Biquad::makeLowPass();
    myBiquad->setOffset(1);

    std::shared_ptr<Sampler> sampler = std::make_shared<Sampler>("C:/Users/mchlp/Documents/vsCode/synth/resources/02 Under Lies.wav");
    // sampler->setBypass(true);

    std::shared_ptr<Graph> g1 = std::make_shared<Graph>(sampler);
    std::shared_ptr<SquareWave> sine1 = std::make_shared<SquareWave>(440.0f, 0.3f);
    std::shared_ptr<SineWave> sine2 = std::make_shared<SineWave>(100.0f, 0.3f);
    // std::shared_ptr<Reverb> reverb = std::make_shared<Reverb>(2);

    std::shared_ptr<Synthesizer> synth = std::make_shared<Synthesizer>(SynthTemp::makeNote);
    // synth->addNote(48, std::make_shared<SampleSequencer>("C:/Users/mchlp/Documents/vsCode/synth/resources/kick.wav"));
    // synth->addNote(50, std::make_shared<SampleSequencer>("C:/Users/mchlp/Documents/vsCode/synth/resources/ride.wav"));
    // synth->addNote(52, std::make_shared<SampleSequencer>("C:/Users/mchlp/Documents/vsCode/synth/resources/snare.wav"));

    int biquad_1 = g1->addNode(myBiquad);
    int biquad_2 = g1->addNode(myBiquad2);

    g1->addChild(0, biquad_1);
    g1->addChild(0, biquad_2);
    std::shared_ptr<AudioObject> dummy = std::make_shared<AudioObject>();
    dummy->setBypass(true);
    int output = g1->addNode(dummy);
    g1->addChild(biquad_1, output);
    g1->addChild(biquad_2, output);

    std::shared_ptr<Chain> myChain = std::make_shared<Chain>();
    myChain->addInput(synth);
    myChain->addInput(myBiquad);
    myChain->addInput(myBiquad2);

	AudioStream myStream = AudioStream(myChain.get());

    myStream.openStream(2, Pa_GetDefaultInputDevice(), 0.03, 2, Pa_GetDefaultOutputDevice(), 0.03);
	myStream.startPlayback();

    DebugPainter painter = DebugPainter(&renderer, "myPainter");
    Face root = Face(1024.0f, 768.0f);

    Panel panel = Panel(
        root,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 
        glm::vec4(0.0f, 0.0f, 100.0f, 100.0f), 
        glm::vec3(1.0f, 1.0f, 0.0f
    ));
    root.addChild(panel);

	Button button1 = Button(panel, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(10.0f, 10.0f, 5.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    panel.addChild(button1);

    auto button1Down = std::bind(&Synthesizer::noteDown, synth.get(), 48, 1.0f);
    auto lambda1 = [&myStream](){
         myStream.closeStream(); 
         myStream.openStream(2, Pa_GetDefaultInputDevice(), 0.03, 2, Pa_GetDefaultOutputDevice(), 0.03);
         myStream.startPlayback();
    };
    button1.setCallbackDown(lambda1);
    
    // std::shared_ptr<Button> button2 = std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button2);
    // button2->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(16.0f, 10.0f, 5.0f, 20.0f));
    
    // std::shared_ptr<Button> button3 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button3);
    // button3->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(22.0f, 10.0f, 5.0f, 20.0f));

    // std::shared_ptr<Button> button4 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button4);
    // button4->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(28.0f, 10.0f, 5.0f, 20.0f));

    // std::shared_ptr<Button> button5 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button5);
    // button5->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(34.0f, 10.0f, 5.0f, 20.0f));

    // std::shared_ptr<Button> button6 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button6);
    // button6->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(40.0f, 10.0f, 5.0f, 20.0f));

    // std::shared_ptr<Button> button7 =  std::make_shared<Button>(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    // panel->addChild(button7);
    // button7->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(46.0f, 10.0f, 5.0f, 20.0f));

    // std::shared_ptr<Slider> slider1 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f, 0.0f, 0.5f);
    // panel->addChild(slider1);
    // slider1->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 31.0f, 40.0f, 0.0f));

    // std::shared_ptr<Slider> slider2 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f);
    // panel->addChild(slider2);
    // slider2->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 33.0f, 40.0f, 0.0f));

    // std::shared_ptr<Slider> slider3 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f);
    // panel->addChild(slider3);
    // slider3->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 35.0f, 40.0f, 0.0f));

    // std::shared_ptr<Slider> slider4 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f, 0.0f, 0.4f);
    // panel->addChild(slider4);
    // slider4->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 40.0f, 40.0f, 0.0f));

    // std::shared_ptr<Slider> slider5 = std::make_shared<Slider>(glm::vec3(0.0f, 1.0f, 1.0f), 30.0f, 0.0f, 0.4f);
    // panel->addChild(slider5);
    // slider5->setRect(glm::vec4(0.0f, 0.0f, 0.0f, 10.0f), glm::vec4(10.0f, 45.0f, 40.0f, 0.0f));

    auto hitNote = std::bind(&Synthesizer::noteDown, synth.get(), 48, 1.0f);
    auto releaseNote = std::bind(&Synthesizer::noteUp, synth.get(), 48);

    auto hitNote2 = std::bind(&Synthesizer::noteDown, synth.get(), 52, 1.0f);
    auto releaseNote2 = std::bind(&Synthesizer::noteUp, synth.get(), 52);

    auto hitNote3 = std::bind(&Synthesizer::noteDown, synth.get(), 55, 1.0f);
    auto releaseNote3 = std::bind(&Synthesizer::noteUp, synth.get(), 55);

    auto hitNote4 = std::bind(&Biquad::setBypass, myBiquad.get(), true);
    auto releaseNote4 = std::bind(&Biquad::setBypass, myBiquad.get(), false);

    // auto moveSlider = std::bind(&Reverb::setFeedback, reverb.get(), std::placeholders::_1);
    auto moveSlider1 = std::bind(&SquareWave::setPower, sine1.get(), std::placeholders::_1);
    // auto moveSlider2 = std::bind(&Reverb::setDryGain, reverb.get(), std::placeholders::_1);
    // auto moveSlider3 = std::bind(&Reverb::setWetGain1, reverb.get(), std::placeholders::_1);
    // auto moveSlider4 = std::bind(&Reverb::setWetGain2, reverb.get(), std::placeholders::_1);
    // button1->setCallbackDown(hitNote);
    // button1->setCallbackUp(releaseNote);
    // button2->setCallbackDown(hitNote2);
    // button2->setCallbackUp(releaseNote2);
    // button3->setCallbackDown(hitNote3);
    // button3->setCallbackUp(releaseNote3);
    // button4->setCallbackDown(hitNote4);
    // button4->setCallbackUp(releaseNote4);
    // slider1->setCallbackUpdate(moveSlider1);
    // slider2->setCallbackUpdate(moveSlider1);
    // slider3->setCallbackUpdate(moveSlider2);
    // slider4->setCallbackUpdate(moveSlider3);
    // slider5->setCallbackUpdate(moveSlider4);
    // button3->setCallbackDown(hitDrum);

	// temp_UI::InputHandler input = temp_UI::InputHandler(&root);

    // glfwSetWindowUserPointer( pWindow, &input );

    // glfwSetCursorPosCallback( pWindow, []( GLFWwindow* window, double x, double y)
    // {
    //     temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );

    //     // x = -1.0f + 2 * x / 1024.0f;
    //     // y = -1.0f + 2 * (768.0f - y) / 768.0f;
    //     y = (768.0f - y);

    //     glm::mat4 view = glm::lookAt(
    //         glm::vec3(1,1,1),  // location
    //         glm::vec3(0,0,0),  // target
    //         glm::vec3(0,1,0)   // orientation (up direction)
    //     );

    //     // ortho camera :
    //     glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f); // In world coordinates

    //     glm::vec4 pos = glm::inverse(projection) * glm::inverse(view) * glm::vec4(x, y, 0.0f, 1.0f);

    //     input->mouseMoveCallback(x, y);   
    // });

    // glfwSetMouseButtonCallback( pWindow, []( GLFWwindow* window, int button, int action, int mods)
    // {
    //     temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );
    //     input->mouseCallback(button, action, mods);
    // });

    // glfwSetScrollCallback( pWindow, [](GLFWwindow* window, double x, double y)
    // {
    //     temp_UI::InputHandler* input = static_cast<temp_UI::InputHandler*>( glfwGetWindowUserPointer( window ) );
    //     input->scrollCallback(x, y);
    // });

    glfwSetWindowUserPointer( pWindow, synth.get() );
    glfwSetKeyCallback( pWindow, []( GLFWwindow* window, int key, int scancode, int action, int mods )
    {
        Synthesizer* synth = static_cast<Synthesizer*>( glfwGetWindowUserPointer( window ) );

        if (action == GLFW_PRESS)
            synth->noteDown(key, 1.0f);
        else if(action == GLFW_RELEASE)
            synth->noteUp(key);
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
