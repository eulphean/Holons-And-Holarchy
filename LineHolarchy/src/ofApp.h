#pragma once

#include "ofMain.h"
#include "Holon.h"
#include "Holarchy.h"
#include "StripHolarchy.h"
#include "FrameHolarchy.h"
#include "ofxGui.h"

class GuiParams {
	public:
		GuiParams() {
			// Initialize the GUI params.
			generalParams.setName("Holarchy Params");
			auto currentBufferSize = glm::vec2(ofGetWidth(), ofGetHeight());
			auto minBufferSize = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
			auto maxBufferSize = glm::vec2(ofGetWidth()*10, ofGetHeight()*10);
			generalParams.add(bufferSize.set("Buffer Size", currentBufferSize, minBufferSize, maxBufferSize));
			generalParams.add(depth.set("Holarchy Depth", 4, 1, 25));
			generalParams.add(monochrome.set("Monochrome", true));
			generalParams.add(orderedStart.set("Ordered Start", true));
			generalParams.add(gradient.set("Gradient", true));
			generalParams.add(mixValue.set("Mix Value", 0.8, 0.0, 1.0));
			generalParams.add(alternateMixValue.set("Alternate Mix Value", true));
		}
	
		// Variables.
		ofParameterGroup generalParams;
    ofParameter<glm::vec2> bufferSize;
    ofParameter<int> depth;
    ofParameter<bool> monochrome;
    ofParameter<bool> orderedStart;
    ofParameter<float> mixValue;
    ofParameter<bool> gradient;
    ofParameter<bool> alternateMixValue;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit(); 
		void initGui(); 
	
		void initLineHolarchy();
		void initPageHolarchy(); 
		void mateHolons();
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size, int i);
	
		void keyPressed(int key);
	
		// GUI
		ofxPanel gui;
		ofParameterGroup otherParams;
		ofParameter<bool> isStripHolarchy;
		GuiParams params; 
	
    bool hideGui = false; 
	
	private:
		StripHolarchy stripHolarchy;
		FrameHolarchy frameHolarchy;
};

