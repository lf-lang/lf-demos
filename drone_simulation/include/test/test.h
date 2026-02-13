#ifndef _test_main_H
#define _test_main_H
#ifndef _TEST_MAIN_H // necessary for arduino-cli, which automatically includes headers that are not used
#ifndef TOP_LEVEL_PREAMBLE_1296612741_H
#define TOP_LEVEL_PREAMBLE_1296612741_H
/*Correspondence: Range: [(3, 0), (9, 27)) -> Range: [(0, 0), (6, 27)) (verbatim=true; src=/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/CSV_reader.lf)*/#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// LF runtime provides this; forward-declare to avoid missing headers.
void lf_request_stop(void);
#endif // TOP_LEVEL_PREAMBLE_1296612741_H
#ifndef TOP_LEVEL_PREAMBLE_1726519676_H
#define TOP_LEVEL_PREAMBLE_1726519676_H
/*Correspondence: Range: [(3, 2), (4, 18)) -> Range: [(0, 0), (1, 18)) (verbatim=true; src=/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/avoid_planner_modal.lf)*/#include <math.h>
#include <stdio.h>
#endif // TOP_LEVEL_PREAMBLE_1726519676_H
#ifndef TOP_LEVEL_PREAMBLE_648214521_H
#define TOP_LEVEL_PREAMBLE_648214521_H
/*Correspondence: Range: [(3, 0), (5, 19)) -> Range: [(0, 0), (2, 19)) (verbatim=true; src=/home/duckie/drone_msp_env/lf-demos/drone_simulation/src/DebugRcPrinter.lf)*/#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif // TOP_LEVEL_PREAMBLE_648214521_H
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
typedef struct test_self_t{
    self_base_t base; // This field is only to be used by the runtime, not the user.
    int end[0]; // placeholder; MSVC does not compile empty structs
} test_self_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} csv_reader_front_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} csv_reader_bottom_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} csv_reader_left_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} csv_reader_right_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} csv_reader_top_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} csv_reader_step_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} avoidplanner_front_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} avoidplanner_left_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} avoidplanner_right_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} avoidplanner_bottom_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    double value;

} avoidplanner_top_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_land_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_roll_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_pitch_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_yaw_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_throttle_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_aux1_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} avoidplanner_aux2_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_step_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_roll_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_pitch_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_yaw_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_throttle_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_aux1_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} debugrcprinter_aux2_t;
typedef struct {
    token_type_t type;
    lf_token_t* token;
    size_t length;
    bool is_present;
    lf_port_internal_t _base;
    int value;

} userlandcmd_out_t;
#endif
#endif
