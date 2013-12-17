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
}

Patch::~Patch() {
}

void Patch::draw() {
}

void Patch::setDraggable(bool d) {
    this->draggable = d;
}

void Patch::update() {

    update_i++;

    if (client != NULL) {

        if (update_i % 30 == 0) {

            Message m("ping");
            m.addIntValue(ping_i);
            ping_i++;
            m.send(client);

            // check for ping replies
            zmq_msg_t msg;
            zmq_msg_init(&msg);
            if (zmq_msg_recv(&msg, client, ZMQ_DONTWAIT) != -1) {
                Message m(&msg);
                status = RESPONSIVE;
                unresponsive_i = 0;

                if (!ports_queried) {

                    Message in("input_ports");
                    in.send(client);
                    cout << "query input ports" << endl;
                    zmq_msg_t in_msg;
                    zmq_msg_init(&in_msg);
                    if (zmq_msg_recv(&in_msg, client, 0) != -1) {
                        Message m(&in_msg);

                        for (int i = 0; i < m.string_v.size(); i++) {
                            ports.push_back(
                                    new Port(m.string_v[i], Port::INPUT));
                        }

                    }
                    zmq_msg_close(&in_msg);

                    Message out("output_ports");
                    out.send(client);
                    cout << "query output ports" << endl;
                    zmq_msg_t out_msg;
                    zmq_msg_init(&out_msg);
                    if (zmq_msg_recv(&out_msg, client, 0) != -1) {
                        Message m(&out_msg);

                        for (int i = 0; i < m.string_v.size(); i++) {
                            ports.push_back(
                                    new Port(m.string_v[i], Port::OUTPUT));
                        }
                    }
                    zmq_msg_close(&out_msg);

                    ports_queried = true;
                }

            } else {
                unresponsive_i++;
            }
            zmq_msg_close(&msg);
        }
    }

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