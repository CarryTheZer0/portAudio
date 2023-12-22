
#include <iostream>

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

    SineWave mySine;
	AdsrEnvelope myEnvelope(&mySine);

	myEnvelope.setAttackTime(0.2f);
	myEnvelope.setDecayTime(0.05f);
	myEnvelope.setDecayTime(1.0f);

	AudioStream myStream = AudioStream(&myEnvelope);

    myStream.openStream(2, inputDevice, inDeviceInfo->defaultLowInputLatency, 2, outputDevice, outDeviceInfo->defaultLowOutputLatency);
	myStream.startPlayback();

    while (true)
    	

    err = Pa_Terminate();
	if( err != paNoError )
	   printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	return 0;
}