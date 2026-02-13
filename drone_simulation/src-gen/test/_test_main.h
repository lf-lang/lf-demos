#ifndef _TEST_MAIN_H
#define _TEST_MAIN_H
#include "include/core/reactor.h"
#include "_avoidplanner.h"
#include "_csv_reader.h"
#include "_debugrcprinter.h"
#include "_userlandcmd.h"
#ifndef TOP_LEVEL_PREAMBLE_1296612741_H
#define TOP_LEVEL_PREAMBLE_1296612741_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// LF runtime provides this; forward-declare to avoid missing headers.
void lf_request_stop(void);
#endif // TOP_LEVEL_PREAMBLE_1296612741_H
#ifndef TOP_LEVEL_PREAMBLE_1726519676_H
#define TOP_LEVEL_PREAMBLE_1726519676_H
#include <math.h>
#include <stdio.h>
#endif // TOP_LEVEL_PREAMBLE_1726519676_H
#ifndef TOP_LEVEL_PREAMBLE_648214521_H
#define TOP_LEVEL_PREAMBLE_648214521_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif // TOP_LEVEL_PREAMBLE_648214521_H
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
    struct self_base_t base;
#line 53 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_test_main.h"
#line 54 "/home/duckie/drone_msp_env/lf-demos/drone_simulation/src-gen/test/_test_main.h"
} _test_main_main_self_t;
_test_main_main_self_t* new__test_main();
#endif // _TEST_MAIN_H
