#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdint>

using namespace std;


// programmers compile flag: clang++ -std=c++17 -O2 -Wno-unused-result -o FILENAME
long long mp[200003];
uint32_t rout[200003], rmp[200003], pos[200003];

inline uint32_t Find(uint32_t r) {
    uint32_t jt = 1;
    rout[0] = r;
    
    while (const uint32_t x = pos[r]) {
        __builtin_prefetch(&pos[x], 0, 3);
        rout[jt++] = x;
        r = x;
    }
    
    ++r;
    uint32_t i=0;
    for (; i+4 < jt; i+=4) {
        __builtin_prefetch(&pos[rout[i+4]], 1, 3);
        pos[rout[i]] = pos[rout[i+1]] = pos[rout[i+2]] = pos[rout[i+3]] = r;
    }
    for(; i < jt; ++i) {
        pos[rout[i]] = r;
    }
    
    return r-1;
}

/*
inline size_t fast_lower_bound(const long long* arr, size_t n, const long long target) {
    size_t base = 0;

    if (base + 131072 < n) base = (arr[base + 131072] < target) ? base + 131072: base;
    if (base + 65536 < n) base = (arr[base + 65536] < target) ? base + 65536 : base;
    if (base + 32768 < n) base = (arr[base + 32768] < target) ? base + 32768 : base;
    if (base + 16384 < n) base = (arr[base + 16384] < target) ? base + 16384 : base;
    if (base + 8192 < n) base = (arr[base + 8192] < target) ? base + 8192 : base;
    if (base + 4096 < n) base = (arr[base + 4096] < target) ? base + 4096 : base;
    if (base + 2048 < n) base = (arr[base + 2048] < target) ? base + 2048 : base;
    if (base + 1024 < n) base = (arr[base + 1024] < target) ? base + 1024 : base;
    if (base + 512 < n) base = (arr[base + 512] < target) ? base + 512 : base;
    if (base + 256 < n) base = (arr[base + 256] < target) ? base + 256 : base;
    if (base + 128 < n) base = (arr[base + 128] < target) ? base + 128 : base;
    if (base + 64 < n) base = (arr[base + 64] < target) ? base + 64 : base;
    if (base + 32 < n) base = (arr[base + 32] < target) ? base + 32 : base;
    if (base + 16 < n) base = (arr[base + 16] < target) ? base + 16 : base;
    if (base + 8 < n) base = (arr[base + 8] < target) ? base + 8 : base;
    if (base + 4 < n) base = (arr[base + 4] < target) ? base + 4 : base;
    if (base + 2 < n) base = (arr[base + 2] < target) ? base + 2 : base;
    if (base + 1 < n) base = (arr[base + 1] < target) ? base + 1 : base;
    return base + (arr[base] < target);
}
 TO SLOW!!
*/

struct Node {
    long long val;
    uint32_t idx;
    uint64_t pad;
    Node(long long val=0, uint32_t idx=0):val(val),idx(idx),pad((val << 20)|idx){}
    const bool operator<(const Node &rhs) const {
        return pad < rhs.pad;
    }
};

Node tarr[200003];
void radixSort(Node* a, Node* b, const uint32_t n) {
    uint32_t count[65536];
    
    memset(count, 0, sizeof(count));
    for (int i=0; i<n; ++i) ++count[a[i].pad & 0xFFFF];
    for (int i=1; i<65536; ++i) count[i] += count[i-1];
    for (int i= n-1; i >= 0; --i) b[--count[a[i].pad & 0xFFFF]] = a[i];
    
    memset(count, 0, sizeof(count));
    for (int i=0; i<n; ++i) ++count[(b[i].pad>>16) & 0xFFFF];
    for (int i=1; i<65536; ++i) count[i] += count[i-1];
    for (int i= n-1; i >= 0; --i) a[--count[(b[i].pad>>16) & 0xFFFF]] = b[i];
    
    memset(count, 0, sizeof(count));
    for (int i=0; i<n; ++i) ++count[(a[i].pad>>32) & 0xFFFF];
    for (int i=1; i<65536; ++i) count[i] += count[i-1];
    for (int i= n-1; i >= 0; --i) b[--count[(a[i].pad>>32) & 0xFFFF]] = a[i];
    
    memset(count, 0, sizeof(count));
    for (int i=0; i<n; ++i) ++count[(b[i].pad>>48) & 0xFFFF];
    for (int i=1; i<65536; ++i) count[i] += count[i-1];
    for (int i= n-1; i >= 0; --i) a[--count[(b[i].pad>>48) & 0xFFFF]] = b[i];
}

Node arr[200003];
vector<long long> solution(long long k, vector<long long> room_number) {
    const uint32_t n = static_cast<uint32_t>(room_number.size());
    vector<long long> answer; answer.reserve(n);
    
    for (uint32_t i = 0; i < n; ++i) {
        arr[i] = Node{room_number[i], i};
    }
    
    (void)(n < 65536 ? sort(arr, arr+n) : radixSort(arr, tarr, n));
    
    long long mx = -1e9;
    uint32_t it = 0;
    
    for (uint32_t i=0; i<n; ++i) {
        const long long a = arr[i].val;
        mx = mx < a ? a : mx + 1;
        mp[i] = mx;
        while (a > mp[it]) ++it;
        rmp[arr[i].idx] = it;
    }
    
    
    memset(pos, 0, sizeof(uint32_t) * (n + 3));
    
    for (uint32_t i = 0; i + 1 < n; ++i) {
        __builtin_prefetch(&pos[rmp[i+1]], 0, 3);
        uint32_t cr = rmp[i]; //fast_lower_bound(mp, n, room);
        uint32_t pcr = Find(cr);
        answer.push_back(mp[pcr]);
    }
    {
        uint32_t cr = rmp[n-1];
        uint32_t pcr = Find(cr);
        answer.push_back(mp[pcr]);
    }
    return answer;
}

#include <iostream>
#include <random>
#include <chrono>

int main() {
    vector<long long> brr = {1,3,4,1,3,1};
    cout<<"TEST 1\n";
    auto xxx = solution(10, brr);
    for(auto x:xxx)cout<<x<<" ";
    cout<<endl;
    cout<<"--------------\n";
    random_device rd;
    vector<long long> arr(100000);
    mt19937_64 gen(rd());
    for(auto &a:arr) {
        a = abs((long long)(gen()));
    }
    cout<<"TEST 2  start!\n";
    auto start = std::chrono::steady_clock::now();
    auto ans = solution(10, arr);
    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout<<"Time: "<< (diff.count())<<"ms"<<endl;
    
    return EXIT_SUCCESS;
}
