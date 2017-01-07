from collections import namedtuple
import random

# Leave commented to generate different stochastic structures
#random.seed(0)

# Define the list of symbols that can be used in our L-Systems
GRAMMAR = ('F', # Forward
           'f', # Forward with pen up
           '-', # Left turn
           '+', # Right turn
           '[', # Push pen state onto stack
           ']', # Pop pen state from stack
           'n', # Label used to identify branch variants
           'm', # Label used to identify branch variants
           ' ') # Ignored, only used to format rule string

Program = namedtuple('Program', 'scale angle initiator rules')

class Rule(object):
    '''
        Rule class to match rule key on document elements
        and replace the key with the generator pattern if
        the rule matches
    '''
    def __init__(self, key, generator):
        self.key = key
        self.generator = generator
        self.keyLen = len(self.key)

    def apply(self, doc):
        '''
            Return number of doc elements consumed and the
            new sequence that replaces them
        '''
        elem = doc[:self.keyLen]
        if elem == self.key:
            return self.keyLen, self.generator
        return 0, ''


class StochasticRule(object):
    '''
        Same concept as a Rule class but has multiple sub-rules
        which have the same key.

        The sub-rules have defined probabilities of being applied
        in the event of a match
    '''
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
            # if weights do not exactly sum to 1, favour the last rule
            subRule = self.generators[-1]
            randomPick = random.random()
            for idx, wb in enumerate(self.weightBoundaries):
                if randomPick <= wb:
                    subRule = self.generators[idx]
                    break
            return self.keyLen, subRule.pattern
        return 0, ''




