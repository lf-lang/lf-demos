#ifndef _USERLANDCMD_H
#define _USERLANDCMD_H
#include "include/core/reactor.h"
#ifndef TOP_LEVEL_PREAMBLE_2018270965_H
#define TOP_LEVEL_PREAMBLE_2018270965_H
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static struct termios _old_tio;
static void kb_setup(void){
  struct termios tio;
  tcgetattr(STDIN_FILENO, &_old_tio);
  tio = _old_tio;
  cfmakeraw(&tio);
  tcsetattr(STDIN_FILENO, TCSANOW, &tio);
  // non-blocking stdin
  int fl = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK);
  fprintf(stderr, "[UserLandCmd] Press 'l' or 'L' to land.\n");
}
static void kb_restore(void){ tcsetattr(STDIN_FILENO, TCSANOW, &_old_tio); }
#endif // TOP_LEVEL_PREAMBLE_2018270965_H
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
} _userlandcmd_out_t;
typedef struct {
    struct self_base_t base;
#line 42 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_userlandcmd.h"
#line 43 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_userlandcmd.h"
    _userlandcmd_out_t _lf_out;
    int _lf_out_width;
    reaction_t _lf__reaction_0;
    reaction_t _lf__reaction_1;
    reaction_t _lf__reaction_2;
    trigger_t _lf__poll;
    reaction_t* _lf__poll_reactions[1];
    trigger_t _lf__startup;
    reaction_t* _lf__startup_reactions[1];
    trigger_t _lf__shutdown;
    reaction_t* _lf__shutdown_reactions[1];
} _userlandcmd_self_t;
_userlandcmd_self_t* new__userlandcmd();
#endif // _USERLANDCMD_H
