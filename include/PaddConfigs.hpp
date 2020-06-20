#pragma once

#include <tuple>

class GL2;
class GL4;

template <typename T>
class DisplayPadd;
class StoryPadd;
class AudioPadd;


// #define PADD_MAKER(...) typedef std::tuple<\
// typedef std::tuple<DisplayPadd<GL4>*,StoryPadd*,AudioPadd*> SCRATCHPADD_GL4;
// typedef std::tuple<DisplayPadd<GL2>*,StoryPadd*,AudioPadd*> SCRATCHPASS_GL21;

#define SCRATCHPADD_GL4 DisplayPadd<GL4>,StoryPadd,AudioPadd
#define SCRATCHPASS_GL21 DisplayPadd<GL2>,StoryPadd,AudioPadd

// #define SCRATCHPADD_GL4 PADD_MAKER(DisplayPadd<GL4>,StoryPadd,AudioPadd)