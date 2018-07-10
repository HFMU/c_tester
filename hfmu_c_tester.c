#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
typedef int (*cfoo)(int);
void test(void) {
  void *handle;
  cfoo cfooF;

  handle = dlopen("/Users/casperthule/source/haskell/HFMU/dist/build/HFMU/libHFMU.dylib", RTLD_LAZY);

  if(!handle){
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
  }

  dlerror();
    cfooF = dlsym(handle, "cfoo");

  int a;
  a=cfooF(5);

  printf("%i\n", a);
  
  dlclose(handle);
}

int main(int argc, char *argv[]) {
  test(); 
  return 0;
}
