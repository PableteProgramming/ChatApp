#ifdef __linux__
void RunWindow(int);
#else
void RunWindow(SOCKET);
#endif

const int scale = 200;

void Client_Ctrl_Handler(int);