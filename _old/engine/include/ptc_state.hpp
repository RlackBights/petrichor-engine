#ifndef PTC_STATE_HPP
#define PTC_STATE_HPP

class State
{
public:
    static int argumentCount;
    static char** arguments;

    static void SetArguments(int _argc, char** _argv);
};

#endif