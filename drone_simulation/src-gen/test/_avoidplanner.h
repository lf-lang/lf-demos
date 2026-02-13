#ifndef _AVOIDPLANNER_H
#define _AVOIDPLANNER_H
#include "include/core/reactor.h"
#ifndef TOP_LEVEL_PREAMBLE_1726519676_H
#define TOP_LEVEL_PREAMBLE_1726519676_H
#include <math.h>
#include <stdio.h>
#endif // TOP_LEVEL_PREAMBLE_1726519676_H
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
} _avoidplanner_front_t;
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
} _avoidplanner_left_t;
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
} _avoidplanner_right_t;
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
} _avoidplanner_bottom_t;
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
} _avoidplanner_top_t;
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
} _avoidplanner_land_t;
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
} _avoidplanner_roll_t;
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
} _avoidplanner_pitch_t;
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
} _avoidplanner_yaw_t;
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
} _avoidplanner_throttle_t;
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
} _avoidplanner_aux1_t;
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
} _avoidplanner_aux2_t;
typedef struct {
    struct self_base_t base;
#line 180 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.h"
    #line 29 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double height_sp;
    #line 30 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int thr_base;
    #line 31 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int thr_min;
    #line 32 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int thr_max;
    #line 34 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double k_h_p;
    #line 35 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double k_h_i;
    #line 36 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double i_h_min;
    #line 37 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double i_h_max;
    #line 39 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double ceil_min_clear;
    #line 40 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double ceil_soft_band;
    #line 42 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double front_thresh;
    #line 43 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double side_target;
    #line 44 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double k_side;
    #line 45 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double k_yaw;
    #line 46 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int pitch_fwd;
    #line 47 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int pitch_slow;
    #line 50 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int ticks;
    #line 51 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int takeoff;
    #line 52 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double i_h;
    #line 53 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int hover_ok;
    #line 54 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int ok_count;
    #line 57 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int land_thr;
    #line 58 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int land_touch;
    #line 59 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int land_min_thr;
    #line 60 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int land_step;
    #line 63 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int t_neutral;
    #line 64 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int t_arm;
    #line 65 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    int t_takeoff;
#line 237 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.h"
    _avoidplanner_front_t* _lf_front;
    // width of -2 indicates that it is not a multiport.
    int _lf_front_width;
    // Default input (in case it does not get connected)
    _avoidplanner_front_t _lf_default__front;
    _avoidplanner_left_t* _lf_left;
    // width of -2 indicates that it is not a multiport.
    int _lf_left_width;
    // Default input (in case it does not get connected)
    _avoidplanner_left_t _lf_default__left;
    _avoidplanner_right_t* _lf_right;
    // width of -2 indicates that it is not a multiport.
    int _lf_right_width;
    // Default input (in case it does not get connected)
    _avoidplanner_right_t _lf_default__right;
    _avoidplanner_bottom_t* _lf_bottom;
    // width of -2 indicates that it is not a multiport.
    int _lf_bottom_width;
    // Default input (in case it does not get connected)
    _avoidplanner_bottom_t _lf_default__bottom;
    _avoidplanner_top_t* _lf_top;
    // width of -2 indicates that it is not a multiport.
    int _lf_top_width;
    // Default input (in case it does not get connected)
    _avoidplanner_top_t _lf_default__top;
    _avoidplanner_land_t* _lf_land;
    // width of -2 indicates that it is not a multiport.
    int _lf_land_width;
    // Default input (in case it does not get connected)
    _avoidplanner_land_t _lf_default__land;
    _avoidplanner_roll_t _lf_roll;
    int _lf_roll_width;
    _avoidplanner_pitch_t _lf_pitch;
    int _lf_pitch_width;
    _avoidplanner_yaw_t _lf_yaw;
    int _lf_yaw_width;
    _avoidplanner_throttle_t _lf_throttle;
    int _lf_throttle_width;
    _avoidplanner_aux1_t _lf_aux1;
    int _lf_aux1_width;
    _avoidplanner_aux2_t _lf_aux2;
    int _lf_aux2_width;
    reaction_t _lf__reaction_0;
    reaction_t _lf__reaction_1;
    reaction_t _lf__reaction_2;
    reaction_t _lf__reaction_3;
    reaction_t _lf__reaction_4;
    reaction_t _lf__reaction_5;
    reaction_t _lf__reaction_6;
    reaction_t _lf__reaction_7;
    reaction_t _lf__reaction_8;
    reaction_t _lf__reaction_9;
    trigger_t _lf__tick;
    reaction_t* _lf__tick_reactions[5];
    trigger_t _lf__reset;
    reaction_t* _lf__reset_reactions[5];
    trigger_t _lf__front;
    reaction_t* _lf__front_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__left;
    reaction_t* _lf__left_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__right;
    reaction_t* _lf__right_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__bottom;
    reaction_t* _lf__bottom_reactions[2];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__top;
    reaction_t* _lf__top_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    trigger_t _lf__land;
    reaction_t* _lf__land_reactions[1];
    #ifdef FEDERATED
    
    #endif // FEDERATED
    reactor_mode_t _lf__modes[5];
} _avoidplanner_self_t;
_avoidplanner_self_t* new__avoidplanner();
#endif // _AVOIDPLANNER_H
