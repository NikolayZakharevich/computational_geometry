#include <bits/stdc++.h>
#include <ostream>

using namespace std;

struct point {
    int x, y;

    point(int x, int y) : x(x), y(y) {}

    bool operator==(const point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

typedef vector<point>::iterator it;
typedef long long ll;

enum position {
    INSIDE,
    BORDER,
    OUTSIDE
};

ll left_turn_det(const point &a, const point &b, const point &c) {
    return (ll)(b.x - a.x) * (c.y - a.y) - (ll)(c.x - a.x) * (b.y - a.y);
}

bool left_turn(const point &a, const point &b, const point &c) {
    return left_turn_det(a, b, c) > 0;
}

auto getCmp(const point &pivot, const point &p) {
    return [&pivot, &p](const point &middle, const point &x) {
        return left_turn(pivot, middle, x);
    };
}

auto format(it begin, it end, bool isReversed) {
    if (isReversed) reverse(begin, end);
    auto pivot_it = min_element(begin, end, [](const point &lhs, const point &rhs) {
        return lhs.x != rhs.x ? lhs.x < rhs.x : lhs.y < rhs.y;
    });
    rotate(begin, pivot_it, end);
}


bool in_triangle(const point &a, const point &b, const point &c, const point &p) {
    return left_turn_det(a, b, p) >= 0 && left_turn_det(b, c, p) >= 0 && left_turn_det(c, a, p) >= 0;
}

position is_inside(const vector<point> &polygon, const point &p) {
    if (p.x < polygon[0].x) return OUTSIDE;
    if (p == polygon[0]) return BORDER;

    auto cmp = getCmp(polygon.front(), p);
    auto it = std::lower_bound(polygon.begin() + 2, polygon.end(), p, cmp);
    if (it == polygon.end()) {
        return OUTSIDE;
    }

    auto a = polygon.front();
    auto b = *(it - 1);
    auto c = *it;

    if (!in_triangle(a, b, c, p)) return OUTSIDE;
    if (left_turn_det(b, c, p) == 0) return BORDER;
    if (b == polygon[1] && left_turn_det(a, b, p) == 0) return BORDER;
    if (c == polygon.back() && left_turn_det(c, a, p) == 0) return BORDER;

    return INSIDE;
}

int main() {
    int n;
    cin >> n;
    vector<point> polygon;
    bool isReversed = true;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        auto p = point(x, y);
        if (i > 0 && polygon.back() == p) continue;
        if (i >= 2) {
            ll det = left_turn_det(polygon[polygon.size() - 2], polygon.back(), p);
            if (isReversed && det > 0) {
                isReversed = false;
            }
            if (!det) {
                polygon.pop_back();
            }
        }
        polygon.push_back(p);
    }
    if (!left_turn_det(polygon[polygon.size() - 2], polygon.back(), polygon.front())) {
        polygon.pop_back();
    }
    format(polygon.begin(), polygon.end(), isReversed);

    int k;
    cin >> k;
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;
        auto result = is_inside(polygon, point(x, y));
        switch (result) {
            case INSIDE:
                cout << "INSIDE\n";
                break;
            case BORDER:
                cout << "BORDER\n";
                break;
            case OUTSIDE:
                cout << "OUTSIDE\n";
                break;
        }
    }
    return 0;
}