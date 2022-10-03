#!/bin/env python

from pprint import pprint
import time

from joystick import Joyous
from esphub import ESPHub


buttons = '''
    blink
    quit
'''.strip().split()

axes = '''
    breathe1
    breathe2
    servo1
    servo2
    servo3
    servo4
'''.strip().split()

hub = ESPHub()
print("ESPHub clients:")
print(hub.clients)

if hub.esp():
    hub.blink(10); time.sleep(.1); hub.blink(10); time.sleep(.1); hub.blink(10); time.sleep(.1)

js = Joyous('config.json')
js.configure(axes, buttons)

print("Joystick configured, starting control loop...")
while js.poll():
    d = js.delta
    if d:
        pprint(d)

    breathe = lambda x : 1-(x+1)/2
    servo = lambda x : int(90 + x*90)
    fake = lambda x : 0.18 + 0.48 * breathe(x)

    axes = dict(
        breathe1 = (2, breathe),
        servo1   = (3, servo),
        servo3   = (5, servo),
        servo2   = (4, servo),
        servo4   = (0, servo),
    )

    pins = []
    for k, (pin, fn) in axes.items():
        if k in d.axes:
            pins.append( (pin, fn(d.axes[k])) )

    if not hub.esp() and (pins or d.buttons.blink):
        print("Requesting list of clients...")
        hub.refresh()
    else:
        if pins:
            print("Sending pins:")
            pprint(pins)
            hub.pins(pins)
        if d.buttons.blink:
            print("Blink!")
            hub.blink()

    if js.state.quit:
        print("Caught quit button, quitting...")
        js.quit()

print("Joystick quit, waiting for esphub")
hub.disconnect()
