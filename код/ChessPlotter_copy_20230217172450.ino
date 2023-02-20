#include <GyverStepper.h>
#include "const.h"
#include "funcs.h"
#include "Plotter.h"
#include <ArduinoQueue.h>
struct scommand{
  byte c = 0;
  int c1 = 0;
  int c2 = 0;
};

scommand buf;
scommand buff;

Plotter p;

int command = 0;
int command2 = 0;
byte command_type = 0;
ArduinoQueue<scommand> commands;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2);
  p.init_plotter();
  p.move_to_corner();
}

void loop() {
  p.update_m();
  update_commands();
  process_commands();
}

void process_commands(){
  if (!p.is_moving() && !commands.isEmpty()){
    buff = commands.dequeue();
    command_type = buff.c;
    command = buff.c1;
    command2 = buff.c2;
  }
  p.process_command(command_type, command, command2);
}

void update_commands(){
  if (Serial.available()){
    buf.c = Serial.peek();
    if (buf.c != 32){
      buf.c1 = Serial.parseInt();
      buf.c2 = Serial.parseInt();
      //Serial.println(command_type);
      commands.enqueue(buf);
    }
    buf = scommand();
  }
}


