#define BRICKWARE_CORE_EXPORTS

#include "BrickwareCore/Screen.hpp"

using namespace Brickware;
using namespace Core;

int Screen::width;
int Screen::height;

int Screen::getWidth(){ return width; }
int Screen::getHeight(){ return height; }
float Screen::getAspectRatio(){ return ((float)width/(float)height); }
