#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);
        void exit();
        void audioOut(float * input, int bufferSize, int nChannels);
    
    
    ofSoundStream soundStream;
    
    float   pan;
    int     sampleRate;
    bool    bNoise;
    
    vector <float> lAudio;
    vector <float> rAudio;
    
    //------------------- for the simple sine wave synthesis
    float   targetFrequency;
    float   phase;
    float   phaseAdder;
    float   phaseAdderTarget;
    
    ofxPanel gui;
    ofxSlider<float> volume;
    
    int     perKeySize = 150;
    int     keyGap = 25;
    int     keyMap[2] = {-1, -1};
    bool    keyPress = false;
    int     rowKeys[4] = {10, 9, 7, 1};
    int     totalKeys[5] = {0, 10, 19, 26, 27};
    char    charKeys[27] = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ' '};
    
    ofTrueTypeFont ttf;
    std::basic_string<char> alphabet;
		
};
