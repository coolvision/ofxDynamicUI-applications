/*
 * Button.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: sk
 */

#include "Patch.h"

ButtonPatch::ButtonPatch(string label, int x, int y, int w, int h) {

    this->label = label;
    this->x = x;
    this->y = y;
    width = w;
    height = h;
}

ButtonPatch::~ButtonPatch() {
}

void ButtonPatch::draw() {

    ofPushStyle();

    bool mouse_over = isMouseOver();

    // draw the rectangle
    ofSetRectMode (OF_RECTMODE_CORNER);
    ofSetLineWidth(1.0f);

    ofFill();
    if (mouse_over) {
        ofSetColor(hover_color);
    } else {
        ofSetColor(color);
    }
    ofRect(x, y, width, height);

    ofFill();
    ofSetColor(text_color / 2.0f);
    ofRect(x, y, height, height);

    ofNoFill();
    if (mouse_over) {
        ofSetColor(hover_border_color);
    } else {
        ofSetColor(border_color);
    }
    ofRect(x, y, width, height);

    // and the text!
    if (mouse_over) {
        ofSetColor(hover_text_color);
    } else {
        ofSetColor(text_color);
    }

    font.draw(label, 16, (int) (x + 5 + height), (int) (y + 15));

    ofRectangle b = labels_font_m.getStringBoundingBox(label,
            (int) (x + 5 + height), (int) (y + 15));
    if (b.width > width - 50) {
        width = b.width + 50;
    }

//    ofFill();
//    ofSetColor(text_color);
//    ofRect(x + width - 5 + 4, y + 4, 5, height - 1 - 8);

    ofPopStyle();
}

