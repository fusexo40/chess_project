  bool read_switch(byte pin)
  {
    //dual read as crude debounce
    
    if ( SENSORS_INVERTING )
      return !digitalRead(pin) && !digitalRead(pin);
    else
      return digitalRead(pin) && digitalRead(pin);
  }
  bool can_step(byte direction, int min_pin, int max_pin)
  {
    //stop us if we're at home and still going 
    if (read_switch(min_pin) && !direction)
      return false;
    //stop us if we're at max and still going
    else if (read_switch(max_pin) && direction)
      return false;
    //default to being able to step
    return true;
  }

// void go_to_corner(int corner){
//   switch (corner){
//     case 1: 
//       MX.go_to_extreme_position(0);
//       MY.go_to_extreme_position(0);
//       break;
//     case 2: 
//       MX.go_to_extreme_position(0);
//       MY.go_to_extreme_position(1);
//       break;
//     case 3: 
//       MX.go_to_extreme_position(1);
//       MY.go_to_extreme_position(1);
//       break;
//     case 4: 
//       MX.go_to_extreme_position(1);
//       MY.go_to_extreme_position(0);
//       break;
//   }
// }