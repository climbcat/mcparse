#ifndef __SIMCORE_H__
#define __SIMCORE_H__


#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/types.h>


struct Particle {
    double x,y,z; /* position [m] */
    double vx,vy,vz; /* velocity [m/s] */
    double sx,sy,sz; /* spin [0-1] */
    int mcgravitation; /* gravity-state */
    void *mcMagnet;    /* precession-state */
    int allow_backprop; /* allow backprop */
    /* Generic Temporaries: */
    /* May be used internally by components e.g. for special */
    /* return-values from functions used in trace, thusreturned via */
    /* particle struct. (Example: Wolter Conics from McStas, silicon slabs.) */
    double _mctmp_a; /* temp a */
    double _mctmp_b; /* temp b */
    double _mctmp_c; /* temp c */
    unsigned long randstate[7];
    double t, p;     /* time, event weight */
    long long _uid;  /* Unique event ID */
    long _index;     /* component index where to send this event */
    long _absorbed;  /* flag set to TRUE when this event is to be removed/ignored */
    long _scattered; /* flag set to TRUE when this event has interacted with the last component instance */
    long _restore;   /* set to true if neutron event must be restored */
    long flag_nocoordschange;   /* set to true if particle is jumping */
};
Particle _particle_global_randnbuse_var;
Particle* _particle = &_particle_global_randnbuse_var;


// defines


typedef double MCNUM;
typedef struct {MCNUM x, y, z;} Coords;
typedef MCNUM Rotation[3][3];


#define _class_particle Particle
#define randstate_t u64
#define MC_PATHSEP_S "/"
#define MC_PATHSEP_C '/'
#define MCCODE_STRING "tracetool_string"
#define MCCODE_NAME "tracetool_name"


// globals


int  defaultmain         = 1;
int  traceenabled        = 0;
char instrument_name[200];
char instrument_source[200];
char *instrument_exe = NULL;
int  numipar             = 0;
FILE *siminfo_file        = NULL;


int mcncount;
int mcrun_num;
int mcseed;


#endif
