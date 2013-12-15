/*
 * AppPatch.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: sk
 */

#include "Patch.h"

AppPatch::AppPatch(string name, string path, int x, int y, int width,
        int height) {

    this->name = name;
    this->path = path;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

AppPatch::~AppPatch() {
}

bool AppPatch::open(string path) {
}

bool AppPatch::sendToApp() {
}

bool AppPatch::receiveFromApp() {
}

bool AppPatch::requestFromApp() {
}

void AppPatch::draw() {

    ofNoFill();

//	if (selected) {
//		ofSetColor(ofColor::darkRed);
//	} else {
    ofSetColor(border_color);
//	}
    ofRect(x, y, width, height);

    ofSetColor(text_color);
    font.draw(name, 16, (int) (x + 5), (int) (y - 3));
}

