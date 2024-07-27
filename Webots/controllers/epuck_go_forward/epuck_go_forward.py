from controller import Robot, Motor

TIME_STEP = 64

MAX_SPEED = 6.28

# create the Robot instance.
robot = Robot()

# get a handler to the motors and set target position to infinity (speed control)
leftMotor = robot.getDevice('left wheel motor')
rightMotor = robot.getDevice('right wheel motor')
leftMotor.setPosition(6.28)
rightMotor.setPosition(0)
leftMotor.setPosition(6.28)
rightMotor.setPosition(6.28)

# set up the motor speeds at 10% of the MAX_SPEED.
leftMotor.setVelocity(10)
rightMotor.setVelocity(12)

while robot.step(TIME_STEP) != -1:
   pass