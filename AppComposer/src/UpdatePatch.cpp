/*
 * UpdatePatch.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: sk
 */

#include "Patch.h"

void Patch::update() {

    update_i++;

    if (client != NULL) {

        if (update_i % 30 == 0) {

            if (status != UNRESPONSIVE) {
                Message m("ping");
                m.addIntValue(ping_i);
                ping_i++;
                m.send(client);
                cout << "send ping to " << address.str << " " << ping_i << endl;
            }

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
