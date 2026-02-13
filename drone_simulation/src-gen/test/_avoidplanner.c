#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/avoid_planner_modal/AvoidPlanner.h"
#include "_avoidplanner.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_0(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 71 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    self->ticks = 0;
    self->takeoff = 1300;
    self->i_h = 0.0;
    self->hover_ok = 0;
    self->ok_count = 0;
    self->land_thr = 1300;
    self->land_touch = 0;
    printf("[AvoidPlanner] Neutral\n");
#line 22 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_1(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _avoidplanner_roll_t* roll = &self->_lf_roll;
    _avoidplanner_pitch_t* pitch = &self->_lf_pitch;
    _avoidplanner_yaw_t* yaw = &self->_lf_yaw;
    _avoidplanner_throttle_t* throttle = &self->_lf_throttle;
    _avoidplanner_aux1_t* aux1 = &self->_lf_aux1;
    _avoidplanner_aux2_t* aux2 = &self->_lf_aux2;
    reactor_mode_t* Arm = &self->_lf__modes[1];
    lf_mode_change_type_t _lf_Arm_change_type = reset_transition;
    #line 82 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    lf_set(roll, 1500);
    lf_set(pitch, 1500);
    lf_set(yaw, 1500);
    lf_set(throttle, 1000);
    lf_set(aux1, 1000);   // disarmed
    lf_set(aux2, 1800);   // ANGLE
    if (++self->ticks >= self->t_neutral) {
      lf_set_mode(Arm);
    }
#line 46 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_2(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 96 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    self->ticks = 0;
    printf("[AvoidPlanner] Arm\n");
#line 56 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_3(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _avoidplanner_roll_t* roll = &self->_lf_roll;
    _avoidplanner_pitch_t* pitch = &self->_lf_pitch;
    _avoidplanner_yaw_t* yaw = &self->_lf_yaw;
    _avoidplanner_throttle_t* throttle = &self->_lf_throttle;
    _avoidplanner_aux1_t* aux1 = &self->_lf_aux1;
    _avoidplanner_aux2_t* aux2 = &self->_lf_aux2;
    reactor_mode_t* takeoff = &self->_lf__modes[2];
    lf_mode_change_type_t _lf_takeoff_change_type = reset_transition;
    #line 101 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    lf_set(roll, 1500);
    lf_set(pitch, 1500);
    lf_set(yaw, 1500);
    lf_set(throttle, 1000);
    lf_set(aux1, 1800);   // arm
    lf_set(aux2, 1800);
    if (++self->ticks >= self->t_arm) {
      lf_set_mode(takeoff);
      printf("[AvoidPlanner] Arm -> takeoff\n");
    }
#line 81 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_4(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 116 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    self->ticks = 0;
    self->takeoff  = 1300;
    printf("[AvoidPlanner] takeoff\n");
#line 92 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_5(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _avoidplanner_roll_t* roll = &self->_lf_roll;
    _avoidplanner_pitch_t* pitch = &self->_lf_pitch;
    _avoidplanner_yaw_t* yaw = &self->_lf_yaw;
    _avoidplanner_throttle_t* throttle = &self->_lf_throttle;
    _avoidplanner_aux1_t* aux1 = &self->_lf_aux1;
    _avoidplanner_aux2_t* aux2 = &self->_lf_aux2;
    reactor_mode_t* Cruise = &self->_lf__modes[3];
    lf_mode_change_type_t _lf_Cruise_change_type = reset_transition;
    #line 122 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    if (self->takeoff < self->thr_base) self->takeoff += 5;
    int rc_thr = self->takeoff;
    if (rc_thr < self->thr_min) rc_thr = self->thr_min;
    if (rc_thr > self->thr_max) rc_thr = self->thr_max;
    
    lf_set(roll, 1500);
    lf_set(pitch, 1500);
    lf_set(yaw, 1500);
    lf_set(throttle, rc_thr);
    lf_set(aux1, 1800);
    lf_set(aux2, 1800);
    
    if (++self->ticks >= self->t_takeoff) {
      lf_set_mode(Cruise);
      printf("[AvoidPlanner] takeoff -> Cruise\n");
    }
#line 123 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_6(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 143 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    self->i_h = 0.0;
    self->hover_ok = 0;
    self->ok_count = 0;
    printf("[AvoidPlanner] Cruise\n");
