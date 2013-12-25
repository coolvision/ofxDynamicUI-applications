/*
 * Patch.h
 *
 *  Created on: Nov 6, 2013
 *      Author: sk
 */

#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxFontStash.h"

#include "Message.h"
#include "Port.h"

class Connection {
public:
    Connection(Port *from_in, Port *to_in) :
            from(from_in), to(to_in) {};
    Port *from;
    Port *to;
};

enum AppStatus {
    RESPONSIVE = 0, UNRESPONSIVE, NOT_RUNNING,
};

enum PatchType {
    PATCH = 0, UI_BUTTON, UI_FIXED_BUTTON, UI_SLIDER
};

// base patch: can be connected to other patches
class Patch: public ofxMSAInteractiveObject {
public:
    Patch();
    virtual ~Patch();

    static ofTrueTypeFont labels_font_m;
    static ofxFontStash font;

    string label;
    int uid; // used for referencing, can be an index in the patches array

    // gui
    void onPress(int x, int y, int button);
    void onDragOver(int x, int y, int button);
    void onDragOutside(int x, int y, int button);
    void onRelease(int x, int y, int button);
    void onReleaseOutside(int x, int y, int button);
    void onDragUpdate(int x, int y, int button);
    virtual bool hitTest(int tx, int ty);
    void setDraggable(bool d);
    void onReleaseAny(int x, int y, int button);

    bool hover;
    bool selected;
    bool dragging;
    bool draggable;
    ofPoint drag_start;

    bool port_connect;
    bool port_hover;
    ofPoint line_start;
    Port *hover_port;



    // events
    ofEvent<string> click_event;

    bool ports_queried;

    // ports should be queried from the app
    vector<Port *> ports_in;
    vector<Port *> ports_out;
    vector<Connection *> connections;

    void *client;
    AppStatus status;

    uint32_t update_i;
    uint32_t ping_i;
    uint32_t unresponsive_i;

    // update connections with other patches
    // move from the outbox to the inboxes of connected patches
    void update();

    uint32_t type;
    bool opened;
    bool automatic_address;
    string name;
    string path;
    Address address; // ip, of some MQ address





 // ui stuff
//===================================================

    // captions
    ofRectangle title_box;
    ofRectangle address_box;



    // ports
    float max_w_in;
    float max_w_out;
    float max_y;

    // visual stuff
    ofColor border_color;
    ofColor hover_border_color;
    ofColor color;
    ofColor text_color;
    ofColor hover_color;
    ofColor hover_text_color;

    void draw();
    void drawButton();
    void drawFixedButton();
    void drawPatch();
    void portsSize(float off_y);
    void drawPorts(float off_y);
};

// visual block representation of an application
class AppPatch: public Patch {
public:

    AppPatch(bool opened, string name_in, string path, int x, int y, int width,
            int height);
    virtual ~AppPatch();

    virtual bool hitTest(int tx, int ty);

    void click(string &s);
};

// buttons used for the toolbox
class ButtonPatch: public Patch {
public:
    ButtonPatch(string label, int x, int y, int w, int h);
    virtual ~ButtonPatch();
};
