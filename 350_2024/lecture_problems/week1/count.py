# /usr/bin/time --verbose python count.py 1000000000
import sys

def count(n):
    count = 0
    for i in range(n):
        count += 1
    print("Number of numbers up to n={}: {}".format(n, count))


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 4
    count(n)