#line 135 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_7(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _avoidplanner_land_t* land = self->_lf_land;
    int land_width = self->_lf_land_width; SUPPRESS_UNUSED_WARNING(land_width);
    _avoidplanner_front_t* front = self->_lf_front;
    int front_width = self->_lf_front_width; SUPPRESS_UNUSED_WARNING(front_width);
    _avoidplanner_left_t* left = self->_lf_left;
    int left_width = self->_lf_left_width; SUPPRESS_UNUSED_WARNING(left_width);
    _avoidplanner_right_t* right = self->_lf_right;
    int right_width = self->_lf_right_width; SUPPRESS_UNUSED_WARNING(right_width);
    _avoidplanner_bottom_t* bottom = self->_lf_bottom;
    int bottom_width = self->_lf_bottom_width; SUPPRESS_UNUSED_WARNING(bottom_width);
    _avoidplanner_top_t* top = self->_lf_top;
    int top_width = self->_lf_top_width; SUPPRESS_UNUSED_WARNING(top_width);
    _avoidplanner_roll_t* roll = &self->_lf_roll;
    _avoidplanner_pitch_t* pitch = &self->_lf_pitch;
    _avoidplanner_yaw_t* yaw = &self->_lf_yaw;
    _avoidplanner_throttle_t* throttle = &self->_lf_throttle;
    _avoidplanner_aux1_t* aux1 = &self->_lf_aux1;
    _avoidplanner_aux2_t* aux2 = &self->_lf_aux2;
    reactor_mode_t* Landing = &self->_lf__modes[4];
    lf_mode_change_type_t _lf_Landing_change_type = reset_transition;
    #line 151 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    // Landing request?
    int land_req = (land->is_present && land->value != 0) ? 1 : 0;
    
    double f = front->is_present  ? front->value  : NAN;
    double l = left->is_present   ? left->value   : NAN;
    double r = right->is_present  ? right->value  : NAN;
    double b = bottom->is_present ? bottom->value : NAN;
    double t = top->is_present    ? top->value    : NAN;
    
    int rc_roll  = 1500;
    int rc_pitch = 1500;
    int rc_yaw   = 1500;
    int rc_thr   = self->thr_base;
    int rc_aux1  = 1800;
    int rc_aux2  = 1800;
    
    // Altitude PI
    if (!isnan(b)) {
      double eh = self->height_sp - b;
      self->i_h += eh * 0.02 * self->k_h_i; // 20 ms tick
      if (self->i_h < self->i_h_min) self->i_h = self->i_h_min;
      if (self->i_h > self->i_h_max) self->i_h = self->i_h_max;
      rc_thr = self->thr_base + (int)(self->k_h_p * eh + self->i_h);
    }
    
    // Ceiling clamp
    if (!isnan(t)) {
      if (t < self->ceil_min_clear) {
        if (rc_thr > 1250) rc_thr = 1250;
        rc_pitch = 1500; rc_yaw = 1500;
      } else if (t < self->ceil_min_clear + self->ceil_soft_band) {
        double frac = (t - self->ceil_min_clear) / self->ceil_soft_band;
        if (frac < 0.0) frac = 0.0;
        if (frac > 1.0) frac = 1.0;
        int clamp = 1350 + (int)(250.0 * frac);
        if (rc_thr > clamp) rc_thr = clamp;
      }
    }
    
    if (rc_thr < self->thr_min) rc_thr = self->thr_min;
    if (rc_thr > self->thr_max) rc_thr = self->thr_max;
    
    // Confirm stable hover band once
    if (!self->hover_ok) {
      if (!isnan(b) && b > 0.6 && b < 1.2) {
        if (++self->ok_count >= 50) self->hover_ok = 1;
      } else {
        self->ok_count = 0;
      }
    }
    
    // Forward + avoidance
    if (self->hover_ok) {
      rc_pitch = self->pitch_fwd;
    
      if (!isnan(f) && f < self->front_thresh) {
        rc_pitch = self->pitch_slow;
        double e = self->front_thresh - f;
        int yaw_cmd = 1500 + (int)(self->k_yaw * e);
        if (yaw_cmd < 1400) yaw_cmd = 1400;
        if (yaw_cmd > 1700) yaw_cmd = 1700;
        rc_yaw = yaw_cmd;
      }
    
      double roll_bias = 0.0;
      if (!isnan(l)) roll_bias += (self->side_target - l);
      if (!isnan(r)) roll_bias -= (self->side_target - r);
      int roll_cmd = 1500 + (int)(self->k_side * roll_bias);
      if (roll_cmd < 1400) roll_cmd = 1400;
      if (roll_cmd > 1600) roll_cmd = 1600;
      rc_roll = roll_cmd;
    }
    
    // Apply outputs
    lf_set(roll, rc_roll);
    lf_set(pitch, rc_pitch);
    lf_set(yaw, rc_yaw);
    lf_set(throttle, rc_thr);
    lf_set(aux1, rc_aux1);
    lf_set(aux2, rc_aux2);
    
    // Transition to Landing if requested
    if (land_req) {
      lf_set_mode(Landing);
      printf("[AvoidPlanner] Cruise -> Landing\n");
    }
