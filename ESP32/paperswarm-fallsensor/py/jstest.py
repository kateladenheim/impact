#!/bin/env python3

from joystick import Joyous
from pprint import pprint


if __name__ == "__main__":
    js = Joyous()
    while js.poll():
        d = js.delta
        if d:
            pprint(d)

