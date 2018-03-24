#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    int bufferSize        = 512;
    sampleRate             = 44100;
    phase                 = 0;
    phaseAdder             = 0.0f;
    phaseAdderTarget     = 0.0f;
    volume                = 0.1f;
    bNoise                 = false;
    
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    
    soundStream.printDeviceList();
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    gui.setup();
    gui.add(volume.setup("volume", 0.5, 0.0, 1.0));
    
    ttf.load("big_noodle_titling.ttf", 200);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(245);
    ofSetColor(20);
    ofSetLineWidth(2);
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2-(10*(perKeySize+keyGap)/2), ofGetHeight()/2-(3*(perKeySize+keyGap)/2));
    for (int i = 0; i < 4; i++) {
        ofPushMatrix();
        if (i != 3) {
            ofTranslate(50*i, 0);
        } else {
            ofTranslate(150*i, 0);
        }
        for (int j = 0; j < rowKeys[i]; j++) {
            if (keyPress && keyMap[0] == i && keyMap[1] == j) {
                ofFill();
                pan = (float)j/(float)rowKeys[i];
                targetFrequency = 2000.0f/4 * (i+j)+(i+1*j+1);
                phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
            } else  {
                ofNoFill();
            }
            if (i != 3) {
                ofDrawRectangle((perKeySize+keyGap)*j, (perKeySize+keyGap)*i, perKeySize, perKeySize);
            } else {
                ofDrawRectangle((perKeySize+keyGap)*j, (perKeySize+keyGap)*i, perKeySize*5+keyGap*4, perKeySize);
            }
            
        }
        ofPopMatrix();
    }
    ofPopMatrix();
    
    ofPushStyle();
    ofSetColor(20);
    ttf.drawString(alphabet, ofGetWidth()/2-100, 400);
    ofPopStyle();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    //pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;
    
    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }
    
    if ( bNoise == true){
        // ---------------------- noise --------------
        for (int i = 0; i < bufferSize; i++){
            lAudio[i] = output[i*nChannels    ] = ofRandom(0, 1) * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = ofRandom(0, 1) * volume * rightScale;
        }
    } else {
        phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
        for (int i = 0; i < bufferSize; i++){
            phase += phaseAdder;
            float sample = sin(phase);
            lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    for (int k = 0; k < totalKeys[4]; k++) {
        if (key == charKeys[k]) {
            alphabet = charKeys[k];
            soundStream.start();
            keyPress = true;
            for (int l = 0; l < 4; l++) {
                if (k > totalKeys[l]-1 && k < totalKeys[l+1]) {
                    keyMap[0] = l;
                }
                for (int m = 0; m < rowKeys[l]; m++) {
                    if (k-totalKeys[l] == m) {
                        keyMap[1] = m;
                    }
                }
            }
        }
    }
    if (key == ' ') {
        bNoise = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key) {
        soundStream.stop();
        keyPress = false;
        for (int i = 0; i < 2; i++) {
            keyMap[i] = -1;
        }
    }
    if (key == ' ') {
        bNoise = false;
    }

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::exit(){

}
