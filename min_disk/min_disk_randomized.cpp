#include <random>

#include <bits/stdc++.h>
#include <ostream>

using namespace std;

typedef long long ll;

const long double EPS = 1e-9;

struct point {
    long double x, y;

    point(long double x, long double y) : x(x), y(y) {}
};

auto dist(const point &a, const point &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

struct disk {
    point center;
    long double radius;

    disk(const point &center, long double radius) : center(center), radius(radius) {}
};

bool is_inside(const disk &d, const point &p) {
    return dist(d.center, p) <= d.radius;
}

auto get_disk(const point &p1, const point &p2) {
    auto c = point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    return disk(c, dist(p1, c));
}

auto center(const point &a, const point &b, const point &c) {
    auto aNorm = (a.x * a.x + a.y * a.y);
    auto bNorm = (b.x * b.x + b.y * b.y);
    auto cNorm = (c.x * c.x + c.y * c.y);
    auto d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    auto x = aNorm * (b.y - c.y) + bNorm * (c.y - a.y) + cNorm * (a.y - b.y);
    auto y = aNorm * (c.x - b.x) + bNorm * (a.x - c.x) + cNorm * (b.x - a.x);
    return point(x / d, y / d);
}

auto square(const point &a, const point &b, const point &c) {
    point va = {b.x - a.x, b.y - a.y};
    point vb = {c.x - a.x, c.y - a.y};
    return fabs(va.x * vb.y - va.y * vb.x) / 2;
}

auto radius(const point &a, const point &b, const point &c) {
    auto sq = square(a, b, c);
    if (sq < EPS) return -1.0l;
    return dist(a, b) * dist(b, c) * dist(a, c) / (4.0 * sq);
}

auto min_disk_with_2_points(vector<point> &points, int idx1, int idx2) {
    auto d = get_disk(points[idx1], points[idx2]);
    for (int i = 0; i < idx1; i++) {
        if (is_inside(d, points[i])) continue;
        auto r = radius(points[i], points[idx1], points[idx2]);
        if (r < EPS) continue;
        d = disk(center(points[i], points[idx1], points[idx2]), r);
    }
    return d;
}

auto min_disk_with_point(vector<point> &points, int idx) {
    shuffle(points.begin(), points.begin() + idx, std::mt19937(std::random_device()()));

    auto d = get_disk(points[0], points[idx]);
    for (int i = 1; i < idx; i++) {
        if (is_inside(d, points[i])) continue;
        d = min_disk_with_2_points(points, i, idx);
    }
    return d;
}

auto min_disk(vector<point> &points) {
    if (points.size() == 1) return disk(points[0], 0);
    shuffle(points.begin(), points.end(), std::mt19937(std::random_device()()));

    auto d = get_disk(points[0], points[1]);
    for (int i = 2; i < points.size(); i++) {
        if (is_inside(d, points[i])) continue;
        d = min_disk_with_point(points, i);
    }
    return d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    size_t n;
    cin >> n;
    vector<point> a;
    a.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        a.emplace_back(x, y);
    }
    auto d = min_disk(a);
    cout << setprecision(20) << fixed << d.radius << "\n" << d.center.x << " " << d.center.y;
    return 0;
}


/**

 4
-1 -1
-1 1
1 1
1 -1

6
0 0
0 1
0 2
0 3
0 4
0 5

4
1 0
0 1
1 2
2 1
 */