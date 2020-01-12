#include <bits/stdc++.h>
#include <ostream>

using namespace std;

typedef long long ll;

enum axis {
    HORIZONTAL = 1,
    VERTICAL = 2,
};

struct point {
    int x, y, num;

    friend ostream &operator<<(ostream &os, const point &point1) {
        os << "x: " << point1.x << " y: " << point1.y;
        return os;
    }

    point(int x, int y, int num) : x(x), y(y), num(num) {}
    point() {}

    int operator[](const axis ax) const { return ax == HORIZONTAL ? x : y; }
};

typedef vector<point>::iterator it;

struct node {
    point x;
    node *left = nullptr;
    node *right = nullptr;
    axis ax;

    node(point x) : x(x) {}

    node(point x, axis ax, node *left, node *right) : x(x), ax(ax), left(left), right(right) {}
};

node *buildKdTree(it begin, it end, int depth = 0) {
    if (begin >= end) return nullptr;

    auto ax = depth & 1 ? VERTICAL : HORIZONTAL;
    auto cmp = [&ax](const point &p1, const point &p2) { return p1[ax] < p2[ax]; };
    auto mid = begin + (end - begin) / 2;
    nth_element(begin, mid, end, cmp);

    while (mid > begin && (*(mid - 1))[ax] == (*mid)[ax]) {
        --mid;
    }

    return new node(*mid, ax, buildKdTree(begin, mid, depth + 1), buildKdTree(mid + 1, end, depth + 1));
}


ll dist(const point &a, const point &b) {
    return (ll) (b.x - a.x) * (b.x - a.x) + (ll) (b.y - a.y) * (b.y - a.y);
}

ll ax_subtract(const point &p1, const point &p2, axis ax) {
    return p1[ax] - p2[ax];
}

struct best_match {
    node* node_;
    ll distance;
    best_match(node* n, ll d) : node_(n), distance(d) {}
};

void nn(const point &key, node *curr, best_match &best) {
    if (curr == nullptr) return;

    ll d = dist(key, curr->x);
    ll dx = ax_subtract(key, curr->x, curr->ax);
    if (d < best.distance) {
        best.node_ = curr;
        best.distance = d;
    }
    node *near = dx <= 0 ? curr->left : curr->right;
    node *far = dx <= 0 ? curr->right : curr->left;
    nn(key, near, best);
    if ((dx * dx) < best.distance) {
        nn(key, far, best);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<point> points;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        points.emplace_back(x, y, i + 1);
    }
    auto root = buildKdTree(points.begin(), points.end(), HORIZONTAL);

    point p;
    while (m--) {
        cin >> p.x >> p.y;
        best_match best(root, LONG_LONG_MAX);
        nn(p, root, best);
//        cout << "query = " << p << ", num = " << best.node_->x.num << ", nn = ";
        cout << best.node_->x.num << "\n";
    }


    return 0;
}

/**
10 10
0 0
0 1
-1 2
-4 5
-4 3
-2 1
-2 0
-2 -1
1 -2
4 2

10 10
0 0
0 1
-1 2
-4 5
-4 3
-2 1
-2 0
-2 -1
1 -2
4 2
0 -3
-1 -3
-2 -3
-1 -1
1 -1
1 0
1 1
-1 1
-2 2
-2 4
*/