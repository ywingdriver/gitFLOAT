#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <Arduino.h>
#include <sstream>

String parseVector(std::vector<float> v);
String parseVectorInt(std::vector<int> v);

#endif
