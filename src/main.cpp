#include <Arduino.h>
#include <AccelStepper.h>
#include <OneButton.h>


const long SMALL_STEP = 10;

const long TicksFastMove = 2000;
const long TicksAcc = 4000;
const float FastSpeed = 3000;

const float SPEED = 1000;
const long MAX_SPEED = 5000;
const long ACCELERATION = 1500;
const long LONG_PRESS_T = 400;

bool stepper_l_stopping = false;
bool stepper_r_stopping = false;

AccelStepper stepper_l(AccelStepper::DRIVER, 2, 3); 
AccelStepper stepper_r(AccelStepper::DRIVER, 5, 6); 

OneButton btn_ll(8, true, true);
OneButton btn_lr(9, true, true);
OneButton btn_rl(10, true, true);
OneButton btn_rr(11, true, true);


void btn_ll_click();
void btn_lr_click();
void btn_rl_click();
void btn_rr_click();


void btn_ll_long_press_start();
void btn_lr_long_press_start();
void btn_rl_long_press_start();
void btn_rr_long_press_start();


void btn_ll_long_press_stop();
void btn_lr_long_press_stop();
void btn_rl_long_press_stop();
void btn_rr_long_press_stop();


void btn_ll_long_press_press() { stepper_l.runSpeed(); }
void btn_lr_long_press_press() { stepper_l.runSpeed(); }
void btn_rl_long_press_press() { stepper_r.runSpeed(); }
void btn_rr_long_press_press() { stepper_r.runSpeed(); }



void setup() {

  Serial.begin(9600);
  Serial.println("start");

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  stepper_l.setEnablePin(4);
  stepper_l.setPinsInverted(true, true, false);
  stepper_l.setMaxSpeed(MAX_SPEED);
  stepper_l.setAcceleration(ACCELERATION);
  

  stepper_r.setEnablePin(7);
  stepper_r.setPinsInverted(true, true, false);
  stepper_r.setMaxSpeed(MAX_SPEED);
  stepper_r.setAcceleration(ACCELERATION);
  
  btn_ll.setPressTicks(LONG_PRESS_T);
  btn_lr.setPressTicks(LONG_PRESS_T);
  btn_rl.setPressTicks(LONG_PRESS_T);
  btn_rr.setPressTicks(LONG_PRESS_T);

  btn_ll.attachClick(&btn_ll_click);
  btn_lr.attachClick(&btn_lr_click);
  btn_rl.attachClick(&btn_rl_click);  
  btn_rr.attachClick(&btn_rr_click);

  btn_ll.attachLongPressStart(&btn_ll_long_press_start);
  btn_lr.attachLongPressStart(&btn_lr_long_press_start);
  btn_rl.attachLongPressStart(&btn_rl_long_press_start);
  btn_rr.attachLongPressStart(&btn_rr_long_press_start);


  btn_ll.attachLongPressStop(&btn_ll_long_press_stop);
  btn_lr.attachLongPressStop(&btn_lr_long_press_stop);
  btn_rl.attachLongPressStop(&btn_rl_long_press_stop);
  btn_rr.attachLongPressStop(&btn_rr_long_press_stop);

  btn_ll.attachDuringLongPress(&btn_ll_long_press_press);
  btn_lr.attachDuringLongPress(&btn_lr_long_press_press);
  btn_rl.attachDuringLongPress(&btn_rl_long_press_press);
  btn_rr.attachDuringLongPress(&btn_rr_long_press_press);
}

void stepper_tick(AccelStepper *stepper, bool *breaking_flag) {
  if(*breaking_flag){
    stepper->runSpeed();

    if(stepper->speed() == 0.0) {
      *breaking_flag = false;
      stepper->moveTo(stepper->currentPosition()); // Target position reset.
    }
  } else {
    stepper->run();
  }
}

void set_stepper_speed(AccelStepper *stepper, float speed){
    if(stepper->isRunning()) stepper->stop();
    stepper->setSpeed(speed);
}

void loop() {

  if(!btn_ll.isLongPressed() && !btn_lr.isLongPressed()) {
    stepper_tick(&stepper_l, &stepper_l_stopping);
  }

  if(!btn_rl.isLongPressed() && !btn_rr.isLongPressed()) {
    stepper_tick(&stepper_r, &stepper_r_stopping);
  }

  //if ((btn_rl.getPressedTicks()>TicksFastMove)  && (btn_rl.getPressedTicks()<TicksAcc) && (stepper_r_stopping==false)){
  //  set_stepper_speed(&stepper_r, -(btn_rl.getPressedTicks()-TicksFastMove)/(TicksAcc-btn_rl.getPressedTicks())*FastSpeed);
  //}



  btn_ll.tick();
  btn_lr.tick();
  btn_rl.tick();
  btn_rr.tick();
}

void stepper_small_step(AccelStepper *stepper, long step) {
  stepper->moveTo(stepper->targetPosition() + step);
}

void btn_ll_click() { stepper_small_step(&stepper_l, -SMALL_STEP);}
void btn_lr_click() { stepper_small_step(&stepper_l, SMALL_STEP);}

void btn_rl_click() { stepper_small_step(&stepper_r, -SMALL_STEP);}
void btn_rr_click() { stepper_small_step(&stepper_r, SMALL_STEP);}

void btn_ll_long_press_start() { set_stepper_speed(&stepper_l, -SPEED); }
void btn_lr_long_press_start(){ set_stepper_speed(&stepper_l, SPEED); }

void btn_rl_long_press_start(){ set_stepper_speed(&stepper_r, -SPEED); }
void btn_rr_long_press_start(){ set_stepper_speed(&stepper_r, SPEED); }


void stepper_l_stop() {
  stepper_l.setSpeed(0);
  stepper_l_stopping = true;
}

void btn_ll_long_press_stop() { stepper_l_stop(); }
void btn_lr_long_press_stop() { stepper_l_stop(); }


void stepper_r_stop() {
  stepper_r.setSpeed(0);
  stepper_r_stopping = true;
}

void btn_rl_long_press_stop(){ stepper_r_stop();}
void btn_rr_long_press_stop(){ stepper_r_stop();}
