'''
    This module applies Rules to the document in iterative generations.
    There are two modes in which this can be done
        * Edge replacement
        * Node replacement

    Both generate the same document that describes the structure according
    to the Rules used.

    Node generation may be easier to use in the renderer to keep track of
    which generation produced a given piece of the structure. This can be
    then used to vary the thickness of branches between generations.
'''
import logging

from rules import *

class EdgeGenerate(object):
    logger = logging.getLogger('EdgeGenerate')

    def _applyRules(self, rules, doc, generation):
        newDoc = ''
        idx = 0
        while idx < len(doc):
            ruleMatched = False
            for rule in rules:
                consumed, ruleDoc = rule.apply(doc[idx:])
                if consumed:
                    idx += consumed
                    ruleMatched = True
                    newDoc += ruleDoc
            if not ruleMatched:
                newDoc += doc[idx]
                idx += 1
        return newDoc

    def __call__(self, program, generations):
        doc = program.initiator
        self.logger.debug('generation %s: %s' % (0, doc))
        for n in range(generations):
            doc = self._applyRules(program.rules, doc, n)
            self.logger.debug('generation %s: %s' %(n+1, doc))
        self.logger.debug('doc: %s' %doc)
        return doc


Node = namedtuple('Node', 'data generation')

class NodeGenerate(object):
    logger = logging.getLogger('NodeGenerate')

    def _applyRules(self, rules, node):
        newData = []
        idx = 0
        self.logger.debug('apply rule: %s' %node.data)
        while idx < len(node.data):
            if type(node.data[idx]) == Node:
                subNode = node.data[idx]
                newData.append(self._applyRules(rules, subNode))
                idx += 1
            else:
                ruleMatched = False
                for rule in rules:
                    consumed, ruleDoc = rule.apply(node.data[idx])
                    if consumed:
                        idx += consumed
                        ruleMatched = True
                        newData.append(Node(ruleDoc, node.generation+1))
                if not ruleMatched:
                    newData.append(node.data[idx])
                    idx += 1
        return Node(newData, node.generation)


    def __call__(self, program, generations):
        root = Node(list(program.initiator), 1)

        for n in range(generations):
            self.logger.info('generation %s' %n)
            root = self._applyRules(program.rules, root)
        return Node(list(program.initiator) + [root], 1)



