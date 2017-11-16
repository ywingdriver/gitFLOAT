#include <Arduino.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "Util.h"

String parseVector(std::vector<float> v) {
    String listString = "[";
    for(std::vector<int>::size_type i = 0; i != v.size(); i++) {
      if (i != 0) {
        listString += ",";
      }

      listString += String(v[i]);
    }

    listString += "]";
    return listString;
}

String parseVectorInt(std::vector<int> v) {
    String listString = "[";
    for(std::vector<int>::size_type i = 0; i != v.size(); i++) {
      if (i != 0) {
        listString += ",";
      }

      listString += String(v[i]);
    }

    listString += "]";
    return listString;
}
