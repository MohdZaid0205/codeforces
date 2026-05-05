#![allow(unused_imports, dead_code, non_snake_case, unused_macros)]

// --- Includes ---
use std::cmp::{max, min, Ordering};
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet, VecDeque, BinaryHeap};
use std::io::{self, BufRead, BufWriter, Write};
use std::ops::{Add, AddAssign, Mul, MulAssign, Sub, SubAssign};
use std::str::FromStr;

// --- Constants ---
const MOD: i64 = 1_000_000_007;

// --- Predefined Structures ---
#[derive(Copy, Clone, Debug, PartialEq, Eq, Hash)]
struct Point {
    x: i64,
    y: i64,
}

impl Add for Point {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self { x: self.x + other.x, y: self.y + other.y }
    }
}

impl Sub for Point {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self { x: self.x - other.x, y: self.y - other.y }
    }
}

// Modular Arithmetic Struct (mint)
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
struct Mint {
    val: i64,
}

impl Mint {
    fn new(v: i64) -> Self {
        let mut val = v % MOD;
        if val < 0 { val += MOD; }
        Self { val }
    }
}

impl Add for Mint {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        let mut val = self.val + other.val;
        if val >= MOD { val -= MOD; }
        Self { val }
    }
}

impl Sub for Mint {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        let mut val = self.val - other.val;
        if val < 0 { val += MOD; }
        Self { val }
    }
}

impl Mul for Mint {
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        Self { val: (self.val * other.val) % MOD }
    }
}

impl AddAssign for Mint { fn add_assign(&mut self, other: Self) { *self = *self + other; } }
impl SubAssign for Mint { fn sub_assign(&mut self, other: Self) { *self = *self - other; } }
impl MulAssign for Mint { fn mul_assign(&mut self, other: Self) { *self = *self * other; } }

// --- Utility Functions ---
fn gcd<T: Copy + PartialEq + Default + std::ops::Rem<Output = T>>(mut a: T, mut b: T) -> T {
    let zero = T::default();
    while b != zero {
        let temp = b;
        b = a % b;
        a = temp;
    }
    a
}

fn lcm(a: i64, b: i64) -> i64 {
    if a == 0 || b == 0 { 0 } else { (a / gcd(a, b)) * b }
}

fn chmin<T: PartialOrd>(a: &mut T, b: T) -> bool {
    if b < *a { *a = b; true } else { false }
}

fn chmax<T: PartialOrd>(a: &mut T, b: T) -> bool {
    if b > *a { *a = b; true } else { false }
}

fn get_factors(n: i64) -> Vec<i64> {
    let mut factors = Vec::new();
    if n <= 0 { return factors; }
    let mut i = 1;
    while i * i <= n {
        if n % i == 0 {
            factors.push(i);
            if i != n / i {
                factors.push(n / i);
            }
        }
        i += 1;
    }
    factors.sort_unstable();
    factors
}

// --- Custom Fast I/O Scanner ---
#[derive(Default)]
struct Scanner {
    buffer: Vec<String>,
}

impl Scanner {
    fn next<T: FromStr>(&mut self) -> T {
        loop {
            if let Some(token) = self.buffer.pop() {
                return token.parse().ok().expect("Failed parsing");
            }
            let mut input = String::new();
            std::io::stdin().read_line(&mut input).expect("Failed reading");
            self.buffer = input.split_whitespace().rev().map(String::from).collect();
        }
    }
    
    // Read an entire string line
    fn next_line(&mut self) -> String {
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).expect("Failed reading");
        input.trim().to_string()
    }
}

// --- LRU Cache (Array-Backed for CP Safety) ---
// Rust avoids raw pointers; an index-based doubly linked list inside a Vec is the 
// fastest and safest way to implement complex graphs/lists in competitive programming.
struct LRUCache<K, V> {
    capacity: usize,
    cache: HashMap<K, usize>,
    nodes: Vec<Option<(K, V, usize, usize)>>, // (key, value, prev, next)
    head: usize,
    tail: usize,
    free_list: Vec<usize>,
}

impl<K: Clone + std::hash::Hash + Eq, V: Clone> LRUCache<K, V> {
    fn new(capacity: usize) -> Self {
        let mut nodes = Vec::with_capacity(capacity + 2);
        nodes.push(None); // Index 0: Head dummy
        nodes.push(None); // Index 1: Tail dummy
        
        Self {
            capacity,
            cache: HashMap::new(),
            nodes,
            head: 0,
            tail: 1,
            free_list: Vec::new(),
        }
    }

