/*
 *  Copyright 2012 Rackspace
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

extern "C" {
  #include "virgo__util.h"
  #include "virgo_brand.h"
  #include "virgo_portable.h"
  #include "virgo_paths.h"
  #include "virgo.h"
};

#include "../deps/breakpad/src/client/linux/handler/exception_handler.h"

google_breakpad::ExceptionHandler *virgo_global_exception_handler = NULL;

static bool dumpCallback(const char* dump_path,
                         const char* minidump_id,
                         void* context,
                         bool succeeded)
{
  printf("FATAL ERROR: Crash Dump written to: %s/%s-crash-report-%s.dmp\n", dump_path, VIRGO_DEFAULT_NAME, minidump_id);
  return succeeded;
}

extern "C" {

  void
  virgo__crash_reporter_init() {
    char path[VIRGO_PATH_MAX];
    virgo_t* v = NULL;
    virgo_error_t* err = virgo__paths_get(v, VIRGO_PATH_PERSISTENT_DIR, path, sizeof(path));

    if (err) {
      printf("Could not create breakpad handler because no temporary dir could be found.");
      return;
    }
    printf(path, '\n\n');
    virgo_global_exception_handler = new google_breakpad::ExceptionHandler(path , NULL, dumpCallback, NULL, true);    
  };

  void
  virgo__crash_reporter_destroy() {
    delete virgo_global_exception_handler;
  };
};

