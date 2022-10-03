from socketio import Client
from pprint import pformat
import logging
import time


DELAY = 0.1

class ESPHub(Client):
    def __init__(self, url = "https://esphub.mehtank.com", **kwargs):
        logging.info(f"Initializing ESPHub object at {url}")
        Client.__init__(self)
        self._clients = dict()

        @self.on("list")
        def ls(data):
            logging.info("Got new client list:")
            self.clients = data
            logging.info(pformat(data))

        @self.event
        def joined(data):
            logging.info('user(s) joined:', data)
            self.clients.update(data)
            logging.info(pformat(self.clients))

        @self.event
        def left(data):
            logging.info('user(s) left:', data)
            for k in data.keys():
                self.clients.pop(k, None)
            logging.info(pformat(self.clients))

        logging.info(f"Connecting...")
        self.connect(url, **kwargs)
        time.sleep(DELAY)
        logging.info(f"Joining membership room...")
        self.emit("enter", "membership")
        logging.info(f"Requesting room list...")
        self.refresh()

    def emit(self, *args, **kwargs):
        Client.emit(self, *args, **kwargs)
        time.sleep(DELAY)

    @property
    def clients(self):
        return self._clients

    @clients.setter
    def clients(self, members):
        self._clients = members

    def refresh(self):
        self.emit("list")

    def esp(self, name=''):
        fns = (
            lambda k, v: k, 
            lambda k, v: v["name"], 
            lambda k, v: "%08x" % v["mac"], 
            lambda k, v: v["ip"], 
        )

        for fn in fns:
            for k, v in self.clients.items():
                if name == fn(k, v):
                    return k
            for k, v in self.clients.items():
                if name in fn(k, v):
                    return k
        return None

    def command(self, event, data=None, who=''):
        if who not in self._clients:
            who = self.esp(who)

        if who in self._clients:
            logging.info(f"Sending command to {who}: event = {event}")
            self.emit("command", dict(sid=who, event=event, data=data))
        else:
            logging.warning("Client not found")

    def __getattr__(self, fn):
        return lambda *args, **kwargs: self.command(fn, *args, **kwargs)

    def __del__(self):
        self.disconnect()