#line 248 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_8(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 242 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    // Start landing from a safe throttle near hover
    self->land_thr = (self->thr_base > 1350 ? 1350 : self->thr_base);
    self->land_touch = 0;
    printf("[AvoidPlanner] Landing\n");
#line 260 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _avoidplannerreaction_function_9(void* instance_args) {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _avoidplanner_bottom_t* bottom = self->_lf_bottom;
    int bottom_width = self->_lf_bottom_width; SUPPRESS_UNUSED_WARNING(bottom_width);
    _avoidplanner_roll_t* roll = &self->_lf_roll;
    _avoidplanner_pitch_t* pitch = &self->_lf_pitch;
    _avoidplanner_yaw_t* yaw = &self->_lf_yaw;
    _avoidplanner_throttle_t* throttle = &self->_lf_throttle;
    _avoidplanner_aux1_t* aux1 = &self->_lf_aux1;
    _avoidplanner_aux2_t* aux2 = &self->_lf_aux2;
    reactor_mode_t* Neutral = &self->_lf__modes[0];
    lf_mode_change_type_t _lf_Neutral_change_type = reset_transition;
    #line 250 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf"
    double b = bottom->is_present ? bottom->value : NAN;
    
    // Neutral attitude, keep ANGLE, armed during descent
    lf_set(roll, 1500);
    lf_set(pitch, 1500);
    lf_set(yaw, 1500);
    lf_set(aux1, 1800);
    lf_set(aux2, 1800);
    
    // Ramp down throttle smoothly
    if (self->land_thr > self->land_min_thr) self->land_thr -= self->land_step;
    
    // If we see ground close, allow lower throttle to settle
    if (!isnan(b) && b < 0.20 && self->land_thr > 1100) {
      self->land_thr = 1100;
    }
    
    lf_set(throttle, self->land_thr);
    
    // Touchdown detection: bottom distance < 8 cm for 0.3 s
    if (!isnan(b) && b < 0.08) {
      if (++self->land_touch >= 15) {
        // Disarm and return to Neutral
        lf_set(aux1, 1000);
        lf_set(throttle, 1000);
        lf_set_mode(Neutral);
        printf("[AvoidPlanner] Landing -> Neutral (touchdown)\n");
      }
    } else {
      self->land_touch = 0;
    }