    fn remove_node(&mut self, idx: usize) {
        if let Some((_, _, prev, next)) = self.nodes[idx].clone() {
            if let Some(p) = &mut self.nodes[prev] { p.3 = next; }
            if let Some(n) = &mut self.nodes[next] { n.2 = prev; }
        }
    }

    fn add_to_head(&mut self, idx: usize) {
        let next_idx = if let Some(h) = &self.nodes[self.head] { h.3 } else { self.tail };
        
        if let Some(node) = &mut self.nodes[idx] {
            node.2 = self.head;
            node.3 = next_idx;
        }
        
        if self.nodes[self.head].is_none() {
            self.nodes[self.head] = Some((unsafe { std::mem::zeroed() }, unsafe { std::mem::zeroed() }, 0, idx));
        } else if let Some(h) = &mut self.nodes[self.head] {
            h.3 = idx;
        }
        
        if self.nodes[next_idx].is_none() && next_idx == self.tail {
            self.nodes[self.tail] = Some((unsafe { std::mem::zeroed() }, unsafe { std::mem::zeroed() }, idx, 0));
        } else if let Some(n) = &mut self.nodes[next_idx] {
            n.2 = idx;
        }
    }

    fn get(&mut self, key: &K) -> Option<V> {
        if let Some(&idx) = self.cache.get(key) {
            self.remove_node(idx);
            self.add_to_head(idx);
            return self.nodes[idx].as_ref().map(|node| node.1.clone());
        }
        None
    }

    fn put(&mut self, key: K, value: V) {
        if let Some(&idx) = self.cache.get(&key) {
            if let Some(node) = &mut self.nodes[idx] { node.1 = value; }
            self.remove_node(idx);
            self.add_to_head(idx);
        } else {
            let new_idx = if let Some(free_idx) = self.free_list.pop() {
                self.nodes[free_idx] = Some((key.clone(), value, 0, 0));
                free_idx
            } else {
                self.nodes.push(Some((key.clone(), value, 0, 0)));
                self.nodes.len() - 1
            };

            self.cache.insert(key.clone(), new_idx);
            self.add_to_head(new_idx);

            if self.cache.len() > self.capacity {
                let tail_prev = self.nodes[self.tail].as_ref().unwrap().2;
                if let Some(node) = &self.nodes[tail_prev] {
                    self.cache.remove(&node.0);
                }
                self.remove_node(tail_prev);
                self.nodes[tail_prev] = None;
                self.free_list.push(tail_prev);
            }
        }
    }
}


fn solve(scan: &mut Scanner, out: &mut BufWriter<io::StdoutLock>) {
    // Read variables: let n = scan.next::<usize>();
    // ---------------+------------+------------------+------------------+
    //  THAT PENGUIN  |     __     | IS GOING PLACES  |      __,_____    |
    //  THAT PENGUIN  |    ( o>    | IS GOING PLACES  |     / __.==--"   |
    //  THAT PENGUIN  |    ///\    | IS GOING PLACES  |    /#(-'         |
    //  THAT PENGUIN  |    \V_/_   | IS GOING PLACES  |    `-'           |
    //  THAT PENGUIN  |            | IS GOING PLACES  |   FUCK FOOBAR    |
    // ---------------+------------+------------------+------------------+

    // ------------------------------------------------------------------+
    // PENGUIN TOLD TO USE: writeln!(out, "", ...).unwrap();             |
    // PENGUIN TOLD TO USE: let input = scan.next::<type>();             |
    // ------------------------------------------------------------------+

    // ------------------------------------------------------------------+
    //                                     ______                        |
    //                  |\_______________ (_____\\______________         |
    //          HH======#H###############H#######################        |
    //                  ' ~""""""""""""""`##(_))#H\"""""Y########        |
    //          FUCK FOOBAR               ))    \#H\      `"Y###         |
    //                                    "      }#H)                    |
    //                                                                   |
    //-------------------------------------------------------------------+

}

fn main() {
    let mut scan = Scanner::default();
    let stdout = io::stdout();
    let mut out = BufWriter::new(stdout.lock());

    let nTestCases = scan.next::<usize>();
    
    for _ in 0..nTestCases {
        solve(&mut scan, &mut out);
    }
}