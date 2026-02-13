#ifndef _CSV_READER_H
#define _CSV_READER_H
#include "include/core/reactor.h"
#ifndef TOP_LEVEL_PREAMBLE_1296612741_H
#define TOP_LEVEL_PREAMBLE_1296612741_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// LF runtime provides this; forward-declare to avoid missing headers.
void lf_request_stop(void);
#endif // TOP_LEVEL_PREAMBLE_1296612741_H
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_front_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_bottom_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_left_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_right_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_top_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;
    #ifdef FEDERATED
    #ifdef FEDERATED_DECENTRALIZED
    tag_t intended_tag;
    #endif
    interval_t physical_time_of_arrival;
    #endif
} _csv_reader_step_t;
typedef struct {
    struct self_base_t base;
    #line 13 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    string path;
    #line 13 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    interval_t period;
#line 105 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_csv_reader.h"
    #line 23 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    FILE* fp;
    #line 24 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    int header_done;
    #line 25 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    int k;
#line 112 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_csv_reader.h"
    _csv_reader_front_t _lf_front;
    int _lf_front_width;
    _csv_reader_bottom_t _lf_bottom;
    int _lf_bottom_width;
    _csv_reader_left_t _lf_left;
    int _lf_left_width;
    _csv_reader_right_t _lf_right;
    int _lf_right_width;
    _csv_reader_top_t _lf_top;
    int _lf_top_width;
    _csv_reader_step_t _lf_step;
    int _lf_step_width;
    reaction_t _lf__reaction_0;
    reaction_t _lf__reaction_1;
    reaction_t _lf__reaction_2;
    trigger_t _lf__tick;
    reaction_t* _lf__tick_reactions[1];
    trigger_t _lf__startup;
    reaction_t* _lf__startup_reactions[1];
    trigger_t _lf__shutdown;
    reaction_t* _lf__shutdown_reactions[1];
} _csv_reader_self_t;
_csv_reader_self_t* new__csv_reader();
#endif // _CSV_READER_H
