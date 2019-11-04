#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct point {
    int x, y;
    bool operator==(const point &rhs) const;
};

ll turn_det(const point &a, const point &b, const point &c) {
    return (ll)(b.x - a.x) * (c.y - a.y) - (ll)(b.y - a.y) * (c.x - a.x);
}

bool point::operator==(const point &rhs) const {
    return x == rhs.x && y == rhs.y;
}

int main() {
    size_t n;
    cin >> n;
    vector<point> a;
    a.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        a.emplace_back(x, y);
    }
    sort(a.begin(), a.end(), [](const point &a, const point &b) { return a.x != b.x ? a.x < b.x : a.y < b.y; });
    a.erase(unique(a.begin(), a.end()), a.end());
    n = a.size();
    auto print = [](const point &p) { cout << p.x << " " << p.y << "\n"; };

    auto l = a.front();
    auto r = a.back();
    vector<point> up = {l};
    vector<point> down = {l};
    for (int i = 1; i < n; i++) {
        auto turn = turn_det(l, r, a[i]);
        if (i == n - 1 || turn > 0) {
            while (up.size() > 1 && turn_det(up[up.size() - 2], up.back(), a[i]) >= 0) {
                up.pop_back();
            }
            up.push_back(a[i]);
        }
        if (i == n - 1 || turn < 0) {
            while (down.size() > 1 && turn_det(down[down.size() - 2], down.back(), a[i]) <= 0) {
                down.pop_back();
            }
            down.push_back(a[i]);
        }
    }
    if (up.back() == down.back()) {
        down.pop_back();
    }
    cout << (down.size() + up.size() - 1) << "\n";
    for_each(down.begin(), down.end(), print);
    for_each(up.rbegin(), up.rend() - 1, print);
    return 0;
}

/**
4
0 0
0 1
1 1
1 0

7
0 0
0 1
1 0
0 0
1 1
1 0
1 0

5
0 0
2 0
0 2
1 1
2 2

3
0 0
0 1
1 1

8
0 0
1 1
2 2
3 3
4 4
5 5
6 6
6 0

8
0 0
1 1
2 2
3 3
4 4
5 5
6 7
6 0


7
1000000000 1000000000
1000000000 1000000000
-1000000000 1000000000
-1000000000 -1000000000
1000000000 1000000000
0 0
-1000000000 -1000000000

12
0 0
0 1
0 2
0 3
3 3
2 2
1 1
0 4
1 4
2 4
3 4
4 4

12
0 0
1 0
2 0
3 0
3 3
2 2
1 1
4 0
4 1
4 2
4 3
4 4

5
0 0
1 0
0 1
-1 0
0 -1


7
0 0
0 1
0 2
0 3
0 4
0 5
3 3

4
3 5
0 1
1 0
2 4

6
4 6
2 4
1 2
0 2
1 0
2 3

5
0 0
1 0
0 1
-1 0
0 -1

8  7 7   7 -7   -7 -7   -7 7   9 0   -9 0    0 9    0 -9
*/