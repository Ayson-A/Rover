#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MCP23X17.h>
#include <ESP32Servo.h>

Adafruit_MCP23X17 mcp;

WebServer server(80); //creates an HTTP server connected to port 80
IPAddress local_IP(192,168,4,2); //sets up main esp32 IP address
IPAddress gateway(192,168,4,1); //sets up where the main esp32 sends its data to which is the esp32cam IP address
IPAddress subnet(255,255,255,0); //needed for wifi config

Servo PanServo;
const int servoPin1 = 16;

Servo BaseServo;
const int servoPin2 = 13;

Servo ShoulderServo;
const int servoPin3 = 19;

Servo ElbowServo;
const int servoPin4 = 18;

Servo GripperServo;
const int servoPin5 = 17;

void forward();
void backward();
void left();
void right();
void stop();
void handleServo();
void handleMove();

void setup() {
  Serial.begin(115200);
  
  //initializing the mcp23017
  Wire.begin();
  if (!mcp.begin_I2C(0x20)) {
    while (1) delay(10);
  }

  //WiFi setup for the main esp32
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin("ESP32-CAM Robot", "1234567890");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("Min ESP32 IP:");
  Serial.println(WiFi.localIP());

  //sets URLs for servo and motor functions
  server.on("/servo", HTTP_GET, handleServo);
  server.on("/move", HTTP_GET, handleMove);

  //makes server start listening to HTTP requests
  server.begin();

  //initializing the aout, bout of the motor controllers
  mcp.pinMode(15, OUTPUT); //left motor
  mcp.pinMode(14, OUTPUT);

  mcp.pinMode(13, OUTPUT); //left motor
  mcp.pinMode(12, OUTPUT);

  mcp.pinMode(11, OUTPUT); //left motor
  mcp.pinMode(10, OUTPUT);

  mcp.pinMode(9, OUTPUT); //right motor
  mcp.pinMode(8, OUTPUT);

  mcp.pinMode(0, OUTPUT); //right motor
  mcp.pinMode(1, OUTPUT);

  mcp.pinMode(2, OUTPUT); //right motor
  mcp.pinMode(3, OUTPUT);

  //initializing the pwm pins for the motors
  pinMode(32, OUTPUT); //left motor
  pinMode(33, OUTPUT); //left motor
  pinMode(25, OUTPUT); //left motor
  pinMode(26, OUTPUT); //right motor
  pinMode(27, OUTPUT); //right motor
  pinMode(14, OUTPUT); //right motor

  //configure PWM channels for speed control
  ledcAttachPin(32, 0); //left motor
  ledcSetup(0, 1000, 8);
  
  ledcAttachPin(33, 1); //left motor
  ledcSetup(1, 1000, 8);

  ledcAttachPin(25, 2); //left motor
  ledcSetup(2, 1000, 8);

  ledcAttachPin(26, 3); //right motor
  ledcSetup(3, 1000, 8);

  ledcAttachPin(27, 4); //right motor
  ledcSetup(4, 1000, 8);

  ledcAttachPin(14, 5); //right motor
  ledcSetup(5, 1000, 8);

  //putting the rover motors on low in startup
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  ledcWrite(4, 0);
  ledcWrite(5, 0);

// Setup Servo  
  ESP32PWM::allocateTimer(0); // Use timer 0 for servo
  
  PanServo.setPeriodHertz(50); // Standard 50Hz servo
  PanServo.attach(servoPin1, 1000, 2000);

  BaseServo.setPeriodHertz(50); // Standard 50Hz servo
  BaseServo.attach(servoPin2, 1000, 2000);

  ShoulderServo.setPeriodHertz(50); // Standard 50Hz servo
  ShoulderServo.attach(servoPin3, 1000, 2000);

  ElbowServo.setPeriodHertz(50); // Standard 50Hz servo
  ElbowServo.attach(servoPin4, 1000, 2000);

  GripperServo.setPeriodHertz(50); // Standard 50Hz servo
  GripperServo.attach(servoPin5, 1000, 2000);
}

void loop() {
  server.handleClient();
  
}

