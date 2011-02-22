#ifndef TILER_MATH_H
#define TILER_MATH_H

#include <cmath>

namespace tiler
{

const double epsilon (0.00001);

template <class T>
bool AlmostZero (T d)
  {
    return (T(-epsilon) < d && d < T(epsilon));
  }

template <class T> 
int Signum (T value) 
  {
    return (value > T(0)) - (value < T(0));
  }

template <class T> 
bool SameSign (T val1, T val2)
  {
    return Signum (val1) == Signum (val2);
  }

}

#endif
