'''
Created on 12 Dec 2016

@author: matt
'''
import logging
from collections import namedtuple
import random

#random.seed(0)

GRAMMAR = ('F', 'f', '-', '+', '[', ']', 'n', 'm', ' ')

Node = namedtuple('Node', 'data')

Program = namedtuple('Program', 'scale angle initiator rules')

class Rule(object):
    def __init__(self, key, generator):
        self.key = key
        self.generator = generator
        self.keyLen = len(self.key)

    def apply(self, doc):
        elem = doc[:self.keyLen]
        if elem == self.key:
            return self.keyLen, self.generator
        return 0, ''


class StochasticRule(object):
    Generator = namedtuple('Generator', 'weight pattern')
    def __init__(self, key, generators):
        self.key = key
        self.generators = [self.Generator(*g) for g in generators]
        self.keyLen = len(key)
        # Map weights to boundaries between 0->1
        counter = 0
        self.weightBoundaries = []
        for w in map(lambda g: g.weight, self.generators):
            counter += w
            self.weightBoundaries.append(counter)

    def apply(self, doc):
        elem = doc[:self.keyLen]
        if elem == self.key:
            # pick sub-rule based on weights
            subRule = self.generators[0]
            randomPick = random.random()
            for idx, wb in enumerate(self.weightBoundaries):
                if randomPick <= wb:
                    subRule = self.generators[idx]
                    break
            return self.keyLen, subRule.pattern
        return 0, ''






class EdgeGenerate(object):
    logger = logging.getLogger('EdgeGenerate')

    def _applyRules(self, rules, doc):
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
            doc = self._applyRules(program.rules, doc)
            self.logger.debug('generation %s: %s' %(n+1, doc))
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



