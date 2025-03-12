

const int X_PIN = A2;
const int Y_PIN = A3;
const int Z_PIN = A4;

const int X_ZERO = 1000; // Adjust based on your sensor
const int Y_ZERO = 1000; // Adjust based on your sensor
const int Z_ZERO = 1100; // Adjust based on your sensor

const int WINDOW_SIZE = 5; // Size of the moving average window
int ax_buffer[WINDOW_SIZE] = {0};
int ay_buffer[WINDOW_SIZE] = {0};
int az_buffer[WINDOW_SIZE] = {0};
int buffer_index = 0;

void setupAccelSensor() {
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  Serial.begin(115200); // Initialize Serial for debugging

}

void readAccelSensor() {
  ax = analogRead(X_PIN); 
  ay = analogRead(Y_PIN);
  az = analogRead(Z_PIN);

  // Debug: Print raw accelerometer values
  Serial.print("Raw - ax: ");
  Serial.print(ax);
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print(" az: ");
  Serial.println(az);

  // Add the new readings to the buffer
  ax_buffer[buffer_index] = ax;
  ay_buffer[buffer_index] = ay;
  az_buffer[buffer_index] = az;
  buffer_index = (buffer_index + 1) % WINDOW_SIZE;
}

void smoothAccelData() {
  int ax_sum = 0, ay_sum = 0, az_sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    ax_sum += ax_buffer[i];
    ay_sum += ay_buffer[i];
    az_sum += az_buffer[i];
  }
  ax = ax_sum / WINDOW_SIZE;
  ay = ay_sum / WINDOW_SIZE;
  az = az_sum / WINDOW_SIZE;

  // Debug: Print smoothed accelerometer values
  Serial.print("Smoothed - ax: ");
  Serial.print(ax);
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print(" az: ");
  Serial.println(az);
}

int getOrientation() {
  int orientation = 0;

  // Subtract out the zeros
  int x = ax - X_ZERO;
  int y = ay - Y_ZERO;
  int z = az - Z_ZERO;

  // Debug: Print zero-adjusted values
  Serial.print("Zero-Adjusted - x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.println(z);

  // If ax has the biggest magnitude, it's either left or right
  if (abs(x) >= abs(y) && abs(x) >= abs(z)) {
    if (x < -30) // left (adjust threshold as needed)
      orientation = 3;
    else if (x > 30) // right (adjust threshold as needed)
      orientation = 4;
  }
  // If ay has the biggest magnitude, it's either up or down
  else if (abs(y) >= abs(x) && abs(y) >= abs(z)) {
    if (y < -100) // up (adjust threshold as needed)
      orientation = 1;
    else if (y > 100) // down (adjust threshold as needed)
      orientation = 2;
  }
  // If az has the biggest magnitude, it's flat (or upside-down)
  else if (abs(z) > abs(x) && abs(z) >= abs(y)) {
    orientation = 0; // flat
  }

  // Debug: Print orientation
  Serial.print("Orientation: ");
  Serial.println(orientation);

  return orientation;
}