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
    APPLICARION_PATCH = 0,
    UI_PATCH
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
    virtual void draw();
    void onPress(int x, int y, int button);
    void onDragOver(int x, int y, int button);
    void onDragOutside(int x, int y, int button);
    void onRelease(int x, int y, int button);
    void onReleaseOutside(int x, int y, int button);
    void onDragUpdate(int x, int y, int button);
    bool hitTest(int tx, int ty);
    void setDraggable(bool d);

    bool selected;
    bool dragging;
    bool draggable;
    ofPoint drag_start;

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
    vector<Port *> ports;
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
};

// visual block representation of an application
class AppPatch: public Patch {
public:

    AppPatch(string name_in, string path, int x, int y, int width, int height);
    ~AppPatch();

    // App
    string name;
    string path; // local FS path
    uint32_t pid; // can monitor the process?
    Address address; // ip, of some MQ address

    bool open(string path); // initialize connected app

    // communicate to the app, update message queues
    bool sendToApp();
    bool receiveFromApp();
    bool requestFromApp();

    void draw();
};

// buttons used for the toolbox
class ButtonPatch: public Patch {
public:
    ButtonPatch(string label, int x, int y, int w, int h);
    ~ButtonPatch();

    void draw();
};
