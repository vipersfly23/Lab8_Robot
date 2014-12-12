Lab8_Robot
==========
Maze Robot -- SO MUCH FUN!

###Objective and Purpose:

The objective of this lab is to build and program an autonous robot that will navigate through a maze. 

Requirements:
1.Your robot must always start at the home position.
2.Your robot is considered successful only if it finds one of the three exits and moves partially out of the maze.
3.A large portion of your grade depends on which door you exit.
  1.Door 1 - Required Functionality
  2.Door 2 - B Functionality
  3.Door 3 - A Functionality1.You cannot hit a wall!
4.Bonus! Navigate from the A door back to the entrance using the same algorithm.1.You cannot hit a wall!
4.Your robot must solve the maze in less than three minutes.
5.Your robot will be stopped if it touches the wall more than twice.
6.Your robot must use the IR sensors to find its path through the maze.


A Functionality: [COMPLETED]

Best time: 45 seconds

Bonus Functionality:
  
Robot successfully navigated the maze back home.

###Implementation:

Implementation was completed using interrupts and setting flags. The flag woulds be set if the robot was closer or farther
than a predefined value, when the robot was within range the flag would be cleared.
Using interrupts i decided to use flags because it was simple and most of my code could be done within the interrupt.

Code:
Variables:
int16 rightValue: stores the converted value from the right sensor
int16 leftValue: stores the converted value from the left sensor
int16 centerValue: stores the converted value from the center sensor.
int8 leftFlagFar: If the robot is too far from the left wall, the this flag is set. (sed for following left wall)
int8 leftFlagNear: If the robot is too close to the wall this flag is set. (used for following the left wall)
int8 rightFlag: if the robot is too close to the right wall, this flag is set.
int8 centerFlag: if the robot is too close to a wall straigh ahead, the flag is set
int8 leftFlag: if the robot is too close to the left wall this flag is set (used for turning)

Functions:
initMSP430: initializes the MSP430, ADC, and Timer.

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/init.GIF "initMSP430")

motorControl: motor is controled by this function. the motor could be turned on,off,reverse or the speed could be increased
or decreased depending on the value passed in.

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/motorControl.GIF "motorControl")

selectInch: this function selects which pin to read from. turns off the ADC, selects the pin, and turns it back on.

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/select.GIF "selectInch")

Header:

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/header.GIF "Header")

Main:
 THe purpose of the code is to follow the left wall, when the center flag and the leftFlag is set, the robot knows to turn
 right.

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/main.GIF "Main")

Timer: Backbone of Lab. Sets all the flags and obtains values from the ADC.

![alt text](https://raw.githubusercontent.com/vipersfly23/Lab8_Robot/master/timer.GIF "Timer")

##Debugging/Testing

	Methodology: The methodology used was to use interrupts to set the flag. From the
	previous labs, getting the robot to run was simple. No further configuration was need to run the robot. The only addition
	is in the timer, flags are set to indicate the robot is too far, too close to a wall or needs to turn. Further more,
  follow the left wall. The robot just hugs the left wall until the center sensor indicates that there's a wall there.
  The reason I didn't use the right sensor was because it's too controllable having a free looping robot that never runs into
  a wall. Though it won't run into a wall, it's difficult to have it complete a maze.
	
	Testing: The testing was conducted in the maze. It was pretty much trial and error. I knew that my robot was worked,
I just had to configure the values correctly to complete the maze. A big issue with the robot was the battery, different battery would output different values thus I had to re-configure what values would set off the flags each time. Another problem with testing was the speed in correlation to the sensors. If the robot moved too fast the senors would be too slow and by the time the flags were set the robot would have already slam in a wall. The testing was difficult and tedious
but eventually worked and I was able to complete both A functionality and Bonus Functionality.
	
	
###Lessons Learned:

	The biggets lesson learned was that different power source has a huge impact on system, thus should be accounted for. 
	The characterization I completed in Lab7 was only applicable to that particular setting. There are also other outside
	factors that need to be considered, such as: room lighting, particular maze, power supply, sensor positions and
	shadows.
	All these played a significant role in the success of the robot.

###Conclusion	
	Conclusively, I had a great time with the robot. One of the purpose for this lab is to allow cadets to enjoy computer 	engineering and I definitely did that. Not only that, but the robot was a success. I was able to complete A
	functionality,along with bonus functionality. Though the robot moved slowly, the requirements were met and overall
	the robot was a	lot of fun!
	
###Documentation:
	
	Used codes from the lab7.c @ http://ecse.bd.psu.edu/cmpen352/.

