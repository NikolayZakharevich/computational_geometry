#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct point {
    int x, y;

    point(int x, int y) : x(x), y(y) {}

    bool operator==(const point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const point &rhs) const {
        return !(*this == rhs);
    }
};

ll turn_det(const point &a, const point &b, const point &c) {
    return (ll) (b.x - a.x) * (c.y - a.y) - (ll) (b.y - a.y) * (c.x - a.x);
}

void quick_hull_rec(const point &l, const point &r, const vector<point> &points, vector<point> &hull) {
    if (points.empty()) {
        hull.push_back(r);
        return;
    }

    auto max_it = max_element(points.begin(), points.end(), [&l, &r](const point &lhs, const point &rhs) {
        return turn_det(lhs, l, r) < turn_det(rhs, l, r);
    });

    vector<point> left_points;
    vector<point> right_points;

    for (const auto &point: points) {
        if (turn_det(point, l, *max_it) > 0) {
            left_points.push_back(point);
        }
        if (turn_det(point, *max_it, r) > 0) {
            right_points.push_back(point);
        }
    }
    quick_hull_rec(l, *max_it, left_points, hull);
    quick_hull_rec(*max_it, r, right_points, hull);
}

tuple<vector<point>, vector<point>> quick_hull(const vector<point> &a) {
    vector<point> up_hull;
    vector<point> down_hull;

    vector<point> upper_points;
    vector<point> lower_points;

    for (const auto &point: a) {
        auto det = turn_det(point, a.front(), a.back());
        if (det > 0) {
            upper_points.push_back(point);
        } else if (det < 0) {
            lower_points.push_back(point);
        }
    }

    quick_hull_rec(a.front(), a.back(), upper_points, up_hull);
    quick_hull_rec(a.back(), a.front(), lower_points, down_hull);
    return {up_hull, down_hull};
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

    const auto &[up, down] = quick_hull(a);
    cout << down.size() + up.size() << "\n";
    auto print = [](const point &p) { cout << p.x << " " << p.y << "\n"; };
    for_each(down.rbegin(), down.rend(), print);
    for_each(up.rbegin(), up.rend(), print);
    return 0;
}