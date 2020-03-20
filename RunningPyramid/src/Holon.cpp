#include "Holon.h"

Holon::Holon(glm::vec2 position, int height) {
	pos.x = position.x; pos.y = position.y;
	hasReproduced = false; 
	assignGenes();
	size.x = getWidthFromGenes(); size.y = height;
}

void Holon::draw() {
	ofPushMatrix();
		ofTranslate(pos);
		ofPushStyle();
			ofColor c = createColorFromGenes();
			ofSetColor(c);
			ofDrawRectangle(0, 0, size.x, size.y);
		ofPopStyle();
	ofPopMatrix();
}

glm::vec2 &Holon::getSize() {
	return size;
}

glm::vec2 &Holon::getPosition() {
	return pos;
}

void Holon::assignGenes() {
	for (int i = 0; i < numGenes-1; i++) {
		genes[i] = ofRandom(1);
	}
	
	genes[numGenes-1] = ofRandom(0.002);
}

ofColor Holon::createColorFromGenes() {
	int hue = ofMap(genes[0], 0, 1, 0, 255, true);
	int saturation = ofMap(genes[1], 0, 1, 0, 255, true);
	int brightness = ofMap(genes[2], 0, 1, 0, 255, true);
	return ofColor::fromHsb(hue, saturation, brightness);
}

float Holon::getWidthFromGenes() {
	// This is the maximum width there can be.
	return ofMap(genes[3], 0, 1, 0, ofGetWidth());
}
