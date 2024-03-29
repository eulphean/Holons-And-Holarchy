#pragma once

#include "ofMain.h"
#include "Holon.h"

class Layer {
	public:
		Layer(glm::vec2 layerResolution, glm::vec2 gSize) {
			layer.allocate(layerResolution.x, layerResolution.y, GL_RGBA);
			gridSize = gSize;
		}
	
		void create(std::vector<std::vector<Holon>> &holons) {
			if (holons.size() > 0) {
				layer.begin();
					// Draw the holon.
					for (int row = 0; row < gridSize.y; row++) {
						for (int col = 0; col < gridSize.x; col++) {
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
			layer.draw(0, 0, ofGetWidth(), ofGetHeight()); 
		}
	
		void saveToImage(int numIdx) {
			ofPixels pix;
			layer.readToPixels(pix);
			ofSaveImage(pix, "image-" + ofToString(numIdx) + ".jpg", OF_IMAGE_QUALITY_BEST);
		}
	
	private:
		ofFbo layer;
		glm::vec2 gridSize; // Rows and columns are same.
}; 

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void initSystem(); 
		void createLayers();
		void combineHolons(bool acrossWidth);
		Holon getRandomHolon();
		Holon crossover(Holon a, Holon b, glm::vec2 position, glm::vec2 size);
		void mutate(Holon &a); 

		void keyPressed(int key);
	
	private:
		glm::vec2 gridSize; // Rows and Columns. 
		std::vector<vector<Holon>> holons;
		std::vector<Layer> layers;
		int layerIdx = 0;
		float mutationProbability;
		float crossoverProbability;
		int numGenerations;
		glm::vec2 layerResolution; 
};

