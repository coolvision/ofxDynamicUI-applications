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
    int to_uid; // "from" patch owns the object
    int from_port_id;
    int to_port_id;
};

enum AppStatus {
    RESPONSIVE = 0, UNRESPONSIVE, NOT_RUNNING,
};

enum PatchType {
    PATCH = 0,
    UI_BUTTON,
    UI_FIXED_BUTTON,
    UI_SLIDER
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
    bool hitTest(int tx, int ty);
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


    // visual stuff
    ofColor border_color;
    ofColor hover_border_color;
    ofColor color;
    ofColor text_color;
    ofColor hover_color;
    ofColor hover_text_color;

    // events
    ofEvent<string> click_event;

    bool ports_queried;

    // ports should be queried from the app
    vector<Port *> ports_in;
    vector<Port *> ports_out;

    vector<Connection *> connections;

    // local message queues
    queue<Message *> inbox;
    queue<Message *> outbox;

    void *client;
    AppStatus status;

    uint32_t update_i;
    uint32_t ping_i;
    uint32_t unresponsive_i;

    // update connections with other patches
    // move from the outbox to the inboxes of connected patches
    void update();

    float max_w_in;
    float max_w_out;
    float max_y;

    uint32_t type;
    string name;
    string path;
    Address address; // ip, of some MQ address

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

    AppPatch(string name_in, string path, int x, int y, int width, int height);
    virtual ~AppPatch();
};

// buttons used for the toolbox
class ButtonPatch: public Patch {
public:
    ButtonPatch(string label, int x, int y, int w, int h);
    virtual ~ButtonPatch();
};
