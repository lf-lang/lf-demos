#include "include/api/schedule.h"
#include <string.h>
#include "low_level_platform/api/low_level_platform.h"
#include "include/test/test.h"
#include "_test_main.h"
// ***** Start of method declarations.
// ***** End of method declarations.
_test_main_main_self_t* new__test_main() {
    _test_main_main_self_t* self = (_test_main_main_self_t*)lf_new_reactor(sizeof(_test_main_main_self_t));

    return self;
}
