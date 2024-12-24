# /usr/bin/time --verbose python nsieve.py 100000000
import sys

def nsieve(n):
    count = 0
    flags = [True] * n
    for i in range(2, n):
        if flags[i]:
            count += 1
            for j in range(i*2, n, i):
                flags[j] = False 
    print("Number of primes up to n={}: {}".format(n, count))


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 4
    nsieve(n)
