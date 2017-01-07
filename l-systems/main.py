'''
Created on 12 Dec 2016

@author: matt
'''
import logging
from optparse import OptionParser



from model import EdgeGenerate, NodeGenerate
import render
import rulesets


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
    parser.add_option('-w', dest='wind', type='int', default=0)

    opts, _ = parser.parse_args()
    if opts.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    if opts.offset:
        render.START_OFFSET = opts.offset

    program = getattr(rulesets, opts.name)

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
            renderer.draw(doc, steps, program.angle, opts.wind)
        else:
            break

if __name__ == '__main__':
    main()
