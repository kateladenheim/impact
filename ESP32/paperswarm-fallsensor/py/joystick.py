#!/usr/bin/env python3

import pygame
import json
from addict import Dict
import logging


class Joyous:
    def __init__(self, conf=None):
        # Set up joystick
        pygame.init()
        pygame.joystick.init()
        self.config_file = conf

        self.js = pygame.joystick.Joystick(0)
        self.js.init()
        self.conf = None
        self.loadconfig()
        self.done = False

        logging.info(f"Starting joystick with" + \
                        f"{self.js.get_numbuttons()} buttons and " + \
                        f"{self.js.get_numaxes()} axes.")

    @property
    def conf(self):
        return self._conf

    @conf.setter
    def conf(self, c):
        self._conf = c
        self.laststate = self.state

    @property
    def buttons(self):
        return [self.js.get_button(i) for i in range(self.js.get_numbuttons())]

    @property
    def axes(self):
        return [self.js.get_axis(i) for i in range(self.js.get_numaxes())]

    @property
    def state(self):
        if self.conf:
            d = Dict()
            for k, v in self.conf.axes.items():
                d[k] = d.axes[k] = self.js.get_axis(v)
            for k, v in self.conf.buttons.items():
                d[k] = d.buttons[k] = self.js.get_button(v)
            return d
        else:
            return self.rawstate

    @property
    def rawstate(self):
        return Dict(axes=self.axes, buttons=self.buttons)

    @property
    def delta(self):
        d = Dict()
        state = self.state
        for k in ("axes", "buttons"):
            try:
                keys = state[k].keys()
            except AttributeError:
                keys = range(len(state[k]))

            for i in keys:
                b1 = self.laststate[k][i]
                b2 = state[k][i]
                if b1 != b2:
                    d["d"+k][i] = b2-b1
                    d[k][i] = b2
        self.laststate = state
        return d

    def loadconfig(self):
        if not self.config_file:
            return None

        try:
            with open(self.config_file) as f:
                self.conf = Dict(json.load(f))
                logging.info(f"Successfully loaded configuration from {self.config_file}")
                return self.conf
        except FileNotFoundError:
            logging.warning(f"Configuration file {self.config_file} not found")
            logging.warning("--> build using configure(axes, buttons)")

        return None

    def configure(self, axes, buttons, force=False):
        if not force:
            old = self.loadconfig()
            if old and set(old.axes.keys()) == set(axes) and \
                       set(old.buttons.keys()) == set(buttons):
                logging.info(f"Using cached config from {self.config_file}")
                return old

        logging.info(f"Rebuilding joystick config file...")
        self.conf = None
        conf = Dict()

        def choose(inptype, keys, threshold):
            for k in keys:
                print(f"Push {inptype} for {k}")
                count = Dict()
                while self.poll():
                    delta = self.delta["d"+inptype]
                    for inp in delta:
                        if delta[inp] > 0:
                            count[inp] += 1
                    for inp in count:
                        if count[inp] >= threshold:
                            print("  -> detected index: ", inp)
                            conf[inptype][k] = inp
                            break
                    else:
                        continue
                    break

        choose("axes", axes, 10)
        choose("buttons", buttons, 1)

        with open(self.config_file, 'w') as f:
            json.dump(conf, f)
            logging.info(f"Configuration written to {self.config_file}")
        self.conf = conf
        return conf

    def quit(self):
        pygame.quit()
        self.done = True
        
    def wait(self):
        pygame.time.wait(100)

    def poll(self):
        if self.done:
            return False

        try:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.quit()
                    return False

        except KeyboardInterrupt:
            self.quit()
            return False

        return True
