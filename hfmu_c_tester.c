#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "fmi/fmi2functions.h"
#include "TargetConditionals.h"

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

  char* libPath = "/Users/casperthule/source/haskell/HFMU/ex_water-tank/dist-newstyle/build/x86_64-osx/ghc-8.4.4/ex-water-tank-0.1.0.0/f/watertankController/build/watertankController/libwatertankController.dylib";
  //Old
 //handle = dlopen(libPath, RTLD_LAZY);

    //New
  handle = dlopen(
                  "/Users/casperthule/source/haskell/HFMU/ex_water-tank/dist-newstyle/build/x86_64-osx/ghc-8.4.4/ex-water-tank-0.1.0.0/f/watertankController/build/watertankController/libwatertankController.dylib",
                  RTLD_LAZY |
                  RTLD_LOCAL |
                  RTLD_FIRST
                  );

  if(!handle){
      fprintf(stderr, " Failed to open library: %s\n", dlerror());
      exit(EXIT_FAILURE);
  }
  dlerror();

  printf("processing fmi2Instantiate\n");

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

  printf("C: min to 1.0 and max to 5.0\n");
  fmi2SetRealTYPE* fmi2SetReal;
  fmi2SetReal = dlsym(handle, "fmi2SetReal");
  unsigned int setRealVRef1[2] = {0,2};
  double setRealVal[2] = {1.0,5.0};
  result = fmi2SetReal(comp, setRealVRef1, 2, setRealVal);
  printf("C: Results of fmi2SetReal: %i\n",result);

  fmi2ExitInitializationModeTYPE* fmi2ExitInitializationMode;
  fmi2ExitInitializationMode = dlsym(handle, "fmi2ExitInitializationMode");
  result = fmi2ExitInitializationMode(comp);
  printf("C: Results of fmi2ExitInitializationMode: %i\n",result);

  double level = 6.0;
  printf("C: Setting level to %f. Expected valve: Open\n",level);
  unsigned int setLevelRef[1] = {1};
  double setLevelVal[1] = {level};
  result = fmi2SetReal(comp, setLevelRef, 1, setLevelVal);
  printf("C: Results of fmi2SetReal: %i\n",result);

  fmi2DoStepTYPE* fmi2DoStep;
  fmi2DoStep = dlsym(handle, "fmi2DoStep");
  double ccp = 0.0;
  double css = 0.1;
  bool noSetPrior = true;
  result = fmi2DoStep(comp, ccp, css, noSetPrior);
  printf("C: Results of fmi2DoStep: %i\n",result);

  fmi2GetBooleanTYPE* fmi2GetBoolean;
  fmi2GetBoolean = dlsym(handle,"fmi2GetBoolean");
  unsigned int valveVRef[1] = {3};
  int* valveVal = (int *)malloc(sizeof(int)*1);
  result = fmi2GetBoolean(comp, valveVRef, 1, valveVal);
  printf("C: Results of fmi2GetBoolean: %i\n",result);
  printf("C: Valve output: %i\n", *valveVal);

  level = 4.0;
  printf("C: Setting level to %f. Expected valve: Open\n",level);
  setLevelVal[0] = level;
  result = fmi2SetReal(comp, setLevelRef, 1, setLevelVal);
  printf("C: Results of fmi2SetReal: %i\n",result);

  ccp = 0.1;
  css = 0.1;
  result = fmi2DoStep(comp, ccp, css, noSetPrior);
  printf("C: Results of fmi2DoStep: %i\n",result);

  result = fmi2GetBoolean(comp, valveVRef, 1, valveVal);
  printf("C: Results of fmi2GetBoolean: %i\n",result);
  printf("C: Valve output: %i\n", *valveVal);

  level = 0.0;
  printf("C: Setting level to %f. Expected valve: Closed\n", level);
  setLevelVal[0] = level;
  result = fmi2SetReal(comp, setLevelRef, 1, setLevelVal);
  printf("C: Results of fmi2SetReal: %i\n",result);

  ccp = 0.2;
  css = 0.1;
  result = fmi2DoStep(comp, ccp, css, noSetPrior);
  printf("C: Results of fmi2DoStep: %i\n",result);

  result = fmi2GetBoolean(comp, valveVRef, 1, valveVal);
  printf("C: Results of fmi2GetBoolean: %i\n",result);
  printf("C: Valve output: %i\n", *valveVal);


  level = 4;
  printf("C: Setting level to %f. Expected valve: Closed\n",level);
  setLevelVal[0] = level;
  result = fmi2SetReal(comp, setLevelRef, 1, setLevelVal);
  printf("C: Results of fmi2SetReal: %i\n",result);

  ccp = 0.3;
  css = 0.1;
  result = fmi2DoStep(comp, ccp, css, noSetPrior);
  printf("C: Results of fmi2DoStep: %i\n",result);

  result = fmi2GetBoolean(comp, valveVRef, 1, valveVal);
  printf("C: Results of fmi2GetBoolean: %i\n",result);
  printf("C: Valve output: %i\n", *valveVal);

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
