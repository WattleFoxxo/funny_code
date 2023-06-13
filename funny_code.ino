#define MOTOR_LEFT_0 9
#define MOTOR_LEFT_1 8
#define MOTOR_RIGHT_0 11
#define MOTOR_RIGHT_1 10

#define LINE_LEFT 3
#define LINE_RIGHT 4

#define WALL_TRIGGER 2

#define WALL_LEFT 7
#define WALL_CENTER 6
#define WALL_RIGHT 5

#define MAX_WIGGLE 20
#define WALL_DISTENCE 15

#define MIN_SPEED 0
#define MAX_SPEED 180

long left_duration;
long center_duration;
long right_duration;
long left_cm;
long center_cm;
long right_cm;

enum Direction {
  Stop,
  Forward,
  Backword,
  Left,
  Right,
};

enum Mode {
  Line,
  Wall,
};

Mode mode = Line;

byte wiggle = MAX_WIGGLE;

void setup() {
  pinMode(MOTOR_LEFT_0, OUTPUT);
  pinMode(MOTOR_LEFT_1, OUTPUT);
  pinMode(MOTOR_RIGHT_0, OUTPUT);
  pinMode(MOTOR_RIGHT_1, OUTPUT);

  pinMode(LINE_LEFT, INPUT);
  pinMode(LINE_RIGHT, INPUT);

  pinMode(WALL_TRIGGER, OUTPUT);

  pinMode(WALL_LEFT, INPUT);
  pinMode(WALL_CENTER, INPUT);
  pinMode(WALL_RIGHT, INPUT);

  Serial.begin(9600);
}

void loop() {
  if (mode == Line) {
    line_mode();
  } else if (mode == Wall) {
    wall_mode();
  }
  //move(Forward);
}

void trigger() {
  delay(200); //delay to stop it being autistic
  digitalWrite(WALL_TRIGGER, false);
  delayMicroseconds(5);
  digitalWrite(WALL_TRIGGER, true);
  delayMicroseconds(10);
  digitalWrite(WALL_TRIGGER, false);
}

void wall_mode() {
  // scan shit
  trigger();
  long left_duration = pulseIn(WALL_LEFT, HIGH);
  trigger();
  long center_duration = pulseIn(WALL_CENTER, HIGH);
  trigger();
  long right_duration = pulseIn(WALL_RIGHT, HIGH);

  long left_cm = (left_duration / 2) / 29.1;
  long center_cm = (center_duration / 2) / 29.1;
  long right_cm = (right_duration / 2) / 29.1;

  Serial.print("left: ");
  Serial.print(left_cm);
  Serial.print(", center: ");
  Serial.print(center_cm);
  Serial.print(", right: ");
  Serial.print(right_cm);
  Serial.print("\n");

  if(left_cm > 15 && center_cm < 15 && right_cm < 15){
    while (true) {
      delay(50);
      move(Stop);
      move(Right);
      if(left_cm > 15 && center_cm < 15 && right_cm < 15){
        break;
      }
    }
    
  } else if(left_cm < 15 && center_cm < 15 && right_cm > 15) {
    while (true) {
      delay(50);
      move(Stop);
      move(Left);
      if(left_cm < 15 && center_cm < 15 && right_cm > 15){
        break;
      }
    }
  } else if(center_cm > 15) {
    move(Forward);
  } else {
    move(Stop);
  }

  delay(400);

  move(Stop);
  
}

void line_mode() {
  if ((digitalRead(LINE_LEFT) != 1) || (digitalRead(LINE_RIGHT) != 1)) {
        wiggle = MAX_WIGGLE;
  }

  if( (digitalRead(LINE_LEFT) == 0) && (digitalRead(LINE_RIGHT) == 0) ){
    move(Forward);
  } else if ( (digitalRead(LINE_LEFT) == 0) && (digitalRead(LINE_RIGHT) == 1) ) {
    move(Left);
  } else if ( (digitalRead(LINE_LEFT) == 1) && (digitalRead(LINE_RIGHT) == 0) ) {
    move(Right);
  } else if ((digitalRead(LINE_LEFT) == 1) && (digitalRead(LINE_RIGHT) == 1)) {
    move(Stop);
    if (wiggle > 0) {
      wiggle -= 1;
      delay(10);
    } else {
      mode = Wall;
    }


  } else {
    move(Stop);
  }
}

void move(Direction dir) {
  switch (dir) {
    case Forward:
      analogWrite(MOTOR_LEFT_0, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_0, MIN_SPEED);
      analogWrite(MOTOR_LEFT_1, MAX_SPEED);
      analogWrite(MOTOR_RIGHT_1, MAX_SPEED);
      break;
    case Backword:
      analogWrite(MOTOR_LEFT_0, MAX_SPEED);
      analogWrite(MOTOR_RIGHT_0, MAX_SPEED);
      analogWrite(MOTOR_LEFT_1, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_1, MIN_SPEED);
      break;
    case Left:
      analogWrite(MOTOR_LEFT_0, MAX_SPEED);
      analogWrite(MOTOR_RIGHT_0, MIN_SPEED);
      analogWrite(MOTOR_LEFT_1, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_1, MAX_SPEED);
      break;
    case Right:
      analogWrite(MOTOR_LEFT_0, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_0, MAX_SPEED);
      analogWrite(MOTOR_LEFT_1, MAX_SPEED);
      analogWrite(MOTOR_RIGHT_1, MIN_SPEED);
      break;
    case Stop:
      analogWrite(MOTOR_LEFT_0, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_0, MIN_SPEED);
      analogWrite(MOTOR_LEFT_1, MIN_SPEED);
      analogWrite(MOTOR_RIGHT_1, MIN_SPEED);
      break;
  }
}
