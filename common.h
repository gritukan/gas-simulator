#ifndef COMMON_H
#define COMMON_H

#include <string>

//! Time stuff.
using TTime = int;
constexpr int Minute = 1;
constexpr int Hour = 60;
constexpr int Day = Hour * 24;
constexpr int Week = Day * 7;

//! Prints time in human-readable format.
std::string PrintTime(int time);

#endif // COMMON_H
