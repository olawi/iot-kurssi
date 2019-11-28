from gpiozero import RGBLED

""" LED number/pin indexes map to GPIO pins"""
LED1_RED = 16
LED1_GRN = 20
LED1_BLU = 21

LED2_RED = 17
LED2_GRN = 27
LED2_BLU = 22

LED3_RED = 10
LED3_GRN = 9
LED3_BLU = 11

PWM_FREQ = 120

""" Define the led array parameters """
leds = []

leds.append(RGBLED(LED1_RED, LED1_GRN, LED1_BLU))
leds.append(RGBLED(LED2_RED, LED2_GRN, LED2_BLU))
leds.append(RGBLED(LED3_RED, LED3_GRN, LED3_BLU))

