#ifndef PTC_STATE_H
#define PTC_STATE_H

class State
{
public:
    static int argumentCount;
    static char** arguments;

    static void SetArguments(int _argc, char** _argv);
};

#endif