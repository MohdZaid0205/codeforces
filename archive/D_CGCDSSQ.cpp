#pragma region START

#pragma region Includes
    #include <iostream>
    #include <iomanip>
    #include <cstdio>

    #include <string>
    #include <cstring>

    #include <vector>
    #include <deque>
    #include <list>
    #include <set>
    #include <map>
    #include <unordered_set>
    #include <unordered_map>
    #include <stack>
    #include <queue>

    #include <algorithm>
    #include <numeric>
    #include <cmath>

    #include <utility>
    #include <tuple>
    #include <functional>
    #include <bitset>
    #include <cassert>

    #include <chrono>
#pragma endregion
#pragma region Constants
    const int MOD = 1e9 + 7;
#pragma endregion
#pragma region Predefiend Structures
    struct Point {
        long long x, y;    
        Point operator+ (const Point& a) const { return {x + a.x, y + a.y}; }
        Point operator- (const Point& a) const { return {x - a.x, y - a.y}; }
        bool  operator==(const Point& a) const { return x == a.x && y == a.y; }
    };
    struct custom_hash {
        static uint64_t splitmix64(uint64_t x) {
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
            static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now()
                .time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }
    };
    struct mint {
        int val;
        mint(long long v = 0) {
            if (v < 0) v = v % MOD + MOD;
            if (v >= MOD) v %= MOD;
            val = v;
        }
        mint& operator+=(const mint& other) { val += other.val; if (val >= MOD) val -= MOD; return *this; }
        mint& operator-=(const mint& other) { val -= other.val; if (val < 0) val += MOD; return *this; }
        mint& operator*=(const mint& other) { val = (long long)val * other.val % MOD; return *this; }
        
        friend mint operator+(mint a, const mint& b) { return a += b; }
        friend mint operator-(mint a, const mint& b) { return a -= b; }
        friend mint operator*(mint a, const mint& b) { return a *= b; }
    };
