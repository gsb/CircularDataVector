//-----------------------------------------------last------------------------------
// Board:  NodeMCU 1.0 (ESP-12E Module)

#include <Arduino.h>
#include <vector>

class CircularVector {
  public:
    std::vector<int>data;
    int first, last, sum, count;
    CircularVector(int k) {
      data = std::vector<int>(k);
      first = last = sum = count = 0;
    }
    int getFirst() {return (count ? data[first] : -1);}
    int getLast() {return (count ? data[(first+count-1)%data.size()] : -1);}
    float getSmoothed() {return (count ? (float)sum/(float)count : 0.0);}
    void addValue(int value) {
      sum += value;
      if (count < data.size()) { // vector is still filling up
        data[count] = value;
        last = count;
        ++count;
      } else { // list is full ... so,
        sum -= data[first];  // use before overwriting
        data[first] = value; // and overwrite
        first = (++first)%data.size();
        last = first ? first-1 : data.size()-1;
      }
    }
    void showData(int order=0) { // show ordered list, first to last OR as array
      for (int k=0, i=first; k < data.size(); k++) {
        int j = (first+k)%data.size();
        if (order) Serial.printf(" [%d] = %d\n", j, data[j]); // oldest-newest
        else Serial.printf(" [%d] = %d\n", k, data[k]); // vector list 0-n
      }
    }
};
//-----------------------------------------------------------------------------

void setup() {

  Serial.begin(115200);

  Serial.println("\n\nstart\n");


  CircularVector* obj = new CircularVector( 7 );

  obj->addValue(10);
  obj->addValue(11);
  obj->addValue(12);
  obj->addValue(13);
  obj->addValue(14);
  obj->addValue(15);
  obj->addValue(16);
  obj->addValue(17);
  obj->addValue(18);
  obj->addValue(19);

  // show current data vector summery elements from 0 to size()
  Serial.printf("\nfirst: %d-%d, last: %d-%d, size: %d, sum: %d, smoothed: %.2f\n", 
    obj->first, obj->data[obj->first], obj->last, obj->data[obj->last], obj->count, obj->sum, obj->getSmoothed());
  obj->showData();

  // show current data vector summery elements from oldest to newest
  Serial.printf("\nfirst: %d-%d, last: %d-%d, size: %d, sum: %d, smoothed: %.2f\n", 
    obj->first, obj->data[obj->first], obj->last, obj->data[obj->last], obj->count, obj->sum, obj->getSmoothed());
  obj->showData(1);

  Serial.println("\ndone\n");
}

void loop() { }

//-----------------------------------------------------------------------------