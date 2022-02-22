

#include <sys/siginfo.h>

#define MESS_Size 30
#define P1 "P1_Process"
#define P2 "P2_Process"
#define P3 "P3_Process"
#define P4 "P4_Process"
#define P5 "P5_Process"
#define P6 "P6_Process"



struct DataArea {

   pid_t pid[7];

};

#define DATA_AREA_SIZE sizeof(struct DataArea)
