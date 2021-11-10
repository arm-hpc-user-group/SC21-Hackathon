#define __START_TRACE() {asm volatile (".inst 0x2520e020");}
#define __STOP_TRACE() {asm volatile (".inst 0x2520e040");}

int armie_start(){
  __START_TRACE();
  return 0;
}

int armie_stop(){
  __STOP_TRACE();
  return 0;
}


