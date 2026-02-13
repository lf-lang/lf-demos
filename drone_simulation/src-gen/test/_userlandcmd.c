#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/UserLandCmd/UserLandCmd.h"
#include "_userlandcmd.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _userlandcmdreaction_function_0(void* instance_args) {
    _userlandcmd_self_t* self = (_userlandcmd_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 29 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/UserLandCmd.lf"
    kb_setup();
#line 15 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_userlandcmd.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _userlandcmdreaction_function_1(void* instance_args) {
    _userlandcmd_self_t* self = (_userlandcmd_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 30 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/UserLandCmd.lf"
    kb_restore();
#line 24 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_userlandcmd.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _userlandcmdreaction_function_2(void* instance_args) {
    _userlandcmd_self_t* self = (_userlandcmd_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _userlandcmd_out_t* out = &self->_lf_out;
    #line 32 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/UserLandCmd.lf"
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
      if (c=='l' || c=='L') {
        lf_set(out, 1);       // emit once per keypress
        fprintf(stderr, "[UserLandCmd] Land requested.\n");
      }
    }
#line 39 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_userlandcmd.c"
}
#include "include/api/reaction_macros_undef.h"
_userlandcmd_self_t* new__userlandcmd() {
    _userlandcmd_self_t* self = (_userlandcmd_self_t*)lf_new_reactor(sizeof(_userlandcmd_self_t));
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _userlandcmdreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = NULL;
    self->_lf__reaction_1.number = 1;
    self->_lf__reaction_1.function = _userlandcmdreaction_function_1;
    self->_lf__reaction_1.self = self;
    self->_lf__reaction_1.deadline_violation_handler = NULL;
    self->_lf__reaction_1.STP_handler = NULL;
    self->_lf__reaction_1.name = "?";
    self->_lf__reaction_1.mode = NULL;
    self->_lf__reaction_2.number = 2;
    self->_lf__reaction_2.function = _userlandcmdreaction_function_2;
    self->_lf__reaction_2.self = self;
    self->_lf__reaction_2.deadline_violation_handler = NULL;
    self->_lf__reaction_2.STP_handler = NULL;
    self->_lf__reaction_2.name = "?";
    self->_lf__reaction_2.mode = NULL;
    self->_lf__poll.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__poll.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__poll_reactions[0] = &self->_lf__reaction_2;
    self->_lf__poll.reactions = &self->_lf__poll_reactions[0];
    self->_lf__poll.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__poll.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__poll.is_timer = true;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__poll.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
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
    return self;
}
