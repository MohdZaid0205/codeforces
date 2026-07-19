import sys
import math
from collections import defaultdict, deque, Counter, OrderedDict
from heapq import heappush, heappop, heapify
from functools import lru_cache, cmp_to_key
from itertools import permutations, combinations, accumulate


# Increase recursion depth for deep DFS
sys.setrecursionlimit(200005)

MOD = 10**9 + 7

class Point:
    __slots__ = ['x', 'y']
    
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y
        
    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)
        
    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)
        
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y
        
    def __hash__(self):
        return hash((self.x, self.y))
        
    def __repr__(self):
        return f"({self.x}, {self.y})"
class Mint:
    __slots__ = ['val']
    
    def __init__(self, val=0):
        self.val = val % MOD
        
    def __add__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        return Mint(self.val + ov)
        
    def __sub__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        return Mint(self.val - ov)
        
    def __mul__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        return Mint(self.val * ov)
        
    def __iadd__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        self.val = (self.val + ov) % MOD
        return self
        
    def __isub__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        self.val = (self.val - ov) % MOD
        return self
        
    def __imul__(self, other):
        ov = other.val if isinstance(other, Mint) else other
        self.val = (self.val * ov) % MOD
        return self
        
    def __repr__(self):
        return str(self.val)

# --- Utility Functions ---
def lcm(a: int, b: int) -> int:
    if a == 0 or b == 0: return 0
    return (a * b) // math.gcd(a, b)
def get_factors(n: int) -> list:
    if n <= 0: return []
    factors = []
    i = 1
    while i * i <= n:
        if n % i == 0:
            factors.append(i)
            if i != n // i:
                factors.append(n // i)
        i += 1
    factors.sort()
    return factors
def vmin(*args): return min(args)
def vmax(*args): return max(args)
class LRUCache:
    def __init__(self, capacity: int):
        self.capacity = capacity
        self.cache = OrderedDict()

    def get(self, key):
        if key not in self.cache:
            return None
        self.cache.move_to_end(key)
        return self.cache[key]

    def put(self, key, value):
        if key in self.cache:
            self.cache.move_to_end(key)
        self.cache[key] = value
        if len(self.cache) > self.capacity:
            self.cache.popitem(last=False)

def solve(get_int, get_str):
    # ---------------+------------+------------------+------------------+
    #  THAT PENGUIN  |     __     | IS GOING PLACES  |      __,_____    |
    #  THAT PENGUIN  |    ( o>    | IS GOING PLACES  |     / __.==--"   |
    #  THAT PENGUIN  |    ///\    | IS GOING PLACES  |    /#(-'         |
    #  THAT PENGUIN  |    \V_/_   | IS GOING PLACES  |    `-'           | 
    #  THAT PENGUIN  |            | IS GOING PLACES  |   FUCK FOOBAR    |
    # ---------------+------------+------------------+------------------+

    a = input()
    b = input()

    if sum([int(c) for c in a])%10 != sum([int(c) for c in b])%10:
        print(-1)
        return

    # l = 1
    # h = min(len(b), len(b))

    # def can(a, b) -> bool :
        # lol this will go to n^p where p is number of partioions
    
    al = list(map(int, list(a)))
    bl = list(map(int, list(b)))

    for i in range(1, len(al)):
        al[i] = (al[i-1] + al[i])%10

    for i in range(1, len(bl)):
        bl[i] = (bl[i-1] + bl[i])%10

    dp = [[-1]*len(al) for _ in range(len(bl))]

    # for i in range(0, len(al)):
    #     for j in range(0, len(bl)):
    #         dp[j][i] = 1 if al[i] == bl[j] else 0
    # print(dp)

    for j in range(0, len(bl)):
        for i in range(0, len(al)):
            if al[i] == bl[j]:
                dd = dp[j-1][i-1] if (i > 0 and j > 0) else 0
                dp[j][i] = dd + 1
            else:
                u = dp[j-1][i] if j > 0 else 0
                l = dp[j][i-1] if i > 0 else 0
                dp[j][i] = max(u, l)
                
    print(dp[-1][-1])

    # ------------------------------------------------------------------+
    #                                     ______                        |
    #                  |\_______________ (_____\\______________         |
    #          HH======#H###############H#######################        |
    #                  ' ~""""""""""""""`##(_))#H\"""""Y########        |
    #          FUCK FOOBAR               ))    \#H\      `"Y###         |
    #                                    "      }#H)                    |
    #                                                                   |
    #-------------------------------------------------------------------+
    ...


def main():
    # input_data = sys.stdin.read().split()
    # if not input_data:
    #     return
        
    # iterator = iter(input_data)
    
    # def get_str():
    #     # return next(iterator)
        
    # def get_int():
    #     return int(next(iterator))

    try:
        t = int(input())
    except StopIteration:
        return

    for _ in range(t):
        solve(0, 0)

if __name__ == "__main__":
    main()