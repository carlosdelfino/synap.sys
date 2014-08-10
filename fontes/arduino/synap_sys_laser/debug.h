#ifndef DEBUG_H
#define DEBUG_H

// macros para ativacao de depuracao
#define D_L_INIT        B00000001
#define D_L_SERIALCMD   B00000010 
#define D_L_SERVO       B10000000 << 1
#define D_L_LASER       B10000000 << 2

//#define DEBUG         ( D_L_INIT | D_L_SERVO | D_L_LASER | D_L_SERIALCMD)
//#define DEBUG          (D_L_SERIALCMD)
//#define DEBUG          (D_L_POTS)

#ifndef DEBUG
#define DEBUG         (0)
#endif

#define DEBUG_INIT        (D_L_INIT        == (DEBUG & D_L_INIT ))
#define DEBUG_SERVOCMD    (D_L_SERVO       == (DEBUG & D_L_SERVO))
#define DEBUG_SERIALCMD   (D_L_SERIALCMD   == (DEBUG & D_L_SERIALCMD ))

#endif
