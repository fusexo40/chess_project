GStepper<STEPPER2WIRE> stepperx(SPT, M1_D, M1_S);
GStepper<STEPPER2WIRE> steppery(SPT, M2_D, M2_S);
struct Plotter{
  bool is_moving_to_corner = false;
  void init_plotter(){
    pinMode(M2_MIN_PIN, INPUT_PULLUP);
    pinMode(M2_MAX_PIN, INPUT_PULLUP);
    pinMode(M1_MIN_PIN, INPUT_PULLUP);
    pinMode(M1_MAX_PIN, INPUT_PULLUP);

    stepperx.setRunMode(FOLLOW_POS); // режим поддержания скорости
    stepperx.setMaxSpeedDeg(6000);
    stepperx.setSpeedDeg(3000);         // в градусах/сек
    stepperx.setAccelerationDeg(500);

    steppery.setRunMode(FOLLOW_POS); // режим поддержания скорости
    steppery.setMaxSpeedDeg(6000);
    steppery.setSpeedDeg(3000);         // в градусах/сек
    steppery.setAccelerationDeg(500);

    stepperx.setCurrent(0);
    steppery.setCurrent(0);
  }
  void move_to_corner(){ 
      stepperx.setTarget(-100000);
      steppery.setTarget(-100000);
      is_moving_to_corner = true;
  }

  void process_command(byte& command_type, int& command, int& command2){
    if (command_type == 103){
      if (command == 1){
        Serial.println((int)(stepperx.getTarget() / INP_K));
      }
      if (command == 2){
        Serial.println((int)(stepperx.getCurrent()/ INP_K));
      }
      if (command == 3){
        Serial.println((int)(steppery.getTarget()/ INP_K));
      }
      if (command == 4){
        Serial.println((int)(steppery.getCurrent()/ INP_K));
      }
    }
    if (command_type == 114){
      move_to_corner();
    }
    if (command_type == 115){
      stepperx.setTarget(command * INP_K);
      steppery.setTarget(command2 * INP_K);
    }
    if (command_type == 117){
      stepperx.brake();
      steppery.brake();
    }
    if (command_type == 121){
      stepperx.reset();
      steppery.reset();
    }
    if (command_type == 109){
      int k1 = command > 2;
      int k2 = command % 2;
      stepperx.setTarget(1000000 * k1);
      steppery.setTarget(1000000 * k2);
    }
    if (command_type == 99){
      byte command_type = 115;
      int v1 = BOARD_CORNER.x + SQUARE_SIZE * command;
      int v2 = BOARD_CORNER.y + SQUARE_SIZE * command2;
      process_command(command_type, v1 , v2);
    }
    command_type = 0;
    command = 0;
    command2 = 0;
  }

  bool is_moving(){
    return (stepperx.getState() || steppery.getState());
  }

  void update_m(){
    byte dirx = stepperx.getCurrent() < stepperx.getTarget();
    byte diry = steppery.getCurrent() < steppery.getTarget();
    stepperx.tick();
    steppery.tick();
    
    if (!can_step(dirx, M1_MIN_PIN, M1_MAX_PIN)){
      stepperx.brake();
      if (is_moving_to_corner){
        stepperx.reset();
        stepperx.setTarget(0);
      }
    }
    if (!can_step(diry, M2_MIN_PIN, M2_MAX_PIN)){
      steppery.brake();
      if (is_moving_to_corner){
        steppery.reset();
        steppery.setTarget(0);
      }
    }
    if (is_moving_to_corner && (stepperx.getTarget() == 0) && (steppery.getTarget() == 0)){
      is_moving_to_corner = 0;
    }
  }
};