#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct plane;

ll det(const plane &x, const plane &y);

ll product(const plane &x, const plane &y);

struct point {
    double x, y;

    point(double x, double y) : x(x), y(y) {}
};

struct plane {
    ll a, b, c;
    int idx;

    plane() = default;

    plane(ll a, ll b, ll c, int num) : a(a), b(b), c(c), idx(num) {}

    bool operator==(const plane &rhs) const {
        return !det(plane(a, b, c, idx), rhs) && product(plane(a, b, c, idx), rhs) > 0;
    }

    bool operator!=(const plane &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const plane &rhs) const {
        return atan2((double) -a, b) < atan2((double) -rhs.a, rhs.b);
    }
};

ll det(const plane &x, const plane &y) {
    return x.a * y.b - y.a * x.b;
}

ll product(const plane &x, const plane &y) {
    return x.a * y.a + x.b * y.b;
}

auto turn_det(const plane &x, const plane &y, const plane &z) {
    return x.a * y.b * z.c - x.c * y.b * z.a + x.c * y.a * z.b - x.b * y.a * z.c + x.b * y.c * z.a - x.a * y.c * z.b;
}

bool left_turn(const plane &x, const plane &y, const plane &z) {
    return turn_det(x, y, z) > 0;
}

auto min(const plane &x, const plane &y) {
    if (x < y) return x;
    if ((y.b && x.c * y.b == y.c * x.b) || (!y.b && (x.c * y.a == x.a * y.c))) return x.idx < y.idx ? x : y;
    auto p = x.b != 0 ? point{.0, (double) -x.c / x.b} : point{(double) -x.c / x.a, .0};
    return y.a * p.x + y.b * p.y + y.c > 0 ? x : y;
}

bool isZero(const plane &x, const plane &y) {
    if (det(x, y) || product(x, y) >= 0) return false;
    auto p = y.a != 0 ? point{(double) -y.c / y.a, .0} : point{.0, (double) -y.c / y.b};
    return (x.a * p.x + x.b * p.y + x.c) <= 0;
}

bool isINF(const plane &x, const plane &y) {
    if (det(x, y)) return false;
    auto p = y.a != 0 ? point{(double) -y.c / y.a, .0} : point{.0, (double) -y.c / y.b};
    return (x.a * p.x + x.b * p.y + x.c) > 0;
}


auto inc(int i, int mod) {
    return (i + 1) % mod;
}

double square(const vector<plane> &intersected) {
    auto n = intersected.size();
    if (n == 0) return .0;
    if (n == 1) return -1.0;

    vector<point> edges;
    for (int idx = 0; idx < n; idx++) {
        auto curr = intersected[idx];
        auto next = intersected[inc(idx, n)];
        if (isZero(curr, next)) return .0;
        double d = det(curr, next);
        edges.emplace_back(-(curr.c * next.b - next.c * curr.b) / d, -(curr.a * next.c - next.a * curr.c) / d);
    }

    double res = .0;
    point prev = edges.back();
    for (const auto &curr: edges) {
        res += (prev.x - curr.x) * (prev.y + curr.y);
        prev = curr;
    }
    return fabs(res / 2);
}

void exitInf() {
    cout << -1;
    exit(0);
}

void exitZero() {
    cout << 0;
    exit(0);
}

int findStart(const vector<plane> &hull) {
    unordered_set<int> used;
    for (auto &p: hull) {
        if (used.find(p.idx) != used.end()) {
            return p.idx;
        }
        used.insert(p.idx);
    }
    return -1;
}

auto buildIntersected(const vector<plane> &unique) {
    vector<plane> intersected, hull{unique.front(), unique[1]};

    for (int i = 2; i < unique.size(); i++) {
        while (hull.size() >= 2 && !left_turn(unique[i], hull[hull.size() - 2], hull.back())) {
            hull.pop_back();
        }
        hull.push_back(unique[i]);
    }

    auto n = hull.size();

    plane prev = hull.back(), curr;
    for (int i = 0; i < n; i++) {
        curr = hull[i];
        if (isZero(prev, curr)) exitZero();
        if (isINF(prev, curr)) exitInf();
        prev = curr;
    }

    int start = findStart(hull);
    if (start == -1) exitZero();

    int idx = -1;
    while (start != hull[++idx].idx) {}

    intersected.push_back(hull[idx]);
    idx = inc(idx, n);
    while (hull[idx].idx != start) {
        intersected.push_back(hull[idx]);
        idx = inc(idx, n);
    }

    return intersected;
}

auto checkAndBuildIntersection(vector<plane> &planes) {
    sort(planes.begin(), planes.end(), [](const plane &x, const plane &y) { return x != y ? x < y : x.c < y.c; });

    vector<plane> unique;
    plane nextUnique = planes.front(), prevPlane, currPlane = planes.front();
    for (int i = 1; i <= planes.size(); i++) {
        prevPlane = currPlane;
        bool last = i == planes.size();
        if (last && unique.empty()) exitInf();
        currPlane = !last ? planes[i] : planes.front();

        if (currPlane == prevPlane) {
            nextUnique = min(nextUnique, currPlane);
        } else {
            unique.push_back(nextUnique);
            nextUnique = currPlane;
        }
    }
    unique.front() = nextUnique;

    auto n = unique.size();
    if (n <= 1) exitInf();
    for (int idx = 0; idx < n; idx++) {
        auto curr = unique[idx];
        auto next = unique[inc(idx, n)];
        if (det(curr, next) < 0) exitInf();
        if (isZero(curr, next) && !det(curr, next) && (product(curr, next) < 0)) exitZero();
    }

    for (int i = 0; i < n; i++) {
        unique.emplace_back(unique[i].a, unique[i].b, unique[i].c, unique[i].idx);
    }
    return buildIntersected(unique);
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
    auto sq = square(checkAndBuildIntersection(planes));
    if (sq < 0) exitInf();
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