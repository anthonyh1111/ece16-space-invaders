// SpaceInvadersController.ino
// Function prototypes
void setupAccelSensor();
void readAccelSensor();
void smoothAccelData();
int getOrientation();

void setupDisplay();
void writeDisplay(const char *message, int row, bool clearRow);
void displayScoreAndLives(int score, int lives);
void displayTopScores(int topScores[3]);
void displaySensitivity(int sensitivity);

void setupCommunication();
String receiveMessage();
void sendMessage(String message);

void setupPhotoSensor();

int ax = 0, ay = 0, az = 0;
int ppg = 0;
int sampleTime = 0;
bool sending = false;

int sensitivity = 1;
const int BUTTON_PIN = 12;

int topScores[3] = {0, 0, 0};

void setup() {
  setupAccelSensor(); // Initialize the accelerometer
  setupCommunication();
  setupDisplay();
  setupPhotoSensor();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  sending = false;

  displaySensitivity(sensitivity);
  displayTopScores(topScores);
  displayScoreAndLives(0, 3); // Initial display
}

void loop() {

  // Check if the button is pressed to cycle sensitivity settings
  if (digitalRead(BUTTON_PIN) == LOW) {
    sensitivity = (sensitivity % 3) + 1;
    delay(500); // Debounce delay
    displaySensitivity(sensitivity);
  }

  // Parse command coming from Python (either "stop" or "start")
  String command = receiveMessage();
  if (command == "stop") {
    sending = false;
    writeDisplay("Ctrl: Off", 3, true);
  } else if (command == "start") {
    sending = true;
    writeDisplay("Ctrl: On", 3, true);
  }

  // Read the accelerometer data
  readAccelSensor(); // Call this to update ax, ay, az
  smoothAccelData(); // Apply moving average filter

  // Send the orientation of the board
  if (sending) {
    int orientation = getOrientation();

    // Debug: Print orientation and sensitivity
  // Debug: Print orientation and sensitivity
    Serial.print("Sensitivity: ");
    Serial.println(sensitivity);
    Serial.print("Sending Orientation: ");
    Serial.println(orientation);

  // Send the orientation to the Python controller
  Serial.println(orientation); // Send the orientation as a string
    // Adjust movement based on sensitivity
    if (orientation == 3 || orientation == 4) {
      if (sensitivity == 1 && millis() % 300 == 0) { // Low sensitivity
        sendMessage(String(orientation));
      } else if (sensitivity == 2) { // Medium sensitivity
        sendMessage(String(orientation));
      } else if (sensitivity == 3) { // High sensitivity
        sendMessage(String(orientation));
      }
    } else {
      sendMessage(String(orientation));
    }
  

  // Example: Update score and lives (replace with actual game data)
  static int score = 0;
  static int lives = 3;
  displayScoreAndLives(score, lives);
  delay(100); // Add a small delay to prevent rapid updates
}