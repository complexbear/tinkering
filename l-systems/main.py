'''
Created on 12 Dec 2016

@author: matt
'''
import logging
from optparse import OptionParser

from model import Program, Rule, StochasticRule, EdgeGenerate, NodeGenerate
import render

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


def main():
    logging.basicConfig()
    logging.getLogger().setLevel(logging.INFO)

    parser = OptionParser()
    parser.add_option('-m', dest='mode', default='edge')
    parser.add_option('-n', dest='name')
    parser.add_option('-g', dest='generations', type='int', default=1)
    parser.add_option('-s', dest='size', type='int', default=50)
    parser.add_option('--offset', dest='offset', type='int', default=0)
    parser.add_option('-v', dest='verbose', action='store_true', default=False)

    opts, _ = parser.parse_args()
    if opts.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    if opts.offset:
        render.START_OFFSET = opts.offset

    program = globals()[opts.name]

    steps = opts.size / (opts.generations * program.scale)

    if opts.mode == 'edge':
        gen = EdgeGenerate()
        renderer = render.PlainRenderer()
    else:
        gen = NodeGenerate()
        renderer = render.NodeRenderer()

    while True:
        key = raw_input('Press return to generate, anything else to exit')
        if key == '':
            doc = gen(program, opts.generations)
            renderer.draw(doc, steps, program.angle)
        else:
            break

if __name__ == '__main__':
    main()
