//
// Created by DingYong on 2026/4/25.
//

#pragma once
#include <cstdio>

class Log_Y
{
public:
    Log_Y();

    // open the file and set the max written lines
    void open(const char* filename,int max_lines);
    // writen the lines, and auto add the timestamp
    void write(float gx,float gy,float gz);
    // close the file
    void close();
    // check if write full max_lines
    bool isFull() const;

private:
    // extract system running time as the timestamp
    float getSystemTimeMs();

private:
    bool _is_opened;
    int _max_lines;
    int _current_line;
    FILE* _file;
};
