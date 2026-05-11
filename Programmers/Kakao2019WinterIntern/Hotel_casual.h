#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>

using namespace std;


// programmers compile flag: clang++ -std=c++17 -O2 -Wno-unused-result -o FILENAME
long long mp[200003];
size_t path[200003], rmp[200003], pos[200003];

inline size_t Find(size_t r) {
    size_t jt = 1;
    path[0] = r;
    
    while (const size_t x = pos[r]) {
        path[jt++] = x;
        r = x;
    }
    
    ++r;
    size_t i=0;
    for (; i+3 < jt; i+=4) {
        pos[path[i]] = pos[path[i+1]] = pos[path[i+2]] = pos[path[i+3]] = r;
    }
    for(; i < jt; ++i) {
        pos[path[i]] = r;
    }
    
    return r-1;
}

vector<long long> solution(long long k, vector<long long> room_number) {
    const size_t n = room_number.size();
    vector<long long> answer; answer.reserve(n);
    vector<pair<long long, long long> > arr(n);
    
    for (size_t i = 0; i < n; ++i) {
        arr[i] = {room_number[i], i};
    }
    sort(arr.begin(), arr.end());
    
    long long mx = -1e9;
    int it = 0, jt = 0;
    
    for (auto [a,i] : arr) {
        mx = mx < a ? a : mx + 1;
        mp[it++] = mx;
        while (a > mp[jt]) ++jt;
        rmp[i] = jt;
    }
    
    memset(pos, 0, sizeof(size_t) * (n + 3));
    
    for (size_t i = 0; i < n; ++i) {
        size_t cr = rmp[i];
        size_t pcr = Find(cr);
        answer.push_back(mp[pcr]);
    }
    
    return answer;
}
