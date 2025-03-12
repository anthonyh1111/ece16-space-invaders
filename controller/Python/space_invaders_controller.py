# space_invaders_controller.py
#Changes and improvements:

#Improvements:
# Smoother movement
#
#

#Features: 
# Top scores 
# OLED Display storing 
# Sensitivity


# space_invaders_controller.py
from ECE16Lib.Communication import Communication
from time import sleep
import socket
import threading

class PygameController:
    def __init__(self, serial_name, baud_rate):
        self.comms = Communication(serial_name, baud_rate)
        self.last_fire_time = 0
        
        # UDP socket configuration
        self.udp_host = "127.0.0.1"
        self.udp_port = 65432
        self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_socket.setblocking(False)
        
        # Game communication address
        self.game_address = (self.udp_host, self.udp_port)

        # Top scores storage
        self.top_scores = [0, 0, 0]
        self.load_top_scores()

    def load_top_scores(self):
        """Load top scores from a file."""
        try:
            with open('top_scores.txt', 'r') as f:
                scores = f.read().split(',')
                self.top_scores = list(map(int, scores))[:3]
        except FileNotFoundError:
            self.top_scores = [0, 0, 0]

    def save_top_scores(self):
        """Save top scores to a file."""
        with open('top_scores.txt', 'w') as f:
            f.write(','.join(map(str, self.top_scores)))

    def update_top_scores(self, new_score):
        """Update the top scores list with a new score."""
        updated = False
        for i in range(3):
            if new_score > self.top_scores[i]:
                self.top_scores.insert(i, new_score)
                self.top_scores = self.top_scores[:3]
                updated = True
                break
        if updated:
            self.save_top_scores()
            self.comms.send_message(f"DATA:TOPSCORES:{','.join(map(str, self.top_scores))}")

    def handle_serial(self):
        """Thread: Read from Arduino and forward commands to the game."""
        while True:
            message = self.comms.receive_message()
            if message.startswith("CMD:"):
                try:
                    # Forward the command to the game
                    self.udp_socket.sendto(message.encode(), self.game_address)
                    print(f"[CONTROLLER] Forwarded command: {message}")
                except Exception as e:
                    print(f"[ERROR] Command send failed: {e}")

    def handle_udp(self):
        """Thread: Read from the game and forward data to the Arduino."""
        while True:
            try:
                data, _ = self.udp_socket.recvfrom(1024)
                message = data.decode('utf-8')
                
                if message.startswith("DATA:SCORE:"):
                    # Forward score updates to Arduino
                    self.comms.send_message(message)
                    print(f"[CONTROLLER] Forwarded score: {message}")
                
                elif message.startswith("DATA:LIVES:"):
                    # Forward lives updates to Arduino
                    self.comms.send_message(message)
                    print(f"[CONTROLLER] Forwarded lives: {message}")
                
                elif message.startswith("DATA:GAMEOVER:"):
                    # Handle game over and update top scores
                    score = int(message.split(':')[2])
                    self.update_top_scores(score)
                    print(f"[CONTROLLER] Game over! Final score: {score}")
                
                elif message.startswith("DATA:TOPSCORES:"):
                    # Forward top scores to Arduino
                    self.comms.send_message(message)
                    print(f"[CONTROLLER] Forwarded top scores: {message}")
                
                else:
                    print(f"[CONTROLLER] Received unknown message: {message}")

            except BlockingIOError:
                pass  # No data available
            except Exception as e:
                print(f"[ERROR] Data handling failed: {e}")

    def run(self):
        """Start the controller and its threads."""
        # Start serial->UDP thread
        serial_thread = threading.Thread(target=self.handle_serial)
        serial_thread.daemon = True
        serial_thread.start()

        # Start UDP->serial thread
        udp_thread = threading.Thread(target=self.handle_udp)
        udp_thread.daemon = True
        udp_thread.start()

        # Keep main thread alive
        try:
            while True:
                sleep(1)
        except KeyboardInterrupt:
            self.shutdown()

    def shutdown(self):
        """Clean up resources and shut down the controller."""
        self.comms.send_message("CMD:QUIT")
        self.udp_socket.close()
        self.comms.close()
        print("\nController shutdown complete")

if __name__ == "__main__":
    serial_name = "COM4"    # Change to your Arduino's port
    baud_rate = 115200
    
    controller = PygameController(serial_name, baud_rate)
    print("Starting controller...")
    
    try:
        controller.run()
    except Exception as e:
        print(f"Critical error: {e}")
        controller.shutdown()