// game window size
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// error check
#define SOCKET_ERROR -1

// game physics
#define MAX_VELOCITY 10
#define TURN_SPEED 0.12
#define ACCELERATION 0.18
#define DECELERATION 0.2

// misc
#define START "CMD_START"
#define EXIT "P1|CMD_EXIT"
#define START_COORD_OK "R_COORDINATES"

// server movement messages
#define RECV_UP "P1|M_UP"
#define RECV_UP_LT "P1|M_UP_LEFT"
#define RECV_UP_RT "P1|M_UP_RIGHT"
#define RECV_DOWN "P1|M_DOWN"
#define RECV_DOWN_LT "P1|M_DOWN_LEFT"
#define RECV_DOWN_RT "P1|M_DOWN_RIGHT"
#define RECV_LEFT "P1|M_LEFT"
#define RECV_RIGHT "P1|M_RIGHT"

// client movement messages
#define UP "M_UP"
#define UP_RIGHT "M_UP_RIGHT"
#define UP_LEFT "M_UP_LEFT"
#define DOWN "M_DOWN"
#define DOWN_RIGHT "M_DOWN_RIGHT"
#define DOWN_LEFT "M_DOWN_LEFT"
#define RIGHT "M_RIGHT"
#define LEFT "M_LEFT"