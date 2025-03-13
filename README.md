# ECE 16 Grand Challenge - Space Invaders Controller

A socket-based controller for Space Invaders to controller the game wirelessly with the ESP32.

The game is a modified version of Space Invaders based off of this project: https://github.com/leerob/Space_Invaders

You must have Pygame installed before being able to run the game: https://www.pygame.org/


# Part 1 README.md 

[video of controller](https://youtu.be/R6F89TPE4mo) 

## Implementations of 3 improvements and 3 features

For the 3 features I chose 

- Updating the OLED with the current score and the number of remaining lives
- Top Scores: Updating the OLED with the top 3 scores after the game ends
- Sensitivity Settings: By pushing the button I could cycle through faster/ slower movements with 1 being the slowest and 3 being the fastest.

For the 3 Improvements

- Smoothing of the accelerometer values for better movements
- Button firing
- Rapid firing

First, to implement the features I created a new socket to send information on which made it easier to manipulate the information sent on that socket. The controller received that infomation on the new socket and I used it to update the display values on the OLED through arduino. To set up the new socket I just had to initialize it in the game code and the controller so that it could send and receive information. On arduino I would write a command such as if(command.startsWith("Score:", etc.)) it would call a function in Display.ino and update the score with its appropriately receieved value and wasn't too hard to implement. The challenge was understanding how to create the new socket properly and where to send information from the game code. 

Second, to implement the improvements I used a moving average filter to smooth the values coming from the acceelerometer and applied that to the getOrientation() function to get more accurate data from the accelerometer. In arduino, I had to make sure that the sensitivity was accurate with the movement so I used an if statement along with a switch statement to make sure the movement and sensitivity worked hand-in-hand. To implement button firing I used the photodetector. In the Photodetector I applied a low-pass-filter (moving average) for data and applied a threshold that I could print out the values for and calculate the threshold in which the button was pressed and not pressed (isCovered) function. The difficulty in these implementations were in figuring the appropriate values for accelerometer movement and correct values for the photodetector "button" press. 

## Using the Controller 
Although the controller isn't perfect, to use the controller you would hold it horizontally and apply a slight tilt upwards for more accurate movement. You would need to implement a button, OLED, photodetector, accelerometer, to your adafeather. For firing you would tap on the photosensor, for movement you would tilt the board left and right, for sensitivity you would cycle through the button with 1 being the lowest and 3 being the highest, and for rapid firing just tap as fast as you can/ hold your finger on the accelerometer . 

## Collaborators 

Anthony Huynh: A18489020

Aaron Chakma: A18088368

To divide the work we did
Part 1: Anthony Huynh 
Part 2: Aaron Chakma 

I did the entire first part and Aaron did the entire second part. 
