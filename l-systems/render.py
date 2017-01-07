'''
Created on 12 Dec 2016

@author: matt
'''
import turtle
import logging
from collections import namedtuple

from model import Node

turtle.mode('logo')

START_OFFSET = 0

WIND = 0

class Action(object):
    State = namedtuple('State', 'position heading width')

    def __init__(self, turtle, steps, angle):
        self.turtle = turtle
        self.steps = steps
        self.leftAngle = angle
        self.rightAngle = angle
        self._actions = {'F': self.fwd,
                         'f': self.hop,
                         '-': self.left,
                         '+': self.right,
                         '[': self.pushState,
                         ']': self.popState}
        self.state = []
        self.goto(0, START_OFFSET)
        self.pensize = 3
        self.turtle.pensize(self.pensize)

    def __call__(self, action):
        func = self._actions.get(action)
        if func:
            func()

    def windy(self, force):
        self.leftAngle -= force * 0.5
        self.rightAngle += force * 0.5

    def fwd(self):
        self.turtle.forward(self.steps)

    def hop(self):
        self.turtle.up()
        self.turtle.forward(self.steps)
        self.turtle.down()

    def left(self):
        self.turtle.left(self.leftAngle)

    def right(self):
        self.turtle.right(self.rightAngle)

    def goto(self, x, y):
        self.turtle.up()
        self.turtle.setpos(x,y)
        self.turtle.down()

    def pushState(self):
        s = self.State(self.turtle.position(), self.turtle.heading(), self.turtle.pensize())
        self.state.append(s)

    def popState(self):
        s = self.state.pop()
        self.goto(*s.position)
        self.turtle.setheading(s.heading)
        self.turtle.pensize(s.width)

    def width(self, generation):
        size = self.pensize * (1.0/generation)
        self.turtle.pensize(max(0.1, size))


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
        turtle.hideturtle()

    def draw(self, doc, steps, angle, wind=0):
        self.reset()
        with TurtleContext():
            action = Action(self.turtle, steps, angle)
            if wind:
                action.windy(wind)
            self._elemDraw(action, doc)

    def _elemDraw(self, action, doc):
        [action(elem) for elem in doc]



class NodeRenderer(PlainRenderer):
    logger = logging.getLogger('NodeRenderer')

    def _drawNode(self, action, node):
        self.logger.debug('node generation: %s' %node.generation)
        for elem in node.data:
            action.width(node.generation)
            if type(elem) == Node:
                self._drawNode(action, elem)
            else:
                action(elem)

    def _elemDraw(self, action, root):
        for elem in root.data:
            action.width(root.generation)
            if type(elem) == Node:
                self._drawNode(action, elem)
            else:
                action(elem)



