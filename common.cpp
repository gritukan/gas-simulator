#include "common.h"

std::string PrintTime(int time)
{
    int days = time / Day;
    time %= Day;

    int hours = time / Hour;
    time %= Hour;

    int minutes = time;
    return std::to_string(days) + " days, " + std::to_string(hours) + " hours, " + std::to_string(minutes) + " minutes";
}
