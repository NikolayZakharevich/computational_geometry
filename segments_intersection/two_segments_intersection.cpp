#include <bits/stdc++.h>
#include <boost/numeric/interval.hpp>
#include <gmpxx.h>
#include "tests.h"

using namespace std;

const double MACHINE_EPS_x4 = 4 * numeric_limits<double>::epsilon();

template<typename T>
struct point {
    T x, y;
};

bool check_bound_box(double a, double b, double c, double d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

template<typename T>
T left_turn_det(const point<T> &a, const point<T> &b, const point<T> &c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

int left_turn(const point<double> &a, const point<double> &b, const point<double> &c) {
    double l = (c.x - a.x) * (b.y - a.y);
    double r = (c.y - a.y) * (b.x - a.x);
    double det = l - r;
    double err = (fabs(l) + fabs(r)) * MACHINE_EPS_x4;
    if (det > err) return 1;
    if (det < -err) return -1;
    auto int_res = left_turn_det<boost::numeric::interval<double>>({a.x, a.y}, {b.x, b.y}, {c.x, c.y});
    if (!boost::numeric::zero_in(int_res)) {
        return int_res > 0 ? 1 : -1;
    }
    int res = cmp(left_turn_det<mpq_class>({a.x, a.y}, {b.x, b.y}, {c.x, c.y}), 0);
    return res > 0 ? 1 : (res < 0 ? -1 : 0);
}

bool solve(const point<double> &a, const point<double> &b, const point<double> &c, const point<double> &d) {
    return check_bound_box(a.x, b.x, c.x, d.x)
           && check_bound_box(a.y, b.y, c.y, d.y)
           && (left_turn(a, b, c) * left_turn(a, b, d) <= 0)
           && (left_turn(c, d, a) * left_turn(c, d, b) <= 0);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_id;
    std::cin >> test_id;
    vector<double> a = genTest(test_id);
    for (int i = 0; i < a.size(); i += 8) {
        cout << (solve({a[i], a[i + 1]}, {a[i + 2], a[i + 3]}, {a[i + 4], a[i + 5]}, {a[i + 6], a[i + 7]}) ? 'Y' : 'N');
    }
}