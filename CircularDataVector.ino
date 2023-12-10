//-----------------------------------------------------------------------------
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

void setup() {

  Serial.begin(115200);

  Serial.println("\n\nDemo start.");

  // Create a data list for saving only the last 7-values added
  CircularVector* obj = new CircularVector( 7 );

  // Populate the data list with an incoming sequence of new values over time
  for (int i=0; i < 30; i++) obj->addValue(10 + i); //...sensor readings perhaps

  // show current data vector summery and elements from 0 to size()
  Serial.printf("\nSummary - vector order:\nfirst: %d-%d, last: %d-%d, size: %d, sum: %d, smoothed: %.2f\n", 
    obj->first, obj->data[obj->first], obj->last, obj->data[obj->last], obj->count, obj->sum, obj->getSmoothed());
  obj->showData();

  // show current data vector summery and elements from oldest to newest
  Serial.printf("\nSummary - data sequence order:\nfirst: %d-%d, last: %d-%d, size: %d, sum: %d, smoothed: %.2f\n", 
    obj->first, obj->data[obj->first], obj->last, obj->data[obj->last], obj->count, obj->sum, obj->getSmoothed());
  obj->showData(1);

  Serial.println("\nDemo done.\n");
}

void loop() { }

//-----------------------------------------------------------------------------

/* Demo Output

Demo start.

Summary - vector order:
first: 2-33, last: 1-39, size: 7, sum: 252, smoothed: 36.00
 [0] = 38
 [1] = 39
 [2] = 33
 [3] = 34
 [4] = 35
 [5] = 36
 [6] = 37

Summary - data sequence order:
first: 2-33, last: 1-39, size: 7, sum: 252, smoothed: 36.00
 [2] = 33
 [3] = 34
 [4] = 35
 [5] = 36
 [6] = 37
 [0] = 38
 [1] = 39

Demo done.
*/
