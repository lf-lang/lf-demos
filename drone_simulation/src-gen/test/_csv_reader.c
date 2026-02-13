#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/CSV_reader/CSV_reader.h"
#include "_csv_reader.h"
// ***** Start of method declarations.
// ***** End of method declarations.
#include "include/api/reaction_macros.h"
void _csv_readerreaction_function_0(void* instance_args) {
    _csv_reader_self_t* self = (_csv_reader_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 28 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    self->fp = fopen(self->path, "r");
    self->header_done = 0;
    self->k = 0;
    
    if (!self->fp) {
      fprintf(stderr, "[CSV_reader] Failed to open %s\n", self->path);
      lf_request_stop();
    } else {
      fprintf(stderr, "[CSV_reader] Replaying %s\n", self->path);
    }
#line 24 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_csv_reader.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _csv_readerreaction_function_1(void* instance_args) {
    _csv_reader_self_t* self = (_csv_reader_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    
    #line 41 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    if (self->fp) fclose(self->fp);
    self->fp = NULL;
#line 34 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_csv_reader.c"
}
#include "include/api/reaction_macros_undef.h"
#include "include/api/reaction_macros.h"
void _csv_readerreaction_function_2(void* instance_args) {
    _csv_reader_self_t* self = (_csv_reader_self_t*)instance_args; SUPPRESS_UNUSED_WARNING(self);
    _csv_reader_front_t* front = &self->_lf_front;
    _csv_reader_bottom_t* bottom = &self->_lf_bottom;
    _csv_reader_left_t* left = &self->_lf_left;
    _csv_reader_right_t* right = &self->_lf_right;
    _csv_reader_top_t* top = &self->_lf_top;
    _csv_reader_step_t* step = &self->_lf_step;
    #line 46 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf"
    if (!self->fp) {
      fprintf(stderr, "[CSV_reader] No file handle. Stopping.\n");
      lf_request_stop();
      return;
    }
    
    char line[512];
    
    // Skip header once
    if (!self->header_done) {
      if (!fgets(line, sizeof(line), self->fp)) {
        fprintf(stderr, "[CSV_reader] Empty file. Stopping.\n");
        lf_request_stop();
        return;
      }
      self->header_done = 1;
    }
    
    // Read next data line
    if (!fgets(line, sizeof(line), self->fp)) {
      fprintf(stderr, "[CSV_reader] End of file. Stopping.\n");
      fclose(self->fp);
      self->fp = NULL;
      lf_request_stop();
      return;
    }
    
    // Expected columns:
    // front_m,bottom_m,left_m,right_m,top_m,obstacle_within_0p20m_direction
    // Empty fields are allowed (become NAN).
    double vals[5];
    for (int i = 0; i < 5; i++) vals[i] = NAN;
    
    char* p = line;
    for (int col = 0; col < 5; col++) {
      char* comma = strchr(p, ',');
      if (comma) *comma = '\0';
    
      // Empty string -> keep NAN
      if (p[0] != '\0' && p[0] != '\n' && p[0] != '\r') {
        vals[col] = strtod(p, NULL);
      }
    
      if (!comma) break;
      p = comma + 1;
    }
    
    // Publish values that exist
    if (!isnan(vals[0])) lf_set(front,  vals[0]);
    if (!isnan(vals[1])) lf_set(bottom, vals[1]);
    if (!isnan(vals[2])) lf_set(left,   vals[2]);
    if (!isnan(vals[3])) lf_set(right,  vals[3]);
    if (!isnan(vals[4])) lf_set(top,    vals[4]);
    
    // Step counter increments per successful row
    self->k += 1;
    lf_set(step, self->k);
#line 104 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_csv_reader.c"
}
#include "include/api/reaction_macros_undef.h"
_csv_reader_self_t* new__csv_reader() {
    _csv_reader_self_t* self = (_csv_reader_self_t*)lf_new_reactor(sizeof(_csv_reader_self_t));
    self->_lf__reaction_0.number = 0;
    self->_lf__reaction_0.function = _csv_readerreaction_function_0;
    self->_lf__reaction_0.self = self;
    self->_lf__reaction_0.deadline_violation_handler = NULL;
    self->_lf__reaction_0.STP_handler = NULL;
    self->_lf__reaction_0.name = "?";
    self->_lf__reaction_0.mode = NULL;
    self->_lf__reaction_1.number = 1;
    self->_lf__reaction_1.function = _csv_readerreaction_function_1;
    self->_lf__reaction_1.self = self;
    self->_lf__reaction_1.deadline_violation_handler = NULL;
    self->_lf__reaction_1.STP_handler = NULL;
    self->_lf__reaction_1.name = "?";
    self->_lf__reaction_1.mode = NULL;
    self->_lf__reaction_2.number = 2;
    self->_lf__reaction_2.function = _csv_readerreaction_function_2;
    self->_lf__reaction_2.self = self;
    self->_lf__reaction_2.deadline_violation_handler = NULL;
    self->_lf__reaction_2.STP_handler = NULL;
    self->_lf__reaction_2.name = "?";
    self->_lf__reaction_2.mode = NULL;
    self->_lf__tick.last_tag = NEVER_TAG;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__tick.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
    #endif // FEDERATED_DECENTRALIZED
    self->_lf__tick_reactions[0] = &self->_lf__reaction_2;
    self->_lf__tick.reactions = &self->_lf__tick_reactions[0];
    self->_lf__tick.number_of_reactions = 1;
    #ifdef FEDERATED
    self->_lf__tick.physical_time_of_arrival = NEVER;
    #endif // FEDERATED
    self->_lf__tick.is_timer = true;
    #ifdef FEDERATED_DECENTRALIZED
    self->_lf__tick.intended_tag = (tag_t) { .time = NEVER, .microstep = 0u};
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
