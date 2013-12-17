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

void Patch::drawButton() {
    
    ofPushStyle();

    ofFill();
    ofSetColor(color);
    ofRect(x, y, width, height);

    darwPorts();

    ofPopStyle();
}

void Patch::drawFixedButton() {

    ofPushStyle();

    ofFill();
    ofSetColor(color);
    ofRect(x, y, width, height);

    ofSetColor(text_color);
    font.draw(label, 16, (int) (x + 5), (int) y + height - 7);

    ofPopStyle();
}

void Patch::drawPatch() {

    ofPushStyle();

    portsSize();

    ofSetColor(text_color);
    font.draw(name + " - " + address.str, 16, (int) (x + 5), (int) (y - 3));

    ofFill();

    if (status == UNRESPONSIVE) {
        ofSetColor(ofColor::gray);
    } else {
        ofSetColor(color);
    }
    ofRect(x, y, width, height);

    drawPorts();

    ofPopStyle();
}

void Patch::portsSize() {

    float max_w_in = -1.0f;
    float max_w_out = -1.0f;
    float max_y = -1.0f;

    ofSetColor(ofColor::black);
    ofNoFill();
    for (int i = 0; i < ports_in.size(); i++) {

        ofRectangle r = font.getBBox(ports_in[i]->name, 16, (int) (x + 10),
                                     (int) (y + 20 + 20 * i));

        ports_in[i]->r = r;

        if (r.width > max_w_in) {
            max_w_in = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }

    for (int i = 0; i < ports_out.size(); i++) {

        ofRectangle r = font.getBBox(ports_out[i]->name, 16, (int) (x + 10 + width - 100),
                                     (int) (y + 20 + 20 * i));

        ports_out[i]->r = r;

        if (r.width > max_w_out) {
            max_w_out = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }

    if (max_y > 0.0f) {
        height = max_y - y + 10;
    }

}

void Patch::drawPorts() {

    for (int i = 0; i < ports_in.size(); i++) {
        ofSetColor(ofColor::black);
        ofNoFill();
        Patch::font.draw(ports_in[i]->name, 16, (int) (x + 10),
                         (int) (y + 20 + 20 * i));
        //ofRect(ports_in[i]->r);

        ofRectangle socket = ports_in[i]->r;
        socket.x = x;
        socket.width = 5;
        ofFill();
        ofSetColor(ofColor::orange);
        ofRect(socket);
    }

    for (int i = 0; i < ports_out.size(); i++) {
        ofSetColor(ofColor::black);
        ofNoFill();
        font.draw(ports_out[i]->name, 16, (int) (x + width - ports_out[i]->r.width - 10),
                  (int) (y + 20 + 20 * i));
        //ofRect(ports_out[i]->r);

        ofRectangle socket = ports_out[i]->r;
        socket.x = x + width - 5;
        socket.width = 5;
        ofFill();
        ofSetColor(ofColor::orange);
        ofRect(socket);
    }
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
                            ports_in.push_back(
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
                            ports_out.push_back(
                                    new Port(m.string_v[i], Port::OUTPUT));
                        }
                    }
                    zmq_msg_close(&out_msg);

                    ports_queried = true;
                }

            } else {
                unresponsive_i++;

                if (unresponsive_i > 1) {
                    status = UNRESPONSIVE;
                }
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
