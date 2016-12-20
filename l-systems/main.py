'''
Created on 12 Dec 2016

@author: matt
'''
import logging
from optparse import OptionParser

from model import Program, Rule, EdgeGenerate, NodeGenerate
import render

Koch_1 = Program(4, 90, 'F-F-F-F', [Rule('F', 'F-F+F+FF-F-F+F')] )
Koch_2 = Program(4, 90, 'F-F-F-F', [Rule('F', 'F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F')] )
Koch_3 = Program(4, 90, 'F-F-F-F', [Rule('F', 'FF-F-F-F-F-F+F')] )
Koch_4 = Program(4, 90, 'F-F-F-F', [Rule('F', 'FF-F--F-F')] )

Snowflake = Program(4, 90, '-F', [Rule('F', 'F+F-F-F+F')] )

Lakes = Program(2, 90, 'F+F+F+F', [Rule('F', 'F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF'),
                                   Rule('f', 'ffffff')])

def main():
    logging.basicConfig()
    logging.getLogger().setLevel(logging.INFO)

    parser = OptionParser()
    parser.add_option('-n', dest='name')
    parser.add_option('-g', dest='generations', type='int', default=1)
    parser.add_option('-s', dest='size', type='int', default=50)
    parser.add_option('-v', dest='verbose', action='store_true', default=False)

    opts, _ = parser.parse_args()
    if opts.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    program = globals()[opts.name]

    steps = opts.size / (opts.generations * program.scale)

    gen = EdgeGenerate()
    doc = gen(program, opts.generations)

    rr = render.PlainRenderer()
    rr.draw(doc, steps, program.angle)

    # gen = NodeGenerate()
    # rootNode = gen(program, opts.generations)
    #
    # rr = render.NodeRenderer()
    # rr.draw(rootNode, steps, program.angle)

if __name__ == '__main__':
    main()
    raw_input('any key to exit')