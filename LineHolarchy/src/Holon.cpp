#include "Holon.h"

Holon::Holon(glm::vec2 position, glm::vec2 s, bool mono) {
	pos.x = position.x; pos.y = position.y;
	size.x = s.x; size.y = s.y;
	monochrome = mono;
	assignGenes();
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

void Holon::updateGenes(glm::vec3 newGenes) {
	genes[0] = newGenes.x; genes[1] = newGenes.y; genes[2] = newGenes.z; 
}

float *Holon::getGenes() {
	return genes; 
}

void Holon::assignGenes() {
	for (int i = 0; i < 3; i++) {
		genes[i] = ofRandom(1);
	}
}

ofColor Holon::createColorFromGenes() {
	int red = ofMap(genes[0], 0, 1, 0, 255, true);
	int green = ofMap(genes[1], 0, 1, 0, 255, true);
	int blue = ofMap(genes[2], 0, 1, 0, 255, true);
	
	return monochrome ? ofColor(red) : ofColor(red, green, blue);
}
