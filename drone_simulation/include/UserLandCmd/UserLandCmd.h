#ifndef _userlandcmd_H
#define _userlandcmd_H
#ifndef _USERLANDCMD_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_2018270965_H
#define TOP_LEVEL_PREAMBLE_2018270965_H
/*Correspondence: Range: [(3, 2), (20, 76)) -> Range: [(0, 0), (17, 76)) (verbatim=true; src=/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/UserLandCmd.lf)*/#include <termios.h>
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
#ifdef __cplusplus
extern "C" {
#endif
#include "../include/api/schedule.h"
#include "../include/core/reactor.h"
#ifdef __cplusplus
}
#endif
typedef struct userlandcmd_self_t{
    self_base_t base; // This field is only to be used by the runtime, not the user.
    int end[0]; // placeholder; MSVC does not compile empty structs
} userlandcmd_self_t;
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
} userlandcmd_out_t;
#endif
#endif
