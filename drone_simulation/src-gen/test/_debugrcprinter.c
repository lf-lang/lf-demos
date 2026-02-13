#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/DebugRcPrinter/DebugRcPrinter.h"
#include "_debugrcprinter.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_0(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 31 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->fp = fopen(self->out_path, "w");
    if (!self->fp) {
      fprintf(stderr, "[DebugRcPrinter] Failed to open %s\n", self->out_path);
      return;
    }
    fprintf(self->fp, "step,roll,pitch,yaw,throttle,aux1,aux2\n");
    fflush(self->fp);
#line 21 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_1(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 41 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    if (self->fp) fclose(self->fp);
    self->fp = NULL;
#line 31 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_2(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_roll_t* roll = self->_lf_roll;
    int roll_width = self->_lf_roll_width; SUPPRESS_UNUSED_WARNING(roll_width);
    #line 46 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->rr  = roll->value;
#line 41 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_3(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_pitch_t* pitch = self->_lf_pitch;
    int pitch_width = self->_lf_pitch_width; SUPPRESS_UNUSED_WARNING(pitch_width);
    #line 47 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->pp  = pitch->value;
#line 51 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_4(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_yaw_t* yaw = self->_lf_yaw;
    int yaw_width = self->_lf_yaw_width; SUPPRESS_UNUSED_WARNING(yaw_width);
    #line 48 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->yy  = yaw->value;
#line 61 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_5(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_throttle_t* throttle = self->_lf_throttle;
    int throttle_width = self->_lf_throttle_width; SUPPRESS_UNUSED_WARNING(throttle_width);
    #line 49 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->thr = throttle->value;
#line 71 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_6(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_aux1_t* aux1 = self->_lf_aux1;
    int aux1_width = self->_lf_aux1_width; SUPPRESS_UNUSED_WARNING(aux1_width);
    #line 50 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->a1  = aux1->value;
#line 81 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_7(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_aux2_t* aux2 = self->_lf_aux2;
    int aux2_width = self->_lf_aux2_width; SUPPRESS_UNUSED_WARNING(aux2_width);
    #line 51 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    self->a2  = aux2->value;
#line 91 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _debugrcprinterreaction_function_8(void* instance_args) {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _debugrcprinter_step_t* step = self->_lf_step;
    int step_width = self->_lf_step_width; SUPPRESS_UNUSED_WARNING(step_width);
    #line 54 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf"
    if (!self->fp) return;
    
    fprintf(self->fp, "%d,%d,%d,%d,%d,%d,%d\n",
      step->value,
      self->rr, self->pp, self->yy, self->thr, self->a1, self->a2
    );
    fflush(self->fp);
