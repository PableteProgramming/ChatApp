#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <thread>
#include <pthread.h>
const int PORT = 8080;
const std::string add= "127.0.0.1";
