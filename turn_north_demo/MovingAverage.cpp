/*
 * Copyright (c) 2014 Daniel Fowler
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 * associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial 
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MovingAverage_H
#define MovingAverage_H

class MovingAverage {
private:
  const int _windowSize;
  float _currentAverage;
  int _totalValuesAdded;
  float _numerator;
  float _denominator;
public:
  MovingAverage(int);
  ~MovingAverage();
  float add_value(float);
  float get_value();
  bool is_window_full(); 
};

#endif

inline MovingAverage::MovingAverage(int windowSize) 
  :_windowSize(windowSize) 
{
  // constructor  
  _totalValuesAdded = 0;
  _numerator = 0;
  _denominator = 0.0;
}

inline MovingAverage::~MovingAverage() {}

/**
 * Add a new value to the moving average
 */
inline float MovingAverage::add_value(float newValue) {
  if(_denominator < _windowSize) {
    _numerator += newValue;
    _denominator += 1.0;
    _currentAverage = _numerator / _denominator; // simple average
  } else {
    _numerator = (_currentAverage * (_windowSize - 1)) + newValue;
    _currentAverage = _numerator / _windowSize;
  }
  return _currentAverage;
}

inline float MovingAverage::get_value() {
  return _currentAverage;
}

inline bool MovingAverage::is_window_full() {
  return _denominator == _windowSize;
}
