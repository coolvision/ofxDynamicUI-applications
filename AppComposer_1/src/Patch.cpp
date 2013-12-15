/*
 * Patch.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: sk
 */

#include "Patch.h"

ofTrueTypeFont Patch::labels_font_m;
ofxFontStash Patch::font;

Patch::Patch() {

    if (!labels_font_m.isLoaded()) {
        labels_font_m.loadFont("UbuntuMono-R.ttf", 12, true, true, true);
    }

    disableAllEvents();

    enableMouseEvents();

    selected = false;
    dragging = false;
    draggable = true;

    border_color.set(200.0f);
    hover_border_color.set(200.0f);
    color.set(50.0f);
    text_color.set(200.0f);
    hover_color.set(50.0f);
    hover_text_color.set(ofColor::lightSteelBlue);
}

Patch::~Patch() {
}

void Patch::draw() {
}

void Patch::setDraggable(bool d) {
    this->draggable = d;
}

void Patch::update() {
}

void Patch::onPress(int x, int y, int button) {

    ofNotifyEvent(click_event, label);

    selected = !selected;
    dragging = true;
    drag_start.set(x - this->x, y - this->y);
}

void Patch::onDragOver(int x, int y, int button) {

    onDragUpdate(x, y, button);
}

void Patch::onDragOutside(int x, int y, int button) {

    onDragUpdate(x, y, button);
}

void Patch::onDragUpdate(int x, int y, int button) {

    if (draggable && dragging) {
        this->x = x - drag_start.x;
        this->y = y - drag_start.y;
    }
}

void Patch::onRelease(int x, int y, int button) {
    dragging = false;
}

void Patch::onReleaseOutside(int x, int y, int button) {
    dragging = false;
}

bool Patch::hitTest(int tx, int ty) {
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}
