# ADAbot Resources

This repository houses the code and resources used for the [ADAbot](https://people.missouristate.edu/AnthonyClark/arcs-lab/) project.

ADABot uses a Raspberry Pi and a Pololu A-Star robot controller which is based on ardunio for control and monitoring. 

## Source Code

The ADABot source code is split into two folders, the astar and pi folders.

The code that runs on the pi uses a library thats already imported (a_star.py) from pololu.

setspeed.py is something I wrote for sending signals to the a-star to control the attached electric motors. (super simple proof of concept) It can be used to change speeds on the fly. 

The a-star robot works on the Arduino platform. Within the astar folder, is the main.ino file which contains the code that runs on the a-star robot controller. This code was adapted from the a-star example library. 

The main.ino also handles setting up the interrupt pins for reading encoder data from the motor encoders. It then sends this over the write to the raspberry pi, which is read and consumed. This is so that the motor speeds can be changed if needed.

The pi can send motor speed information to a-star, and the a-star can read motor encoder information, and send it back to the Pi. As well the a-star can control other electrical components plugged into it such as servos. 

## Useful Links
* [The Pololu A-Star Robot Controller Product Page](https://www.pololu.com/product/3119)
* [Pololu A-Star Slave Library](https://github.com/pololu/pololu-rpi-slave-arduino-library)
* [Pololu A-Star Robot Controller Manual](https://www.pololu.com/docs/pdf/0J66/a-star_32u4_robot_controller.pdf)
* [Pololu A-Star Robot Project Example](https://www.pololu.com/blog/577/building-a-raspberry-pi-robot-with-the-a-star-32u4-robot-controller)
* [Motor Encoders Used in the Project](https://www.pololu.com/product/2598)
* [The C++ examples that run on the A-Star](https://github.com/pololu/a-star-32u4-arduino-library)
* [Install Board information to Ardunio IDE](https://github.com/pololu/a-star-32u4-arduino-library)
* [Install Windows Drivers Needed](https://www.pololu.com/docs/0J61/6.1)