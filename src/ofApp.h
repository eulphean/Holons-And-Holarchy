#pragma once

#include "ofMain.h"
#include "Holon.h"

class Layer {
	public:
		Layer(int gSize) {
			layer.allocate(ofGetWidth()*2, ofGetHeight()*2, GL_RGBA);
			gridSize = gSize;
		}
	
		void create(std::vector<std::vector<Holon>> &holons) {
			if (holons.size() > 0) {
				layer.begin();
					// Draw the holon.
					for (int row = 0; row < gridSize; row++) {
						for (int col = 0; col < gridSize; col++) {
							Holon h = holons[row][col];
							h.draw();
						}
					}
				layer.end();
			} else {
				layer.begin();
					ofBackground(ofColor::black);
				layer.end(); 
			}
		}
	
		void draw() {
			layer.draw(0, 0); 
		}
	
		void saveToImage(int numIdx) {
			ofPixels pix;
			layer.readToPixels(pix);
			ofSaveImage(pix, "image-" + ofToString(numIdx) + ".jpg", OF_IMAGE_QUALITY_BEST);
		}
	
	private:
		ofFbo layer;
		int gridSize; // Rows and columns are same.
}; 

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void initSystem(); 
		void createLayers();
		void combineHolons();
		Holon getRandomHolon();
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size);
		void mutate(Holon &a); 

		void keyPressed(int key);
	
	private:
		float gridSize;
		std::vector<vector<Holon>> holons;
		std::vector<Layer> layers;
		int layerIdx = 0;
		float mutationProbability;
		float crossoverProbability;
		int numGenerations; 
};

