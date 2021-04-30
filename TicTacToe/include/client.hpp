#include "includes.h"
#ifdef __linux__
void RunWindow(int);
#else
void RunWindow(SOCKET);
#endif