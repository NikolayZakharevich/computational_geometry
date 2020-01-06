#include <bits/stdc++.h>
#include <ostream>

using namespace std;

struct point {
    int x, y;

    point(int x, int y) : x(x), y(y) {}

    bool operator==(const point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    friend ostream &operator<<(ostream &os, const point &point1) {
        os << "x: " << point1.x << " y: " << point1.y;
        return os;
    }
};

enum position {
    INSIDE,
    OUTSIDE
};

int left_turn_det(const point &a, const point &b, const point &c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

bool left_turn(const point &a, const point &b, const point &c) {
    return left_turn_det(a, b, c) > 0;
}

position is_inside(const vector<point> &polygon, const point &p) {
    int cnt = 0;
    for (size_t i = 0; i < polygon.size(); i++) {
        auto l = polygon[i];
        auto r = polygon[(i + 1) % polygon.size()];
        if (l.y > r.y) {
            swap(l, r);
        }
        if (r.y < p.y || l.y >= p.y) continue;
        int orient = left_turn_det(l, r, p);
        if (orient == 0) {
            return INSIDE;
        }
        if (orient > 0) {
            cnt++;
        }
    }
    return cnt % 2 ? INSIDE : OUTSIDE;
}

int main() {
    int n, x0 ,y0;
    cin >> n >> x0 >> y0;
    vector<point> polygon;
    bool isReversed = true;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        if (i > 0 && x == polygon.back().x && y == polygon.back().y) continue;
        polygon.emplace_back(x, y);
        if (isReversed && i >= 2 && left_turn(polygon[i - 2], polygon[i - 1], polygon[i])) {
            isReversed = false;
        }
    }
    if (isReversed) {
        reverse(polygon.begin(), polygon.end());
    }

    auto result = is_inside(polygon, point(x0, y0));
    switch (result) {
        case INSIDE:
            cout << "YES\n";
            break;
        case OUTSIDE:
            cout << "NO\n";
            break;
    }
    return 0;
}

/*
4 1 1
0 0
2 0
2 2
0 2
 */