#line 308 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_avoidplanner.c"
}
#include "include/api/reaction_macros_undef.h"
_avoidplanner_self_t* new__avoidplanner() {
    _avoidplanner_self_t* self = (_avoidplanner_self_t*)lf_new_reactor(sizeof(_avoidplanner_self_t));
    // Set input by default to an always absent default input.
    self->_lf_front = &self->_lf_default__front;
    // Set the default source reactor pointer
    self->_lf_default__front._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_left = &self->_lf_default__left;
    // Set the default source reactor pointer
    self->_lf_default__left._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_right = &self->_lf_default__right;
    // Set the default source reactor pointer
    self->_lf_default__right._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_bottom = &self->_lf_default__bottom;
    // Set the default source reactor pointer
    self->_lf_default__bottom._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_top = &self->_lf_default__top;
    // Set the default source reactor pointer
    self->_lf_default__top._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_land = &self->_lf_default__land;
    // Set the default source reactor pointer
    self->_lf_default__land._base.source_reactor = (self_base_t*)self;
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _avoidplannerreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = &self->_lf__modes[0];
    self->_lf__reaction_1.number = 1;
    self->_lf__reaction_1.function = _avoidplannerreaction_function_1;
    self->_lf__reaction_1.self = self;
    self->_lf__reaction_1.deadline_violation_handler = NULL;
    self->_lf__reaction_1.STP_handler = NULL;
    self->_lf__reaction_1.name = "?";
    self->_lf__reaction_1.mode = &self->_lf__modes[0];
    self->_lf__reaction_2.number = 2;
    self->_lf__reaction_2.function = _avoidplannerreaction_function_2;
    self->_lf__reaction_2.self = self;
    self->_lf__reaction_2.deadline_violation_handler = NULL;
    self->_lf__reaction_2.STP_handler = NULL;
    self->_lf__reaction_2.name = "?";
    self->_lf__reaction_2.mode = &self->_lf__modes[1];
    self->_lf__reaction_3.number = 3;
    self->_lf__reaction_3.function = _avoidplannerreaction_function_3;
    self->_lf__reaction_3.self = self;
    self->_lf__reaction_3.deadline_violation_handler = NULL;
    self->_lf__reaction_3.STP_handler = NULL;
    self->_lf__reaction_3.name = "?";
    self->_lf__reaction_3.mode = &self->_lf__modes[1];
    self->_lf__reaction_4.number = 4;
    self->_lf__reaction_4.function = _avoidplannerreaction_function_4;
    self->_lf__reaction_4.self = self;
    self->_lf__reaction_4.deadline_violation_handler = NULL;
    self->_lf__reaction_4.STP_handler = NULL;
    self->_lf__reaction_4.name = "?";
    self->_lf__reaction_4.mode = &self->_lf__modes[2];
    self->_lf__reaction_5.number = 5;
    self->_lf__reaction_5.function = _avoidplannerreaction_function_5;
    self->_lf__reaction_5.self = self;
    self->_lf__reaction_5.deadline_violation_handler = NULL;
    self->_lf__reaction_5.STP_handler = NULL;
    self->_lf__reaction_5.name = "?";
    self->_lf__reaction_5.mode = &self->_lf__modes[2];
    self->_lf__reaction_6.number = 6;
    self->_lf__reaction_6.function = _avoidplannerreaction_function_6;
    self->_lf__reaction_6.self = self;
    self->_lf__reaction_6.deadline_violation_handler = NULL;
    self->_lf__reaction_6.STP_handler = NULL;
    self->_lf__reaction_6.name = "?";
    self->_lf__reaction_6.mode = &self->_lf__modes[3];
    self->_lf__reaction_7.number = 7;
    self->_lf__reaction_7.function = _avoidplannerreaction_function_7;
    self->_lf__reaction_7.self = self;
    self->_lf__reaction_7.deadline_violation_handler = NULL;
    self->_lf__reaction_7.STP_handler = NULL;
    self->_lf__reaction_7.name = "?";
    self->_lf__reaction_7.mode = &self->_lf__modes[3];
    self->_lf__reaction_8.number = 8;
    self->_lf__reaction_8.function = _avoidplannerreaction_function_8;
    self->_lf__reaction_8.self = self;
    self->_lf__reaction_8.deadline_violation_handler = NULL;
    self->_lf__reaction_8.STP_handler = NULL;
    self->_lf__reaction_8.name = "?";
    self->_lf__reaction_8.mode = &self->_lf__modes[4];
    self->_lf__reaction_9.number = 9;
    self->_lf__reaction_9.function = _avoidplannerreaction_function_9;
    self->_lf__reaction_9.self = self;
    self->_lf__reaction_9.deadline_violation_handler = NULL;
    self->_lf__reaction_9.STP_handler = NULL;
    self->_lf__reaction_9.name = "?";
    self->_lf__reaction_9.mode = &self->_lf__modes[4];
    self->_lf__tick.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__tick.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__tick_reactions[0] = &self->_lf__reaction_1;
    self->_lf__tick_reactions[1] = &self->_lf__reaction_3;
    self->_lf__tick_reactions[2] = &self->_lf__reaction_5;
    self->_lf__tick_reactions[3] = &self->_lf__reaction_7;
    self->_lf__tick_reactions[4] = &self->_lf__reaction_9;
    self->_lf__tick.reactions = &self->_lf__tick_reactions[0];
    self->_lf__tick.number_of_reactions = 5;
    #ifdef FEDERATED
    self->_lf__tick.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__tick.is_timer = true;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__tick.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__reset.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__reset_reactions[0] = &self->_lf__reaction_0;
    self->_lf__reset_reactions[1] = &self->_lf__reaction_2;
    self->_lf__reset_reactions[2] = &self->_lf__reaction_4;
    self->_lf__reset_reactions[3] = &self->_lf__reaction_6;
    self->_lf__reset_reactions[4] = &self->_lf__reaction_8;
    self->_lf__reset.last_tag = NEVER_TAG;
    self->_lf__reset.reactions = &self->_lf__reset_reactions[0];
    self->_lf__reset.number_of_reactions = 5;
    self->_lf__reset.is_timer = false;
    self->_lf__front.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__front.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__front_reactions[0] = &self->_lf__reaction_7;
    self->_lf__front.reactions = &self->_lf__front_reactions[0];
    self->_lf__front.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__front.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__front.tmplt.type.element_size = sizeof(double);
    self->_lf__left.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__left.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__left_reactions[0] = &self->_lf__reaction_7;
    self->_lf__left.reactions = &self->_lf__left_reactions[0];
    self->_lf__left.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__left.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__left.tmplt.type.element_size = sizeof(double);
    self->_lf__right.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__right.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__right_reactions[0] = &self->_lf__reaction_7;
    self->_lf__right.reactions = &self->_lf__right_reactions[0];
    self->_lf__right.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__right.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__right.tmplt.type.element_size = sizeof(double);
    self->_lf__bottom.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__bottom.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__bottom_reactions[0] = &self->_lf__reaction_7;
    self->_lf__bottom_reactions[1] = &self->_lf__reaction_9;
    self->_lf__bottom.reactions = &self->_lf__bottom_reactions[0];
    self->_lf__bottom.number_of_reactions = 2;
    #ifdef FEDERATED
    self->_lf__bottom.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__bottom.tmplt.type.element_size = sizeof(double);
    self->_lf__top.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__top.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__top_reactions[0] = &self->_lf__reaction_7;
    self->_lf__top.reactions = &self->_lf__top_reactions[0];
    self->_lf__top.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__top.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__top.tmplt.type.element_size = sizeof(double);
    self->_lf__land.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__land.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__land_reactions[0] = &self->_lf__reaction_7;
    self->_lf__land.reactions = &self->_lf__land_reactions[0];
    self->_lf__land.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__land.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__land.tmplt.type.element_size = sizeof(int);
    // Initialize modes
    self_base_t* _lf_self_base = (self_base_t*)self;
    self->_lf__modes[0].state = &_lf_self_base->_lf__mode_state;
    self->_lf__modes[0].name = "Neutral";
    self->_lf__modes[0].deactivation_time = 0;
    self->_lf__modes[0].flags = 0;
    self->_lf__modes[1].state = &_lf_self_base->_lf__mode_state;
    self->_lf__modes[1].name = "Arm";
    self->_lf__modes[1].deactivation_time = 0;
    self->_lf__modes[1].flags = 0;
    self->_lf__modes[2].state = &_lf_self_base->_lf__mode_state;
    self->_lf__modes[2].name = "takeoff";
    self->_lf__modes[2].deactivation_time = 0;
    self->_lf__modes[2].flags = 0;
    self->_lf__modes[3].state = &_lf_self_base->_lf__mode_state;
    self->_lf__modes[3].name = "Cruise";
    self->_lf__modes[3].deactivation_time = 0;
    self->_lf__modes[3].flags = 0;
    self->_lf__modes[4].state = &_lf_self_base->_lf__mode_state;
    self->_lf__modes[4].name = "Landing";
    self->_lf__modes[4].deactivation_time = 0;
    self->_lf__modes[4].flags = 0;
    // Initialize mode state
    _lf_self_base->_lf__mode_state.parent_mode = NULL;
    _lf_self_base->_lf__mode_state.initial_mode = &self->_lf__modes[0];
    _lf_self_base->_lf__mode_state.current_mode = _lf_self_base->_lf__mode_state.initial_mode;
    _lf_self_base->_lf__mode_state.next_mode = NULL;
    _lf_self_base->_lf__mode_state.mode_change = no_transition;
    return self;
}
