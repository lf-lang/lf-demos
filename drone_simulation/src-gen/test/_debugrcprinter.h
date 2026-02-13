#ifndef _DEBUGRCPRINTER_H
#define _DEBUGRCPRINTER_H
#include "include/core/reactor.h"
#ifndef TOP_LEVEL_PREAMBLE_648214521_H
#define TOP_LEVEL_PREAMBLE_648214521_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif // TOP_LEVEL_PREAMBLE_648214521_H
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
} _debugrcprinter_step_t;
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
} _debugrcprinter_roll_t;
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
} _debugrcprinter_pitch_t;
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
} _debugrcprinter_yaw_t;
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
} _debugrcprinter_throttle_t;
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
} _debugrcprinter_aux1_t;
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
} _debugrcprinter_aux2_t;
typedef struct {
    struct self_base_t base;
    #line 9 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    string out_path;
#line 113 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.h"
    #line 21 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    FILE* fp;
    #line 23 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int rr;
    #line 24 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int pp;
    #line 25 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int yy;
    #line 26 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int thr;
    #line 27 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int a1;
    #line 28 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    int a2;
#line 128 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.h"
    _debugrcprinter_step_t* _lf_step;
    // width of -2 indicates that it is not a multiport.
    int _lf_step_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_step_t _lf_default__step;
    _debugrcprinter_roll_t* _lf_roll;
    // width of -2 indicates that it is not a multiport.
    int _lf_roll_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_roll_t _lf_default__roll;
    _debugrcprinter_pitch_t* _lf_pitch;
    // width of -2 indicates that it is not a multiport.
    int _lf_pitch_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_pitch_t _lf_default__pitch;
    _debugrcprinter_yaw_t* _lf_yaw;
    // width of -2 indicates that it is not a multiport.
    int _lf_yaw_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_yaw_t _lf_default__yaw;
    _debugrcprinter_throttle_t* _lf_throttle;
    // width of -2 indicates that it is not a multiport.
    int _lf_throttle_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_throttle_t _lf_default__throttle;
    _debugrcprinter_aux1_t* _lf_aux1;
    // width of -2 indicates that it is not a multiport.
    int _lf_aux1_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_aux1_t _lf_default__aux1;
    _debugrcprinter_aux2_t* _lf_aux2;
    // width of -2 indicates that it is not a multiport.
    int _lf_aux2_width;
    // Default input (in case it does not get connected)
    _debugrcprinter_aux2_t _lf_default__aux2;
    reaction_t _lf__reaction_0;
    reaction_t _lf__reaction_1;
    reaction_t _lf__reaction_2;
    reaction_t _lf__reaction_3;
    reaction_t _lf__reaction_4;
    reaction_t _lf__reaction_5;
    reaction_t _lf__reaction_6;
    reaction_t _lf__reaction_7;
    reaction_t _lf__reaction_8;
    trigger_t _lf__startup;
    reaction_t* _lf__startup_reactions[1];
    trigger_t _lf__shutdown;
    reaction_t* _lf__shutdown_reactions[1];
    trigger_t _lf__step;
    reaction_t* _lf__step_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__roll;
    reaction_t* _lf__roll_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__pitch;
    reaction_t* _lf__pitch_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__yaw;
    reaction_t* _lf__yaw_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__throttle;
    reaction_t* _lf__throttle_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__aux1;
    reaction_t* _lf__aux1_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__aux2;
    reaction_t* _lf__aux2_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
} _debugrcprinter_self_t;
_debugrcprinter_self_t* new__debugrcprinter();
#endif // _DEBUGRCPRINTER_H