void forward() {
  mcp.digitalWrite(15, HIGH);
  mcp.digitalWrite(14, LOW);

  mcp.digitalWrite(13, HIGH);
  mcp.digitalWrite(12, LOW);

  mcp.digitalWrite(11, HIGH);
  mcp.digitalWrite(10, LOW);

  mcp.digitalWrite(9, HIGH);
  mcp.digitalWrite(8, LOW);

  mcp.digitalWrite(0, HIGH);
  mcp.digitalWrite(1, LOW);

  mcp.digitalWrite(2, HIGH);
  mcp.digitalWrite(3, LOW);

  ledcWrite(0, 255);
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  ledcWrite(3, 255);
  ledcWrite(4, 255);
  ledcWrite(5, 255);
}

void backward() {
  mcp.digitalWrite(15, LOW);
  mcp.digitalWrite(14, HIGH);

  mcp.digitalWrite(13, LOW);
  mcp.digitalWrite(12, HIGH);

  mcp.digitalWrite(11, LOW);
  mcp.digitalWrite(10, HIGH);

  mcp.digitalWrite(9, LOW);
  mcp.digitalWrite(8, HIGH);

  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, HIGH);

  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(3, HIGH);

  ledcWrite(0, 255);
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  ledcWrite(3, 255);
  ledcWrite(4, 255);
  ledcWrite(5, 255);
}

void stop() {
  mcp.digitalWrite(15, LOW);
  mcp.digitalWrite(14, LOW);

  mcp.digitalWrite(13, LOW);
  mcp.digitalWrite(12, LOW);

  mcp.digitalWrite(11, LOW);
  mcp.digitalWrite(10, LOW);

  mcp.digitalWrite(9, LOW);
  mcp.digitalWrite(8, LOW);

  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, LOW);

  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(3, LOW);

  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  ledcWrite(4, 0);
  ledcWrite(5, 0);
}

void right() {
  mcp.digitalWrite(15, HIGH);
  mcp.digitalWrite(14, LOW);

  mcp.digitalWrite(13, HIGH);
  mcp.digitalWrite(12, LOW);

  mcp.digitalWrite(11, HIGH);
  mcp.digitalWrite(10, LOW);

  mcp.digitalWrite(9, LOW);
  mcp.digitalWrite(8, HIGH);

  mcp.digitalWrite(0, LOW);
  mcp.digitalWrite(1, HIGH);

  mcp.digitalWrite(2, LOW);
  mcp.digitalWrite(3, HIGH);

  ledcWrite(0, 255);
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  ledcWrite(3, 255);
  ledcWrite(4, 255);
  ledcWrite(5, 255);
}

void left() {
  mcp.digitalWrite(15, LOW);
  mcp.digitalWrite(14, HIGH);

  mcp.digitalWrite(13, LOW);
  mcp.digitalWrite(12, HIGH);

  mcp.digitalWrite(11, LOW);
  mcp.digitalWrite(10, HIGH);

  mcp.digitalWrite(9, HIGH);
  mcp.digitalWrite(8, LOW);

  mcp.digitalWrite(0, HIGH);
  mcp.digitalWrite(1, LOW);

  mcp.digitalWrite(2, HIGH);
  mcp.digitalWrite(3, LOW);

  ledcWrite(0, 255);
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  ledcWrite(3, 255);
  ledcWrite(4, 255);
  ledcWrite(5, 255);
}

void handleServo() {
  if (!server.hasArg("joint") || !server.hasArg("value")) {
    server.send(400, "text/plain", "Missing joint or value");
    return;
  }
  String joint = server.arg("joint");
  int angle = server.arg("value").toInt();
  angle = constrain(angle, 0, 180);

  if (joint == "pan") {
    PanServo.write(angle);
  } else if (joint == "base") {
    BaseServo.write(angle);
  } else if (joint == "shoulder") {
    ShoulderServo.write(angle);
  } else if (joint == "elbow") {
    ElbowServo.write(angle);
  } else if (joint == "gripper") {
    GripperServo.write(angle);
  } else{
    server.send(404, "text/plain", "Unknown joint");
    return;
  }
  
  server.send(200, "text/plain", "OK");
}

void handleMove() {
  if (!server.hasArg("direction")) {
    server.send(400, "text/plain", "Missing direction");
    return;
  }

  String d = server.arg("direction");

  if (d == "forward") {
    forward();
  } else if (d == "reverse") {
    backward();
  } else if (d == "left") {
    left();
  } else if (d == "right") {
    right();
  } else {
    stop();
  }

  server.send(200, "text/plain", "Moved");
}





