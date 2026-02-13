#ifndef _csv_reader_H
#define _csv_reader_H
#ifndef _CSV_READER_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_1296612741_H
#define TOP_LEVEL_PREAMBLE_1296612741_H
/*Correspondence: Range: [(3, 0), (9, 27)) -> Range: [(0, 0), (6, 27)) (verbatim=true; src=/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf)*/#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// LF runtime provides this; forward-declare to avoid missing headers.
void lf_request_stop(void);
#endif // TOP_LEVEL_PREAMBLE_1296612741_H
#ifdef __cplusplus
extern "C" {
#endif
#include "../include/api/schedule.h"
#include "../include/core/reactor.h"
#ifdef __cplusplus
}
#endif
typedef struct csv_reader_self_t{
    self_base_t base; // This field is only to be used by the runtime, not the user.
    string path;
    interval_t period;
    FILE* fp;
    int header_done;
    int k;
    int end[0]; // placeholder; MSVC does not compile empty structs
} csv_reader_self_t;
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
} csv_reader_front_t;
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
} csv_reader_bottom_t;
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
} csv_reader_left_t;
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
} csv_reader_right_t;
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
} csv_reader_top_t;
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
} csv_reader_step_t;
#endif
#endif
