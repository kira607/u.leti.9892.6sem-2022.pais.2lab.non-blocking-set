#ifndef INC_LOG_H
#define INC_LOG_H

#include <iostream>

#define LOG(msg) {std::cout << msg;} while(0)
#define LOGLN(msg) {LOG(msg << '\n');} while(0)

#ifdef DBG
#define DEBUG(msg) {LOG(msg);} while(0)
#define DEBUGLN(msg) {LOGLN(msg);} while(0)
#else
#define DEBUG(msg) {;} while(0)
#define DEBUGLN(msg) {;} while(0)
#endif

#endif //INC_LOG_H
