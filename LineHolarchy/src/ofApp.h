#pragma once

#include "ofMain.h"
#include "Holon.h"
#include "Holarchy.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit(); 
		void initGui(); 
	
		void initHolons();
		void mateHolons();
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size, int i);
	
		void keyPressed(int key);
	
		// GUI
		ofxPanel gui;
		ofParameterGroup generalParams;
    ofParameter<glm::vec2> bufferSize;
    ofParameter<int> depth;
    ofParameter<bool> monochrome;
    ofParameter<bool> orderedStart;
    ofParameter<float> mixValue;
    ofParameter<bool> gradient;
	
    bool hideGui = false; 
	
	private:
		std::vector<Holon> holons;
		int holarchyDepth;
		Holarchy holarchy;
};

