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

    ports_out.push_back(new Port(label, Port::OUTPUT));

    type = UI_BUTTON;
}

ButtonPatch::~ButtonPatch() {
}
