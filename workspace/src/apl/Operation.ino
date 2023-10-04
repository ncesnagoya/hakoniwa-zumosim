#if 0
#define MOTOR_SPEED 120
#define TURN_SPEED  120

#define REFLECTANCE_THRESHOLD  400

#define INTERSECTION_OSTIM     350

#define ON_LINE(val)  ((val) > REFLECTANCE_THRESHOLD)
#else
#define MOTOR_SPEED 100
#define TURN_SPEED  320

#define REFLECTANCE_THRESHOLD  450

#define INTERSECTION_OSTIM     350
#endif
#define ON_LINE(val)  (((val) != 0) && ((val) < REFLECTANCE_THRESHOLD))

void doOperation(void) {
  char cmd;
  std::cout << "ccommand=" << ccommand << std::endl;
  cmd = command[ccommand++];

  goStraight();

  //交差点の中心まで進む
  motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
  delay(INTERSECTION_OSTIM);

  //コマンド処理
  if (cmd == '.') {
    //停止コマンド
    motors.setSpeeds(0, 0);
    //buzzer.playNum(1);
    while (1) {
      led.on();
      bool ret = delay(1000);
      if (ret == false) {
        return;
      }
      led.off();
      ret = delay(1000);
      if (ret == false) {
        return;
      }
    };
  }
  else if ((cmd == 'r') || (cmd == 'l')) {
    //左右へのターンコマンド
    doTurn(cmd);
  }
}

//直進
void goStraight(void) {
  motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);

  while (1) {
    reflectances.update();

    if (ON_LINE(reflectances.value(2))) {
      motors.setSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    } else if (ON_LINE(reflectances.value(5))) {
      motors.setSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    } else {
      motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    }

    //横線を見つけたら終了
    if (ON_LINE(reflectances.value(1)) || ON_LINE(reflectances.value(6))) {
      return;
    }
  }
}

//ターン
void doTurn(char dir) {
  int count = 0;
  int pstate;

  reflectances.update();

  switch (dir) {
    case 'r':
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      pstate = reflectances.value(5);
      while (count < 2) {
        reflectances.update();
        if ((ON_LINE(reflectances.value(5)) ^ ON_LINE(pstate))) {
          count++;
        }
        pstate = reflectances.value(5);
      }
      break;
    case 'l':
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      pstate = reflectances.value(2);
      while (count < 2) {
        reflectances.update();
        if ((ON_LINE(reflectances.value(2)) ^ ON_LINE(pstate))) {
          count++;
        }
        pstate = reflectances.value(2);
      }
      break;
  }
}
