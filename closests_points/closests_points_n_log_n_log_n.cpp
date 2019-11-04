#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

int ansI = 0;
int ansJ = 0;

const ll INF = LONG_LONG_MAX;
ll min_dist = INF;

struct point {
    ll x, y;
};


ll dist(const point &a, const point &b) {
    ll dx = (b.x - a.x);
    ll dy = (b.y - a.y);
    return dx * dx + dy * dy;
}

ll merge(vector<point> &a, int l, int m, int r) {
    int l_start = m - 1;
    int r_end = m;
    while (l_start > l && (a[m].x - a[l_start - 1].x) * (a[m].x - a[l_start - 1].x) < min_dist) {
        l_start--;
    }
    while (r_end < r && (a[r_end].x - a[m].x) * (a[r_end].x - a[m].x) < min_dist) {
        r_end++;
    }
    map<ll, int> l_y;
    map<ll, int> r_y;
    for (int i = l_start; i < m; i++) {
        l_y[a[i].y] = i;
    }
    for (int i = m; i < r_end; i++) {
        r_y[a[i].y] = i;
    }
    auto l_it = l_y.begin();
    auto r_it = r_y.begin();

    while (l_it != l_y.end() && r_it != r_y.end()) {
        ll cur = dist(a[l_it->second], a[r_it->second]);
        if (cur < min_dist) {
            ansI = l_it->second;
            ansJ = r_it->second;
            min_dist = cur;
        }
        r_it++;
        if (r_it == r_y.end() || r_it->first - l_it->first > min_dist) {
            l_it++;
            r_it = r_y.lower_bound(l_it->first - min_dist);
        }
    }
    return min_dist;
}

ll merge_sort(vector<point> &a, int l, int r) {
    if (l >= r - 1) {
        return INF;
    }
    int m = (l + r) >> 1;
    merge_sort(a, l, m);
    merge_sort(a, m, r);
    return merge(a, l, m, r);
}

int main() {
    int n;
    cin >> n;
    vector<point> a;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        a.push_back({x, y});
    }
    sort(a.begin(), a.end(), [](const point &a, const point &b) { return a.x < b.x; });
    merge_sort(a, 0, n);
    cout << a[ansI].x << " " << a[ansI].y << "\n" << a[ansJ].x << " " << a[ansJ].y << "\n";

    return 0;
}

/**
4
0 0
0 1
1 1
1 0

4
-100 100
-50 50
30 30
20 -20

5
0 0
3 0
9 0
10 0
16 0

3
-1000000000 -1000000000
0 0
1000000000 1000000000


8
0 0
1 1
2 2
3 3
3 4
4 4
5 5
7 1
*/