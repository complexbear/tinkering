# Demo of the bi-directional communication of generators in Python

def generator(seq_len):
    x = 0
    while x < seq_len:
        # Return our data values to the user of the generator as normal
        # But we can also receive values back from the user to modify our
        # generator sequence
        jump_size = yield x
        x += jump_size if jump_size else 1


if __name__ == '__main__':

    # Use case 1 - normal iterator of the generator
    gen = generator(3) # Create an instance of the generator
    print('normal iteration: {}'.format(list(gen)))

    # Use case 2 - choose an alternate step size in the sequence
    gen = generator(20)

    # Must start the iteration this way as we haven't reached the yield
    # statement in the generator function yet
    x = gen.send(None) 
    try:
        while True:
            print('yielded {}'.format(x))
            # We can modify the behaviour of the generator during it's operation
            x = gen.send(x + 1)
    except StopIteration:
        print('received end of generator exception')



