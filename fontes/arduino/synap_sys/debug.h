#ifndef DEBUG_H
#define DEBUG_H

// macros para ativacao de depuracao
#define D_L_INIT        B00000001
#define D_L_SERIALCMD   B00000010
#define D_L_OUTPUT      B00000100
#define D_L_PAIR1       B00001000
#define D_L_PAIR2       B00010000
#define D_L_INPUT       B00100000
#define D_L_POTS        B10000000

//#define DEBUG         ( D_L_INIT | D_L_OUTPUT | D_L_PAIR | D_L_INPUT)
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR1 | D_L_PAIR2 )
//#define DEBUG         ( D_L_OUTPUT | D_L_PAIR  )
#define DEBUG          (D_L_SERIALCMD)
//#define DEBUG          (D_L_POTS)

#ifndef DEBUG
#define DEBUG         (0)
#endif

#define DEBUG_INIT        (D_L_INIT        == (DEBUG & D_L_INIT ))

#endif
