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
    template <typename T, typename... Args> T vmin(T a, Args... args) { return min(a, vmin(args...)); }
    template <typename T, typename... Args> T vmax(T a, Args... args) { return max(a, vmax(args...)); }
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
#pragma region pre-Compute

std::string C12[] = {"12", "21"};
std::string C13[] = {"123", "132", "213", "231", "312", "321"};
std::string C14[] = {
     "1234", "1243", "1324", "1342",
     "1423", "1432", "2134", "2143", 
     "2314", "2341", "2413", "2431", 
     "3124", "3142", "3214", "3241", 
     "3412", "3421", "4123", "4132", 
     "4213", "4231", "4312", "4321"
};

#pragma endregion
#pragma region  tests
    std::cin.tie(NULL);

    int t = 1;
    std::cin >> t;
    while (t--) {
#pragma endregion
// ---------------+------------+------------------+------------------+
//  THAT PENGUIN  |     __     | IS GOING PLACES  |      __,_____    |
//  THAT PENGUIN  |    ( o>    | IS GOING PLACES  |     / __.==--"   |
//  THAT PENGUIN  |    ///\    | IS GOING PLACES  |    /#(-'         |
//  THAT PENGUIN  |    \V_/_   | IS GOING PLACES  |    `-'           | 
//  THAT PENGUIN  |            | IS GOING PLACES  |   FUCK FOOBAR    |
// ---------------+------------+------------------+------------------+

std::string S;
std::cin >> S;

int a, b;
std::cin >> a >> b;

std::string* seq = nullptr;

if (S.size() == 2){
    seq = C12;
} else if (S.size() == 3){
    seq = C13;
} else if (S.size() == 4){
    seq = C14;
}

std::string s1 = seq[a-1];
std::string s2 = seq[b-1];

int x = 0;
int y = S.size();

for (int i = 0; i < S.size(); i++){
    if (s1[i] == s2[i]){
        x ++;
        y --;
    }
}

std::cout << x << "A" << y << "B" << std::endl;

// ------------------------------------------------------------------+
//                                     ______                        |
//                  |\_______________ (_____\\______________         |
//          HH======#H###############H#######################        |
//                  ' ~""""""""""""""`##(_))#H\"""""Y########        |
//          FUCK FOOBAR               ))    \#H\       `"Y###        |
//                                    "      }#H)                    |
//                                                                   |
//-------------------------------------------------------------------+
#pragma region END
}

return 0;
}
#pragma endregion