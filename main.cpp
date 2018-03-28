/*
 * default_main.cpp
 *
 *  Created on: Oct 24, 2014
 *      Author: parallels
 */
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <libgen.h>
#include <signal.h>
#include <getopt.h>
#include "../include/Bela.h"

using namespace std;

// Handle Ctrl-C by requesting that the audio rendering stop
void interrupt_handler(int var)
{
	gShouldStop = true;
}

// Print usage information
void usage(const char * processName)
{
	cerr << "Usage: " << processName << " [options]" << endl;

	Bela_usage();

	cerr << "   --help [-h]:                        Print this menu\n";
}
#define FAUST_FILENAME 140
int main(int argc, char *argv[])
{
	BelaInitSettings settings;	// Standard audio settings

	struct option customOptions[] =
	{
		{"faust", 1, NULL, FAUST_FILENAME},
		{"help", 0, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	// Set default settings
	Bela_defaultSettings(&settings);
	settings.setup = setup;
	settings.render = render;
	settings.cleanup = cleanup;
	char const* faustFilename = NULL;
	// Parse command-line arguments
	while (1) {
		int c;
		if ((c = Bela_getopt_long(argc, argv, "h", customOptions, &settings)) < 0)
				break;
		switch (c) {
		case FAUST_FILENAME:
			faustFilename = optarg;
			break;
		case 'h':
			usage(basename(argv[0]));
			exit(0);
			break;
		case '?':
		default:
			usage(basename(argv[0]));
			exit(1);
		}
	}
	
	printf("Trying to parse FAUST file: %s\n", faustFilename);
	// Initialise the PRU audio device
	if(Bela_initAudio(&settings, (void*)faustFilename) != 0) {
		cout << "Error: unable to initialise audio" << endl;
		return -1;
	}

	// Start the audio device running
	if(Bela_startAudio()) {
		cout << "Error: unable to start real-time audio" << endl;
		// Stop the audio device
		Bela_stopAudio();
		// Clean up any resources allocated for audio
		Bela_cleanupAudio();
		return -1;
	}

	// Set up interrupt handler to catch Control-C and SIGTERM
	signal(SIGINT, interrupt_handler);
	signal(SIGTERM, interrupt_handler);

	// Run until told to stop
	while(!gShouldStop) {
		usleep(100000);
	}

	// Stop the audio device
	Bela_stopAudio();

	// Clean up any resources allocated for audio
	Bela_cleanupAudio();

	// All done!
	return 0;
}