#line 107 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_debugrcprinter.c"
}
#include "include/api/reaction_macros_undef.h"
_debugrcprinter_self_t* new__debugrcprinter() {
    _debugrcprinter_self_t* self = (_debugrcprinter_self_t*)lf_new_reactor(sizeof(_debugrcprinter_self_t));
    // Set input by default to an always absent default input.
    self->_lf_step = &self->_lf_default__step;
    // Set the default source reactor pointer
    self->_lf_default__step._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_roll = &self->_lf_default__roll;
    // Set the default source reactor pointer
    self->_lf_default__roll._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_pitch = &self->_lf_default__pitch;
    // Set the default source reactor pointer
    self->_lf_default__pitch._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_yaw = &self->_lf_default__yaw;
    // Set the default source reactor pointer
    self->_lf_default__yaw._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_throttle = &self->_lf_default__throttle;
    // Set the default source reactor pointer
    self->_lf_default__throttle._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_aux1 = &self->_lf_default__aux1;
    // Set the default source reactor pointer
    self->_lf_default__aux1._base.source_reactor = (self_base_t*)self;
    // Set input by default to an always absent default input.
    self->_lf_aux2 = &self->_lf_default__aux2;
    // Set the default source reactor pointer
    self->_lf_default__aux2._base.source_reactor = (self_base_t*)self;
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _debugrcprinterreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = NULL;
    self->_lf__reaction_1.number = 1;
    self->_lf__reaction_1.function = _debugrcprinterreaction_function_1;
    self->_lf__reaction_1.self = self;
    self->_lf__reaction_1.deadline_violation_handler = NULL;
    self->_lf__reaction_1.STP_handler = NULL;
    self->_lf__reaction_1.name = "?";
    self->_lf__reaction_1.mode = NULL;
    self->_lf__reaction_2.number = 2;
    self->_lf__reaction_2.function = _debugrcprinterreaction_function_2;
    self->_lf__reaction_2.self = self;
    self->_lf__reaction_2.deadline_violation_handler = NULL;
    self->_lf__reaction_2.STP_handler = NULL;
    self->_lf__reaction_2.name = "?";
    self->_lf__reaction_2.mode = NULL;
    self->_lf__reaction_3.number = 3;
    self->_lf__reaction_3.function = _debugrcprinterreaction_function_3;
    self->_lf__reaction_3.self = self;
    self->_lf__reaction_3.deadline_violation_handler = NULL;
    self->_lf__reaction_3.STP_handler = NULL;
    self->_lf__reaction_3.name = "?";
    self->_lf__reaction_3.mode = NULL;
    self->_lf__reaction_4.number = 4;
    self->_lf__reaction_4.function = _debugrcprinterreaction_function_4;
    self->_lf__reaction_4.self = self;
    self->_lf__reaction_4.deadline_violation_handler = NULL;
    self->_lf__reaction_4.STP_handler = NULL;
    self->_lf__reaction_4.name = "?";
    self->_lf__reaction_4.mode = NULL;
    self->_lf__reaction_5.number = 5;
    self->_lf__reaction_5.function = _debugrcprinterreaction_function_5;
    self->_lf__reaction_5.self = self;
    self->_lf__reaction_5.deadline_violation_handler = NULL;
    self->_lf__reaction_5.STP_handler = NULL;
    self->_lf__reaction_5.name = "?";
    self->_lf__reaction_5.mode = NULL;
    self->_lf__reaction_6.number = 6;
    self->_lf__reaction_6.function = _debugrcprinterreaction_function_6;
    self->_lf__reaction_6.self = self;
    self->_lf__reaction_6.deadline_violation_handler = NULL;
    self->_lf__reaction_6.STP_handler = NULL;
    self->_lf__reaction_6.name = "?";
    self->_lf__reaction_6.mode = NULL;
    self->_lf__reaction_7.number = 7;
    self->_lf__reaction_7.function = _debugrcprinterreaction_function_7;
    self->_lf__reaction_7.self = self;
    self->_lf__reaction_7.deadline_violation_handler = NULL;
    self->_lf__reaction_7.STP_handler = NULL;
    self->_lf__reaction_7.name = "?";
    self->_lf__reaction_7.mode = NULL;
    self->_lf__reaction_8.number = 8;
    self->_lf__reaction_8.function = _debugrcprinterreaction_function_8;
    self->_lf__reaction_8.self = self;
    self->_lf__reaction_8.deadline_violation_handler = NULL;
    self->_lf__reaction_8.STP_handler = NULL;
    self->_lf__reaction_8.name = "?";
    self->_lf__reaction_8.mode = NULL;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__startup.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__startup_reactions[0] = &self->_lf__reaction_0;
    self->_lf__startup.last_tag = NEVER_TAG;
    self->_lf__startup.reactions = &self->_lf__startup_reactions[0];
    self->_lf__startup.number_of_reactions = 1;
    self->_lf__startup.is_timer = false;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__shutdown.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__shutdown_reactions[0] = &self->_lf__reaction_1;
    self->_lf__shutdown.last_tag = NEVER_TAG;
    self->_lf__shutdown.reactions = &self->_lf__shutdown_reactions[0];
    self->_lf__shutdown.number_of_reactions = 1;
    self->_lf__shutdown.is_timer = false;
    self->_lf__step.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__step.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__step_reactions[0] = &self->_lf__reaction_8;
    self->_lf__step.reactions = &self->_lf__step_reactions[0];
    self->_lf__step.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__step.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__step.tmplt.type.element_size = sizeof(int);
    self->_lf__roll.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__roll.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__roll_reactions[0] = &self->_lf__reaction_2;
    self->_lf__roll.reactions = &self->_lf__roll_reactions[0];
    self->_lf__roll.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__roll.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__roll.tmplt.type.element_size = sizeof(int);
    self->_lf__pitch.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__pitch.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__pitch_reactions[0] = &self->_lf__reaction_3;
    self->_lf__pitch.reactions = &self->_lf__pitch_reactions[0];
    self->_lf__pitch.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__pitch.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__pitch.tmplt.type.element_size = sizeof(int);
    self->_lf__yaw.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__yaw.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__yaw_reactions[0] = &self->_lf__reaction_4;
    self->_lf__yaw.reactions = &self->_lf__yaw_reactions[0];
    self->_lf__yaw.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__yaw.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__yaw.tmplt.type.element_size = sizeof(int);
    self->_lf__throttle.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__throttle.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__throttle_reactions[0] = &self->_lf__reaction_5;
    self->_lf__throttle.reactions = &self->_lf__throttle_reactions[0];
    self->_lf__throttle.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__throttle.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__throttle.tmplt.type.element_size = sizeof(int);
    self->_lf__aux1.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__aux1.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__aux1_reactions[0] = &self->_lf__reaction_6;
    self->_lf__aux1.reactions = &self->_lf__aux1_reactions[0];
    self->_lf__aux1.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__aux1.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__aux1.tmplt.type.element_size = sizeof(int);
    self->_lf__aux2.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__aux2.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__aux2_reactions[0] = &self->_lf__reaction_7;
    self->_lf__aux2.reactions = &self->_lf__aux2_reactions[0];
    self->_lf__aux2.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__aux2.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__aux2.tmplt.type.element_size = sizeof(int);
    return self;
}
