/**************************************************************************/
/*!
    Utils for escape room sensors "Egypt"
*/
/**************************************************************************/

#ifndef __EGYPTSENSOR_H__
#define __EGYPTSENSOR_H__

#define LOOP_CYCLE      100
#define BLOCK_NUMBER    4
#define TIMEOUT         4000
#define LED_PIN         2 // internal LED pin of the feather
#define TIMEOUT_TOUCH   150   // ~ in number of LOOP_CYCLE
#define WIN_PIN         1

bool is_master (uint8_t data[16]) {
// if data=="master0000000000"
// return true
// else return false

  uint8_t master[] = { 'm', 'a', 's', 't', 'e', 'r', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (uint8_t i=0; i<16; i++)
  {
    if (data[i]==master[i]){
      if (i==15){
        return true;
      }
    }
    else {
      return false;
    }
  }
}

bool check_id (uint8_t id[7], uint8_t check[7]) {
// if id==check
// return true
// else return false

  for (uint8_t i=0; i<7; i++)
  {
    if (id[i]==check[i]){
      if (i==6){
        return true;
      }
    }
    else {
      return false;
    }
  }
}

bool change_target (uint8_t target[7], uint8_t newTarget[7]) {
// target=newTarget

  for (uint8_t i=0; i<7; i++){
    target[i]=newTarget[i];
  }
}

#endif
