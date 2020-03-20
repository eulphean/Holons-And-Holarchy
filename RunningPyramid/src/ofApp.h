#pragma once

#include "ofMain.h"
#include "Holon.h"

class Pyramid {
	public:
		Pyramid() {
		
		}
	
		void init(glm::vec2 layerSize, std::vector<Holon> &holons) {
			if (holons.size() > 0) {
				fbo.allocate(layerSize.x, layerSize.y, GL_RGBA);
				fbo.begin();
					ofBackground(ofColor::red);
					for (int i = 0; i < holons.size(); i++) {
						holons[i].draw();
					}
				fbo.end();
			}
		}
	
		void update(std::vector<Holon> &holons) {
			if (holons.size() > 0) {
				fbo.begin();
					for (int i = 0; i < holons.size(); i++) {
						holons[i].draw();
					}
				fbo.end();
			}
		}
	
		void draw() {
			fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	
	private:
		ofFbo fbo;
		int depthHeight = 50;
		int runningYPos = 0;
		int holarchyDepth;
};

class Population {
	public:
		Population() {
			
		}
	
		void init() {
			
		}
	
		void selection() {
			
		}
	
		void reproduction() {
			
		}
	
		Holon getRandomHolon();
	
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size);
	
		void mutate(Holon &a);
	
	private:
		std::vector<Holon> holons;
		float mutationProbability;
		float crossoverProbability;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
	private:
		std::vector<Holon> holons;
		glm::vec2 layerSize;
		Pyramid pyramid;
		Population population; 
};

