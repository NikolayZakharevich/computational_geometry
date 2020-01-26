#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct plane;

ll det(const plane &x, const plane &y);

ll product(const plane &x, const plane &y);

struct point {
    double x, y;
};

struct plane {
    ll a, b, c;
    int num;

    plane() = default;

    plane(ll a, ll b, ll c, int num) : a(a), b(b), c(c), num(num) {}

    bool operator<(const plane &rhs) const {
        return atan2((double) -a, (double) b) < atan2((double) -rhs.a, (double) rhs.b);
    }

    bool operator==(const plane &rhs) const {
        return det(plane(a, b, c, num), rhs) == 0 && product(plane(a, b, c, num), rhs) > 0;
    }

    bool operator!=(const plane &rhs) const {
        return !(rhs == *this);
    }
};

ll det(const plane &x, const plane &y) {
    return x.a * y.b - y.a * x.b;
}

point intersectionPoint(const plane &x, const plane &y) {
    double d = det(x, y);
    return {-(x.c * y.b - y.c * x.b) / d, -(x.a * y.c - y.a * x.c) / d};
}

ll product(const plane &x, const plane &y) {
    return x.a * y.a + x.b * y.b;
}

ll turn_det(const plane &x, const plane &y, const plane &z) {
    return x.a * y.b * z.c - x.c * y.b * z.a + x.c * y.a * z.b - x.b * y.a * z.c + x.b * y.c * z.a - x.a * y.c * z.b;
}

auto min(const plane &x, const plane &y) {
    if (x < y) return x;
    if ((y.b && x.c * y.b == y.c * x.b) || (!y.b && (x.c * y.a == x.a * y.c))) {
        return x.num < y.num ? x : y;
    }
    auto p = x.b != 0 ?  point{.0, (double) -x.c / x.b} : point{(double) -x.c / x.a, .0};
    return y.a * p.x + y.b * p.y + y.c > 0 ? x : y;
}

bool isZero(const plane &x, const plane &y) {
    if (det(x, y) || product(x, y) >= 0) return false;
    auto p = y.a != 0 ?  point{(double) -y.c / y.a, .0} : point{.0, (double) -y.c / y.b};
    return (x.a * p.x + x.b * p.y + x.c) <= 0;
}

bool isINF(const plane &x, const plane &y) {
    if (det(x, y)) return false;
    auto p = y.a != 0 ? point{(double) -y.c / y.a, .0} : point{.0, (double) -y.c / y.b};
    return (x.a * p.x + x.b * p.y + x.c) > 0;
}

double square(const vector<plane> &intersected) {
    auto n = intersected.size();
    if (n == 0) return .0;
    if (n == 1) return -1.0;

    vector<point> edges;
    for (int idx = 0; idx < n; idx++) {
        int nextIdx = (idx + 1) % n;
        if (isZero(intersected[idx], intersected[nextIdx])) return .0;
        edges.push_back(intersectionPoint(intersected[idx], intersected[nextIdx]));
    }

    double res = .0;
    point prev = edges.back();
    for (const auto &curr: edges) {
        res += (prev.x - curr.x) * (prev.y + curr.y);
        prev = curr;
    }
    return fabs(res / 2);
}


vector<plane> intersected;

int buildIntersected(const vector<plane> &unique, int maxNum) {
    vector<plane> hull{unique.front(), unique[1]};
    vector<char> used(maxNum);

    for (int i = 2; i < unique.size(); i++) {
        while (hull.size() >= 2 && turn_det(unique[i], hull[hull.size() - 2], hull[hull.size() - 1]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(unique[i]);
    }
    int stack = -1;
    for (auto &p : hull) {
        if (used[p.num]) {
            stack = p.num;
            break;
        }
        used[p.num] = 1;
    }

    int n = hull.size();

    for (int i = 0; i < n; i++) {
        auto b = hull[(i + 1) % n];
        if (isINF(hull[i], b)) return -1;
        if (isZero(hull[i], b)) return 0;
    }
    if (stack == -1) return 0;

    for (int i = 0; i < n; i++) {
        if (stack != hull[i].num) continue;
        intersected.push_back(hull[i]);
        i = (i + 1) % n;
        while (stack != hull[i].num) {
            intersected.push_back(hull[i]);
            i = (i + 1) % n;
        }
        break;
    }

    return 1;
}

int checkIntersection(vector<plane> &planes) {
    int maxNum = planes.size();
    sort(planes.begin(), planes.end(), [](const plane &x, const plane &y) { return x != y ? x < y : x.c < y.c; });

    vector<plane> unique;
    plane nextUnique = planes.front(), prev, curr = planes.front();
    for (int i = 1; i <= planes.size(); i++) {
        prev = curr;
        if (i == planes.size()) {
            if (unique.empty()) return -1;
            curr = planes.front();
        } else {
            curr = planes[i];
        }

        if (curr == prev) {
            nextUnique = min(nextUnique, curr);
        } else {
            unique.push_back(nextUnique);
            nextUnique = curr;
        }
    }
    unique.front() = nextUnique;

    auto n = unique.size();
    if (n <= 1) return -1;
    for (int i = 0; i < n; i++) {
        if (det(unique[i], unique[(i + 1) % n]) < 0) return -1;
        auto next = unique[(i + 1) % n];
        if (isZero(unique[i], next) && !det(unique[i], next) && (product(unique[i], next) < 0)) return 0;
    }
    for (int i = 0; i < n; i++) {
        unique.emplace_back(unique[i].a, unique[i].b, unique[i].c, unique[i].num);
    }
    return buildIntersected(unique, maxNum);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<plane> planes;
    size_t n;
    cin >> n;
    planes.reserve(n);
    for (int i = 0; i < n; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        planes.emplace_back(a, b, c, i);
    }
    auto res = checkIntersection(planes);
    if (res == -1 || res == 0) {
        cout << res;
        return 0;
    }
    auto sq = square(intersected);
    if (sq < 0) {
        cout << -1;
        return 0;
    }
    cout << setprecision(20) << fixed << sq;
    return 0;

}

/*

3
0 -1 0
-1 0 0
1 1 2
 -----
2.0000000000

1
1 1 0
 ----
-1

4
1 0 2
-1 0 -2
-1 0 2
1 0 -2
 -----
0

8
0 -1 3
0 1 3
1 0 3
-1 0 3
-1 1 4
-1 -1 4
1 1 4
1 -1 4
 ------
28

8
1 0 1
-1 0 -1
-1 0 1
1 0 -1
0 1 1
0 -1 -1
0 -1 1
0 1 -1
 ------
0

8
0 -1 -3
0 1 -3
1 0 -3
-1 0 -3
-1 1 -4
-1 -1 -4
1 1 -4
1 -1 -4
 -------
0


3
0 -1 0
-1 0 0
3 3 6
 -----
2

8
1 0 4
1 0 2
-1 0 4
-1 0 2
0 1 4
0 1 2
0 -1 4
0 -1 2
 ------
16
 */