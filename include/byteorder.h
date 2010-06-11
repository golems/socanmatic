
#include <sys/param.h>

#if __BYTE_ORDER == __BIG_ENDIAN

// Host to CAN long
#define htocl(n) ((n & 0xff000000) >> 24) | ((n & 0x00ff0000) >> 8) | ((n & 0x0000ff00) << 8) | ((n & 0x000000ff) << 24)

// Host to CAN short
#define htocs(n) ((n & 0xff00) >> 8) | ((n & 0x00ff) << 8)

// Translations are symmetric
#define ctohl(n) htocl(n)
#define htocs(n) ctohs(n)

#else

// CAN is little-endian by default -- do it inline
#define htocl(n) n
#define htocs(n) n
#define ctohl(n) n
#define ctohs(n) n

#endif
