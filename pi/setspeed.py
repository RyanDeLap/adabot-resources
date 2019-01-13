import sys

from a_star import AStar
import time
a_star = AStar()

speed1 = int(sys.argv[1])
speed2 = int(sys.argv[2])

print("Setting left motor to " + str(speed1) + " and right motor to " + str(speed2))

a_star.motors(speed1, speed2)

while 1:
	time.sleep(0.1)
	encoders = a_star.read_encoders()
	print("Left: " + str(encoders[0]))
	print("Right: " + str(encoders[1]))