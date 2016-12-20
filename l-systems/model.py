'''
Created on 12 Dec 2016

@author: matt
'''
import logging
from collections import namedtuple

GRAMMAR = ('F', 'f', '-', '+', '[', ']', 'n', 'm')

Node = namedtuple('Node', 'data')

Rule = namedtuple('Rule', 'key generator')

Program = namedtuple('Program', 'scale angle initiator rules')



class EdgeGenerate(object):
    logger = logging.getLogger('EdgeGenerate')

    def _applyRule(self, rule, doc):
        newDoc = ''
        keyLen = len(rule.key)
        for idx in range(0, len(doc)):
            elem = doc[idx:idx + keyLen]
            if elem == rule.key:
                newDoc += rule.generator
            else:
                newDoc += doc[idx]
        return newDoc

    def __call__(self, program, generations):
        doc = program.initiator
        for n in range(generations):
            self.logger.info('generation %s' %n)
            for r in program.rules:
                doc = self._applyRule(r, doc)
        self.logger.debug('Doc: %s' %doc)
        return doc


class NodeGenerate(object):
    logger = logging.getLogger('NodeGenerate')

    def _applyRule(self, rule, node):
        newData = []
        keyLen = len(rule.key)
        self.logger.debug('apply rule: %s' %node.data)
        for idx in range(0, len(node.data)):
            if type(node.data[idx]) == Node:
                subNode = node.data[idx]
                newData.append(self._applyRule(rule, subNode))
            else:
                elem = node.data[idx : idx + keyLen]
                if elem == rule.key:
                    newData.append(Node(rule.generator))
                else:
                    newData.append(node.data[idx])
        return Node(newData)


    def __call__(self, program, generations):
        root = Node(list(program.initiator))

        # convert rule strings to lists for nesting
        rules = [Rule(list(r.key), list(r.generator)) for r in program.rules]

        for n in range(generations):
            self.logger.info('generation %s' %n)
            for r in rules:
                root = self._applyRule(r, root)
        return Node(list(program.initiator) + [root])



