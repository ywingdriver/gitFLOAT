#include <FS.h>
#include <Memory.h>

void Memory::append(float accel, float temp, float lat, float lon, float speed){

  String lineAppend = (String) accel + "," + (String) temp + "," + String(lat, 6) + ","
              + String(lon, 6) + "," + (String) speed;

  File f = SPIFFS.open("/data.txt", "a");

  if (!f) {
    Serial.println("File open for APPENDING failure");
  } else {
    f.println(lineAppend);
  }
}

void Memory::close(){
  File f = SPIFFS.open("/data.txt", "a");

  if (!f) {
    Serial.println("File open for CLOSING failure");
  } else {
    Serial.print("File size: "); Serial.println(f.size());
    f.close();
  }
}

void Memory::read(){
  File f = SPIFFS.open("/data.txt", "r");

  String s = "";

  if (!f) {
    Serial.println("File open for READING failure");
  } else {
    while(f.position() < f.size()){
      s = f.readStringUntil('\n');
      s.trim();
      Serial.println(s);
      delay(500);
    }
  }
  f.close();
}



// Takes info and puts them in class strings
void Memory::parse(int numOfEntries) {

  File f = SPIFFS.open("/data.txt", "r");

  int numEntries = getNumEntries();
  int everyOther = 1;

  if (numEntries < 200)
  everyOther += numEntries/100 - 1;

  String s = "";
  altData = "[";
  gpsLatData = "[";
  gpsLngData = "[";
  gpsSpeedData = "[";
  tempData = "[";
  fakeTimes = "[";
  fakeTime = millis()/1000;

  if (!f) {
    Serial.println("File open for READING failure");
  }
  else {
    int i = 0;
    int j = 0;
    int first = 1;
    while((f.position() < f.size()) && (i < numOfEntries)){

      if (first != 1 && j == 0) {
        altData += ",";
        gpsLatData += ",";
        gpsLngData += ",";
        gpsSpeedData += ",";
        tempData += ",";
        fakeTimes += ",";
        }
      else {
        first = 0;
      }

        s = f.readStringUntil('\n');
        s.trim();

      if (j == 0) {
        altData += getValue(s, ',', 0);
        tempData += getValue(s, ',', 1);
        gpsLatData += getValue(s, ',', 2);
        latCur = getValue(s, ',', 2);
        gpsLngData += getValue(s, ',', 3);
        lngCur = getValue(s, ',', 3);
        gpsSpeedData += getValue(s, ',', 4);
        fakeTimes += (fakeTime + i);

        i++;
      }

      j++;
      if (j >= everyOther) {
        j = 0;
      }
    }
  }
  altData += "]";
  gpsLatData += "]";
  gpsLngData += "]";
  gpsSpeedData += "]";
  tempData += "]";
  fakeTimes += "]";
  f.close();

}

void Memory::clear(){
  File f = SPIFFS.open("/data.txt", "w");

  if (!f) {
    Serial.println("File open for CLEARING failure");
  } else {
    Serial.println("File clear success");
  }
  f.close();
}

String Memory::getAltData(){
  return altData;
}

String Memory::getGpsLatData(){
  return gpsLatData;
}

String Memory::getGpsLngData(){
  return gpsLngData;
}

String Memory::getGpsSpeedData(){
  return gpsSpeedData;
}

String Memory::getTempData(){
  return tempData;
}

String Memory::getLat(){
  return latCur;
}

String Memory::getLng(){
  return lngCur;
}

String Memory::getFakeTimes(){
  return fakeTimes;
}

void Memory::init(){
  SPIFFS.begin();

  Serial.println("SPIFFS init successful.");
  //clear();
}

// Stack Overflow https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index){
 int found = 0;
 int strIndex[] = {0, -1};
 int maxIndex = data.length()-1;

 for(int i=0; i<=maxIndex && found<=index; i++){
   if(data.charAt(i)==separator || i==maxIndex){
       found++;
       strIndex[0] = strIndex[1]+1;
       strIndex[1] = (i == maxIndex) ? i+1 : i;
   }
 }

 return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int Memory::getNumEntries() {
  File f = SPIFFS.open("/data.txt", "r");

  int numEntries = 0;

  if (!f) {
    Serial.println("File open for READING failure");
  } else {
    while(f.position() < f.size()){
      f.readStringUntil('\n');
      ++numEntries;
    }
  }
  f.close();
  return numEntries;
}
