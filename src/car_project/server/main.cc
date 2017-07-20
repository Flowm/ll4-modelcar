#define GENODE_TARGET focnados_panda
//#define GENODE_RPI
#define GENODE_PANDA

#ifdef GENODE_RPI
    #include "main_rpi.cc"
#endif

#ifdef GENODE_PANDA
    #include "main_panda.cc"
#endif
