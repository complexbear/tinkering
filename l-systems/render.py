'''
Created on 12 Dec 2016

@author: matt
'''
import turtle
import logging
from collections import namedtuple

from model import Node

class Action(object):
    def __init__(self, turtle, steps, angle):
        self.turtle = turtle
        self.steps = steps
        self.angle = angle
        self._actions = {'F': self.fwd,
                         'f': self.hop,
                         '-': self.left,
                         '+': self.right}

    def __call__(self, action):
        func = self._actions.get(action)
        if func:
            func()

    def fwd(self):
        self.turtle.forward(self.steps)

    def hop(self):
        self.turtle.up()
        self.turtle.forward(self.steps)
        self.turtle.down()

    def left(self):
        self.turtle.left(self.angle)

    def right(self):
        self.turtle.right(self.angle)

    def goto(self, x, y):
        self.turtle.up()
        self.turtle.setpos(x,y)
        self.turtle.down()


class TurtleContext(object):
    def __enter__(self):
        turtle.tracer(0, 0)
    def __exit__(self, type, value, traceback):
        turtle.update()


class PlainRenderer(object):
    def __init__(self):
        self.scr = turtle.Screen()
        self.turtle = turtle.Turtle()

    def reset(self):
        self.scr.clear()
        self.turtle.reset()
        self.turtle.speed(0)

    def draw(self, doc, steps, angle):
        self.reset()
        with TurtleContext():
            action = Action(self.turtle, steps, angle)
            [action(elem) for elem in doc]



class NodeRenderer(PlainRenderer):
    logger = logging.getLogger('NodeRenderer')

    Node = namedtuple('Node', 'x y')

    def _drawNode(self, action, node):
        for elem in node.data:
            if type(elem) == Node:
                self._drawNode(action, elem)
            else:
                action(elem)

    def draw(self, root, steps, angle):
        self.reset()
        with TurtleContext():
            action = Action(self.turtle, steps, angle)

            for elem in root.data:
                if type(elem) == Node:
                    self._drawNode(action, elem)
                else:
                    action(elem)