#pragma endregion
#pragma region Utility Functions
    template <typename T> T gcd(T a, T b) { return b == 0 ? a : gcd(b, a % b); }
    template <typename T> T lcm(T a, T b) { return (a / gcd(a, b)) * b; /*::*/ }
    template <typename T, typename... Args> T gcd(T a, Args... args) { return gcd(a, gcd(args...)); }
    template <typename T, typename... Args> T lcm(T a, Args... args) { return lcm(a, lcm(args...)); }
    template<class T>  bool chmin(T& a, const T& b) { 
        if (b < a) { a = b; return true; }
        return false; 
    }
    template<class T>  bool chmax(T& a, const T& b) { 
        if (a < b) { a = b; return true; }
        return false; 
    }
    template <typename T> T vmin(T a, T b) { return std::min(a, b); }
    template <typename T> T vmax(T a, T b) { return std::max(a, b); }
    template <typename T, typename... Args> T vmin(T a, Args... args) { return std::min(a, vmin(args...)); }
    template <typename T, typename... Args> T vmax(T a, Args... args) { return std::max(a, vmax(args...)); }
    template <typename T> std::vector<T> getFactors(T n) {
        static_assert(std::is_integral<T>::value, "Template type must be an integral type.");
        std::vector<T> factors;
        if (n <= 0) return factors; 
        for (T i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                factors.push_back(i);
                if (i != n / i) {
                    factors.push_back(n / i);
                }
            }
        }
        return factors;
    }
    #pragma region Print Utility Functions
        void println() { std::cout << "\n"; }
        template <typename T, typename... Args> 
        void println(T a, Args... args) { 
            std::cout << a << " "; 
            println(args...); 
        }

        void print() { std::cout << ""; }
        template <typename T, typename... Args> 
        void print(T a, Args... args) { 
            std::cout << a << " "; 
            print(args...); 
        }

        void nline() {std::cout << "\n"; }
    #pragma endregion
    #pragma region String Utility Functions
        std::string to_lower(std::string s) {
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
        std::string to_upper(std::string s) {
            transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        }
    #pragma endregion

    template <typename K, typename V> class LRUCache {
    private:
        struct Node {
            K key;
            V val;
            Node* prev;
            Node* next;
            Node(K k, V v) : key(k), val(v), prev(nullptr), next(nullptr) {}
        };

        int capacity;
        std::unordered_map<K, Node*> cache;
        
        Node* head;
        Node* tail;

        void removeNode(Node* node) {
            Node* prevNode = node->prev;
            Node* nextNode = node->next;
            prevNode->next = nextNode;
            nextNode->prev = prevNode;
        }

        void addNodeToHead(Node* node) {
            node->prev = head;
            node->next = head->next;
            head->next->prev = node;
            head->next = node;
        }

        void moveToHead(Node* node) {
            removeNode(node);
            addNodeToHead(node);
        }

        Node* popTail() {
            Node* res = tail->prev;
            removeNode(res);
            return res;
        }

    public:
        LRUCache(int cap) {
            capacity = cap;
            head = new Node(K(), V());
            tail = new Node(K(), V());
            head->next = tail;
            tail->prev = head;
        }

        bool get(const K& key, V& value) {
            if (cache.find(key) == cache.end()) {
                return false;
            }
            Node* node = cache[key];
            moveToHead(node);
            value = node->val;
            return true;
        }

        void put(const K& key, const V& value) {
            if (cache.find(key) != cache.end()) {
                Node* node = cache[key];
                node->val = value;
                moveToHead(node);
            } else {
                Node* newNode = new Node(key, value);
                cache[key] = newNode;
                addNodeToHead(newNode);

                if (cache.size() > capacity) {
                    Node* tailNode = popTail();
                    cache.erase(tailNode->key);
                    delete tailNode;
                }
            }
        }
        
        ~LRUCache() {
            Node* curr = head;
            while (curr != nullptr) {
                Node* nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
        }
    };

#pragma endregion

int main() {
    std::ios_base::sync_with_stdio(false);
#pragma endregion
// ------------------------------------------------------------------+
//     _N\        |  I'M BATMAN       ._M_.       |       ,//)       |
//    /_ |        |  I'M BATMAN      (     )      |     ,;;' \       |
//    B_||.       |  I'M BATMAN       `'V`'       |   ,;;' ( '\      |
//    /_ | '.     |  I'M BATMAN      FAT MAN      |_-'    / '\_)     |
//    0_ m.,,;    |  I'M BATMAN      FAT MAN      |       )          |
//    //_|        |  I'M BATMAN      FAT MAN      |       \          |
// ------------------------------------------------------------------+



// ------------------------------------------------------------------+
//                                                                   |
//          .                                                        |
//         /| ________________________________________________       |
//   O|===|* >________________________________________________Y      |
//         \|                                                        |
//          '                     TIME LIMIT EXCEEDED                |
//                                                                   |
// ------------------------------------------------------------------+
#pragma region  tests
    std::cin.tie(NULL);

    int t = 1;
    // std::cin >> t;
    while (t--) {
#pragma endregion
// ---------------+------------+------------------+------------------+
//  THAT PENGUIN  |     __     | IS GOING PLACES  |      __,_____    |
//  THAT PENGUIN  |    ( o>    | IS GOING PLACES  |     / __.==--"   |
//  THAT PENGUIN  |    ///\    | IS GOING PLACES  |    /#(-'         |
//  THAT PENGUIN  |    \V_/_   | IS GOING PLACES  |    `-'           | 
//  THAT PENGUIN  |            | IS GOING PLACES  |   FUCK FOOBAR    |
// ---------------+------------+------------------+------------------+

long long int n;
std::cin >> n;

std::vector<long long int> arr = std::vector<long long int>(n);
for (long long int i = 0; i < n; i++){ std::cin >> arr[i]; }

long long int q;
std::cin >> q;

std::vector<std::vector<long long int>> sparse;

for (long long int i = 0; i < log2l(n) + 1; i++){
    sparse.push_back(std::vector<long long int>(n, 0LL));
    // for (long long int j = 0; j < n; j ++){
    //     sparse.back().push_back();
    // }
}

sparse[0] = arr;
for (long long int i = 1; i < log2l(n) + 1; i++){
    for (long long int j = 0; j + (1 << (i - 1)) < n; j++){
        sparse[i][j] = gcd(
            sparse[i-1][j], 
            sparse[i-1][j + (1 << (i - 1))]
        );
    }  
}

// for (auto& v:sparse){
//     for (auto& e:v) {
//         print(e);
//     }
//     nline();
// }
// nline();

auto query_gcd = [&](long long L, long long R) {
    long long k = log2l(R - L + 1);
    return gcd(sparse[k][L], sparse[k][R - (1 << k) + 1]);
};

std::map<long long int, long long int> cached;

for (long long int i = 0; i < n; i++) {
    long long int r = i;
    
    while (r < n) {
        long long int g = query_gcd(i, r);
        
        long long int l = r;
        long long int h = n - 1;
        long long int rr = r;
        
        while (l <= h) {
            long long int mid = l + (h - l) / 2;
            
            if (query_gcd(i, mid) == g) {
                rr = mid;
                l = mid + 1;
            } else {
                h = mid - 1;
            }
        }
        
        long long d = rr - r + 1;
        cached[g] += d;
        
        r = rr + 1;
    }
}

for (long long int i = 0; i < q; i++) {
    long long int x;
    std::cin >> x;

    if (cached.count(x)) {
        println(cached[x]);
    } else {
        println(0);
    }
}

// ------------------------------------------------------------------+
//                                     ______                        |
//                  |\_______________ (_____\\______________         |
//          HH======#H###############H#######################        |
//                  ' ~""""""""""""""`##(_))#H\"""""Y########        |
//                                    ))    \#H\       `"Y###        |
//          FUCK FOOBAR               "      }#H)                    |
//                                                                   |
//-------------------------------------------------------------------+
#pragma region END
}

return 0;
}
#pragma endregion