#include <cstdio>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;

inline
vector<int> foo_1(const int n){
    vector<int> ret;
    ret.reserve(n);
    for (int i = 0; i < n; ++i){
        ret.push_back(n);
    }
    return ret;
}

inline
vector<int> foo_2(const vector<int> a) {
    vector<int> ret;
    ret.resize(a.size());
    for (int i = 0; i < a.size(); ++i){
        ret[a.size() - i - 1] = a[i];
    }
    return ret;
}

inline
long long vec_sum(vector<int> a){
    long long ret = 0;
    for (int i = 0; i < a.size(); ++i){
        ret += a[i];
    }
    return ret;
}

void test_1(const int n, const int m){
    long long sum = 0;
    for (int i = 0; i < m; ++i) {
        long long ans = vec_sum(foo_2(foo_1(n)));
        sum += ans;
    }
    cout << sum << endl;
}

void test_2(const int n, const int m){
    long long sum = 0;
    for (int i = 0; i < m; ++i) {
        vector<int> a;
        a.reserve(n);
        vector<int> b;
        for (int i = 0; i < n; ++i){
            a.push_back(n);
        }
        b.resize(a.size());
        for (int i = 0; i < a.size(); ++i){
            b[a.size() - i - 1] = a[i];
        }
        long long ans = 0;
        for (int i = 0; i < b.size(); ++i) {
            ans += b[i];
        }
        sum += ans;
    }
    cout << sum << endl;
}

using namespace std::chrono;

#define TIME_TEST(foo) \
    do {\
        steady_clock::time_point t1 = steady_clock::now();\
        foo;\
        steady_clock::time_point t2 = steady_clock::now();\
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);\
        cout << "Test: "#foo << ", cost time " << time_span.count() << "s" << endl;\
    } while(false)


int main(){

    const int n = 10000;
    const int m = 10000;
    TIME_TEST(test_2(n, m));
    TIME_TEST(test_1(n, m));

    return 0;
}
