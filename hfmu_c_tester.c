#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "fmi/fmi2functions.h"

typedef int (*foo)(int);
typedef bool (*instantiateLEG)(char[]);
typedef void* (*instantiate)(char[]);
typedef int (*printEnv)(void*);
typedef bool (*updateEnv)(void*);

void cbLogger(fmi2ComponentEnvironment cEnv, fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message, ...){
  printf("msg: %s\n", message);
}



void test(void) {
  // Load the library
  void *handle;
  handle = dlopen("/Users/casperthule/source/haskell/HFMU/HFMU/dist/build/HFMU/libHFMU.dylib", RTLD_LAZY);
  if(!handle){
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
  }
  dlerror();
  // Setup experiment
  fmi2Type type = fmi2CoSimulation;
  fmi2CallbackFunctions cbFuncs = {.logger = &cbLogger, .allocateMemory = NULL, .freeMemory = NULL, .stepFinished = NULL, .componentEnvironment = NULL};
  fmi2InstantiateTYPE* fmi2Instantiate;
  fmi2Instantiate = dlsym(handle, "fmi2Instantiate");
  fmi2Component comp;
  comp = fmi2Instantiate("hsInstance", type, "guid", "resourceLoc", &cbFuncs, false, false);
  printf("C: Result of fmi2Instantiate %p\n", comp);

  // Comment setup experiment to see Haskell fail because the end time has not been set
  fmi2SetupExperimentTYPE * fmi2SetupExperiment;
  fmi2SetupExperiment = dlsym(handle, "fmi2SetupExperiment");
  fmi2Status result = fmi2SetupExperiment(comp, false, 0, 0, false, 2.25);


  fmi2EnterInitializationModeTYPE* fmi2EnterInitializationMode;
  fmi2EnterInitializationMode = dlsym(handle, "fmi2EnterInitializationMode");
  result = fmi2EnterInitializationMode(comp);
  printf("C: Result of fmi2EnterInitializationMode: %i\n", result);


  //  cbFuncs.allocateMemory = NULL;
  //cbFuncs.freeMemory = NULL;
  //cbFuncs.stepFinished = NULL;
  //cbFuncs.componentEnvironment = NULL;

  //foo fooF;
  //fooF = dlsym(handle, "foo");
  //int a;
  //a=fooF(5);
  //printf("C: Result of fooF: %i\r\n", a);

  //instantiateLEG instantiateLEGF;
  //bool b;
  //char name[5] = {'H', 'F', 'M', 'U', '\0'};
  //instantiateLEGF = dlsym(handle, "instantiateLEG");
  //b = instantiateLEGF(name);
  //printf("C: Result of instantiateLEGF: %i\r\n", b);

  //instantiate instantiateF;
  //void* c;
  //instantiateF = dlsym(handle, "instantiate");
  //c = instantiateF(name);
  //printf("C: Result of instantiateF: %i\r\n", c);

  //printEnv printEnvF;
  //int d;
  //printEnvF = dlsym(handle, "printEnv");
  //d = printEnvF(c);
  //printf("C: Result of printEnv: %i\r\n", d);

  //updateEnv updateEnvF;
  //bool e;
  //updateEnvF = dlsym(handle, "updateEnv");
  //d = updateEnvF(c);
  //printf("C: Result of updateEnv: %i\r\n", e);

  //int f;
  //printEnvF = dlsym(handle, "printEnv");
  //f = printEnvF(c);
  //printf("C: Result of printEnv: %i\r\n", f);

  dlclose(handle);
}

int main(int argc, char *argv[]) {
  test(); 
  return 0;
}
