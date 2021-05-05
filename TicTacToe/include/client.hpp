#ifdef __linux__
void RunWindow(int);
#else
void RunWindow(SOCKET);
#endif

const int scale = 100;