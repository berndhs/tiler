#ifndef TILER_MATH_H
#define TILER_MATH_H

#include <cmath>

namespace tiler
{

const double epsilon (0.00001);

bool almostZero (double d)
  {
    return (-epsilon < d && d < epsilon);
  }

}

#endif
