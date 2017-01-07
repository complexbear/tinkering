'''

Rules for drawing L-Systems

Program format: (scaling factor, angle, initial sequence, [Rules])

'''
from model import *

Koch_1 = Program(4, 90, 'F-F-F-F', [Rule('F', 'F-F+F+FF-F-F+F')] )
Koch_2 = Program(4, 90, 'F-F-F-F', [Rule('F', 'F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F')] )
Koch_3 = Program(4, 90, 'F-F-F-F', [Rule('F', 'FF-F-F-F-F-F+F')] )
Koch_4 = Program(4, 90, 'F-F-F-F', [Rule('F', 'FF-F--F-F')] )

Snowflake = Program(4, 90, '-F', [Rule('F', 'F+F-F-F+F')] )

Lakes = Program(2, 90, 'F+F+F+F', [Rule('F', 'F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF'),
                                   Rule('f', 'ffffff')])

Gasket = Program(6, 60, 'Fm', [Rule('Fn', 'Fm+Fn+Fm'),
                               Rule('Fm', 'Fn-Fm-Fn')])

Plant_1 = Program(5, 25.7, 'F', [Rule('F', 'F[+F]F[-F]F')])
Plant_2 = Program(5, 22.5, 'n', [Rule('n', 'F-[[n]+n]+F[+Fn]-n'),
                                 Rule('F', 'FF')])

StochPlant_1 = Program(5, 25.7, 'F', [StochasticRule('F', [(0.33, 'F[+F]F[-F]F'),
                                                           (0.33, 'F[+F]F'),
                                                           (0.33, 'F[-F]F')])])