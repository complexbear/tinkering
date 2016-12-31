'''
Created on 12 Dec 2016

@author: matt
'''
import logging

from rules import *

Node = namedtuple('Node', 'data')

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



