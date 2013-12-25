/*
 * DrawPatch.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: sk
 */

#include "Patch.h"

void Patch::draw() {

    if (isMouseOver()) {
        hover = true;
    } else {
        hover = false;
    }

    ofPushStyle();
    ofSetColor(150.0f);
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    ofSetLineWidth(2.0f);

    if (port_connect) {
        ofLine(line_start, ofPoint(ofGetMouseX(), ofGetMouseY()));
    }

    // also draw all connections
    for (int i = 0; i < connections.size(); i++) {
        ofLine(connections[i]->from->cp, connections[i]->to->cp);
    }

    ofPopStyle();


    switch (type) {
    case PATCH:
        drawPatch();
        break;
    case UI_BUTTON:
        drawButton();
        break;
    case UI_FIXED_BUTTON:
        drawFixedButton();
        break;
    };
}

void Patch::drawButton() {

    ofPushStyle();

    portsSize(height - 8);

    ofFill();
    ofSetColor(color);
    if (hover) {
        ofSetColor(color - ofColor(20));
    }
    ofRect(x, y, width, height);

    ofSetColor(150);
    ofRect(x, y, height, height);

    drawPorts(height - 8);

    ofPopStyle();
}

void Patch::drawFixedButton() {

    ofPushStyle();

    ofFill();
    ofSetColor(color);
    if (hover) {
        ofSetColor(color - ofColor(20));
    }
    ofRect(x, y, width, height);

    ofSetColor(text_color);
    font.draw(label, 16, (int) (x + 5), (int) y + height - 7);

    ofPopStyle();
}

void Patch::drawPatch() {

    ofPoint mouse(ofGetMouseX(), ofGetMouseY());

    ofPushStyle();

    portsSize(20);

    if (max_y > 0.0f) {
        height = max_y - y + 10;
    }



    title_box = font.getBBox(name + " - " + "uid: " + ofToString(uid), 16,
            (int) (x + 5), (int) (y - 3));
    title_box.y -= 5;
    title_box.height += 10;

    if (title_box.inside(mouse)) {
        ofSetColor(ofColor::steelBlue);
    } else {
        ofSetColor(text_color);
    }


    font.draw(name + " - " + "uid: " + ofToString(uid), 16,
            (int) (x + 5), (int) (y - 3));

    address_box = font.getBBox(address.str, 16,
            (int) (x + 5), (int) (y + height + 14));
    address_box.y -= 5;
    address_box.height += 10;

    if (address_box.inside(mouse)) {
        ofSetColor(ofColor::steelBlue);
    } else {
        ofSetColor(text_color);
    }

    font.draw(address.str, 16,
              (int) (x + 5), (int) (y + height + 14));

    ofFill();

    if (status == UNRESPONSIVE) {
        ofSetColor(ofColor::gray);
    } else {
        ofSetColor(color);
        if (hover) {
            ofSetColor(color - ofColor(20));
        }
    }

    ofRect(x, y, width, height);

    drawPorts(20);


    ofPopStyle();
}

void Patch::portsSize(float off_y) {

    max_w_in = -1.0f;
    max_w_out = -1.0f;
    max_y = -1.0f;

    ofSetColor(ofColor::black);
    ofNoFill();
    for (int i = 0; i < ports_in.size(); i++) {

        ofRectangle r = font.getBBox(ports_in[i]->name, 16, (int) (x + 10),
                (int) (y + off_y + 20 * i));

        ports_in[i]->r = r;

        if (r.width > max_w_in) {
            max_w_in = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }

    for (int i = 0; i < ports_out.size(); i++) {

        ofRectangle r = font.getBBox(ports_out[i]->name, 16,
                (int) (x + 10 + width - 100), (int) (y + off_y + 20 * i));

        ports_out[i]->r = r;

        if (r.width > max_w_out) {
            max_w_out = r.width;
        }
        if (r.getMaxY() > max_y) {
            max_y = r.getMaxY();
        }
    }
}

void Patch::drawPorts(float off_y) {

    ofPoint mouse(ofGetMouseX(), ofGetMouseY());

    port_hover = false;

    for (int i = 0; i < ports_in.size(); i++) {

        ofSetColor(ofColor::black);
        ofNoFill();
        Patch::font.draw(ports_in[i]->name, 16, (int) (x + 10),
                (int) (y + off_y + 20 * i));
        //ofRect(ports_in[i]->r);

        ofRectangle port_c = ports_in[i]->r;
        port_c.x = x;
        port_c.width = 30;

        ofRectangle socket = ports_in[i]->r;
        socket.x = x;
        socket.width = 5;

        ports_in[i]->cp.set(socket.x, socket.y + socket.height / 2);
        ofFill();
        if (port_c.inside(mouse)) {
            hover_port = ports_in[i];
            port_hover = true;
            line_start = ports_in[i]->cp;
            ofSetColor(ofColor::red);
        } else {
            ofSetColor(ofColor::orange);
        }
        ofRect(socket);
    }

    for (int i = 0; i < ports_out.size(); i++) {

        ofSetColor(ofColor::black);
        ofNoFill();
        font.draw(ports_out[i]->name, 16,
                (int) (x + width - ports_out[i]->r.width - 10),
                (int) (y + off_y + 20 * i));
        //ofRect(ports_out[i]->r);

        ofRectangle port_c = ports_out[i]->r;
        port_c.x = x + width - 30;
        port_c.width = 30;

        ofRectangle socket = ports_out[i]->r;
        socket.x = x + width - 5;
        socket.width = 5;

        ports_out[i]->cp.set(socket.x + socket.width,
                socket.y + socket.height / 2);
        ofFill();
        if (port_c.inside(mouse)) {
            hover_port = ports_out[i];
            port_hover = true;
            line_start = ports_out[i]->cp;
            ofSetColor(ofColor::red);
        } else {
            ofSetColor(ofColor::orange);
        }
        ofRect(socket);
    }
}

