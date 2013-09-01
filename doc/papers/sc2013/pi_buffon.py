from math import sqrt, pow
from random import random
import timeit

def pi_estimate(darts):
    hits = 0
    for i in xrange (0, darts):
    	x = random()
    	y = random()
    	dist = sqrt(pow(x, 2) + pow(y, 2))
    	if dist <= 1.0:
            hits += 1.0
    # hits / throws = 1/4 Pi
    pi = 4 * (hits / darts)
    return pi

from threading import Thread
def threaded_pi_estimate(darts):
    t1 = Thread(target=pi_estimate, args=(darts//4,))
    t2 = Thread(target=pi_estimate, args=(darts//4,))
    t3 = Thread(target=pi_estimate, args=(darts//4,))
    t4 = Thread(target=pi_estimate, args=(darts//4,))
    t1.start() ; t2.start()
    t3.start() ; t4.start()
    t1.join() ; t2.join()
    t3.join() ; t4.join()

from multiprocessing import Pool
def multi_pi_estimate(darts):
    p = Pool(4)
    p.map(pi_estimate,[darts//4]*4)

if __name__ == '__main__':
    median = lambda x: sorted(x)[len(x) / 2]
    nb_iter = 21

    ti = timeit.Timer("pi_estimate(400000)", "from __main__ import pi_estimate")
    stiming = median(ti.repeat(nb_iter, number=1))
    print "sequential", stiming

    ti = timeit.Timer("threaded_pi_estimate(400000)", "from __main__ import threaded_pi_estimate")
    ttiming = median(ti.repeat(nb_iter, number=1))
    print "threaded", ttiming
    print "ratio", ttiming/stiming

    ti = timeit.Timer("multi_pi_estimate(400000)", "from __main__ import multi_pi_estimate")
    mtiming = median(ti.repeat(nb_iter, number=1))
    print "multi", mtiming
    print "ratio", mtiming/stiming

