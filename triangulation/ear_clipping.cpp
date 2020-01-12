#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct point {
    int x, y;
    int num, prev, next;
};

ll turn_det(const point &a, const point &b, const point &c) {
    return (ll) (b.x - a.x) * (c.y - a.y) - (ll) (b.y - a.y) * (c.x - a.x);
}


bool inTriangle(const point &a, const point &b, const point &c, const point &p) {
    auto d1 = turn_det(a, b, p);
    auto d2 = turn_det(b, c, p);
    auto d3 = turn_det(c, a, p);
    return (d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0);
}

const int MAXN = 5050;

point vertexes[MAXN];
int n;
int cnt[MAXN];
int sz;

void findEar(int v) {
    cnt[v] = 0;
    int vIdx = v;
    int prevIdx = vertexes[v].prev;
    int nextIdx = vertexes[v].next;
    for (int i = 0; i < n; i++) {
        if (i == vIdx || i == prevIdx || i == nextIdx) continue;
        if (inTriangle(vertexes[vIdx], vertexes[prevIdx], vertexes[nextIdx], vertexes[i])) {
            cnt[v]++;
        }
    }
}

void printTriangulation() {
    int vIdx = 0;
    sz = n;
    while (sz > 3) {
        int prevIdx = vertexes[vIdx].prev;
        int nextIdx = vertexes[vIdx].next;
        auto trn = turn_det(vertexes[vIdx], vertexes[prevIdx], vertexes[nextIdx]);
        if (trn < 0 && cnt[vIdx] == 0) {
            cout << prevIdx + 1 << " " << vIdx + 1 << " " << nextIdx + 1 << "\n";
            vertexes[prevIdx].next = nextIdx;
            vertexes[nextIdx].prev = prevIdx;
            findEar(prevIdx);
            findEar(nextIdx);
            sz--;
        }
        vIdx = vertexes[vIdx].prev;
    }
    cout << vertexes[vertexes[vIdx].prev].num << " " << vertexes[vIdx].num << " " << vertexes[vertexes[vIdx].next].num << "\n";
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        vertexes[i] = {x, y, i + 1, (i + n - 1) % n, (i + 1) % n};
    }
    for (int i = 0; i < n; i++) {
        findEar(i);
    }
    printTriangulation();
    return 0;
}