# IntroductionToRobotics(2022-2023)
Repository with the homeworks and projects done for the Introduction to Robotics class at the Faculty of Mathematics and Computer Science, University of Bucharest in the 3rd year. We will come with more details soon...

## Homework 1

<img src="https://user-images.githubusercontent.com/79469458/197783519-1b1fdf31-3a36-4771-b979-5c1fbf3f4b04.jpeg" align="left" width="300" height="300">

This project consists in controling an RGB LED using 3 potentiometers. You can change the intensity of each color by rotating the right potentiometer.<br>
The pontentiometers inputs are taken by the arduino board and then the board sends the **analog mapped values** to each of the LEDs anodes.<br>
I used the **orange**(for red), **green** and **blue** cables to color code each of the LEDs anodes as well as each of the potentiometers input by their respective color.<br>
The resistances are *100Ω* for the red anode and *220Ω* for the green and blue anodes.

Link to the working project [here]( https://youtu.be/1vLgQPF1mOE )
<br><br><br><br><br>
## Homework 2

<img src="https://user-images.githubusercontent.com/79469458/198998479-7c34525d-a87d-4304-929b-87c03761122e.jpeg" align="left" width="300" height="300">

This projects is something like a traffic lights simulator. Pressing the button will make the semaphore stop the cars and let people cross the road.<br>
As seen in the video down bellow the system has 4 stages, first being the neutral state and the rest being initiated by pressing the button. Each non-neutral stage runs for a given amount of time, coded using the **millis() function** that checks if enough time has passed to progress to the next stage.<br>
The button is connected to an **interrupt pin** and I use **INPUT_PULLUP** to solve the **floating pin problem**. Pressing it triggers an **interrupt function** that modifies a variable that makes the stage 1 progress in the second stage, the variable resets after stage 4.<br>
I used **red**, **yellow** and **green** cables for each respective LED, **orange** for the buzzer and **grey** for the button input. All ground cables are **black**. Resistances: *330Ω*-LEDs and *220Ω*-buzzer.

Link to the working project [here]( https://youtu.be/Otya_aDd8WY )

## Homework 3

<img src="https://user-images.githubusercontent.com/79469458/200681369-e332f099-4331-4090-9984-25811cee57e1.png" align="left" width="300" height="300">

This project consists in manipulating a 7-segment display using a joystick. The joystick can be moved to choose a segemnt and pressed to set the segment on/off(by moving the y axis). Long pressing in the select state causes a reset.
<br>I used **red** and **black** cables for the 5V/GND pins, **yellow** for the 7-segment LED pins, **blue** for analog inputs(for the joysticks X/Y axes), and **green** for the button pin. I have a *1kΩ* resistance for the 7-segment.

Link to the working project [here]( https://youtu.be/dO4oEZhUCy8 )
