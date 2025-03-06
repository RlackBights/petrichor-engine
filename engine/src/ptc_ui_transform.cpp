#include "ptc_transform.h"
#include <ptc_ui_transform.h>

UITransform::UITransform(Object* _object, int _width, int _height) : Transform(_object)
{
    this->width = _width;
    this->height = _height;
}