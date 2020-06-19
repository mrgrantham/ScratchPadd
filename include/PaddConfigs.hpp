#pragma once

#include <tuple>

class GL2;
class GL4;

template <typename T>
class DisplayPadd;
class StoryPadd;
class AudioPadd;

typedef std::tuple<DisplayPadd<GL4>*,StoryPadd*,AudioPadd*> SCRATCHPADD_GL4;
typedef std::tuple<DisplayPadd<GL2>*,StoryPadd*,AudioPadd*> SCRATCHPASS_GL21;