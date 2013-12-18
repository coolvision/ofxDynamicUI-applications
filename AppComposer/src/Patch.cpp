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

    color.set(255.0f);
    text_color.set(0);

    hover_color.set(50.0f);
    hover_text_color.set(ofColor::lightSteelBlue);

    client = NULL;

    update_i = 0;
    ping_i = 0;
    unresponsive_i = 0;

    port_connect = false;
    port_hover = false;
}

Patch::~Patch() {
}

void Patch::setDraggable(bool d) {
    this->draggable = d;
}

void Patch::onPress(int x, int y, int button) {

    ofNotifyEvent(click_event, label);

    if (port_hover) {

        port_connect = true;

    } else {

        selected = !selected;
        dragging = true;
        drag_start.set(x - this->x, y - this->y);
    }
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

    onReleaseAny(x, y, button);
}

void Patch::onReleaseOutside(int x, int y, int button) {
    dragging = false;

    onReleaseAny(x, y, button);
}

void Patch::onReleaseAny(int x, int y, int button) {

    port_connect = false;
}

bool Patch::hitTest(int tx, int ty) {
    return ((tx > x) && (tx < x + width) && (ty > y) && (ty < y + height));
}
