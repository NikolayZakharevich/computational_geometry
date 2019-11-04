#ifndef TESTS
#define TESTS

#include <utility>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

double fRand(double fMin = 0, double fMax = 1) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


vector<double> genTest(int id) {
    const auto MAXN = size_t(2e6) * 8;
    const int N = 5;
    vector<double> tests[N];

    switch (id) {
        case 0:
            return {
                    0, 2, 2, 1,
                    1, 0, 2, 2,

                    1, 3, 3, 3,
                    2, 3, 2, 4,

                    3, 0, 4, 0,
                    3, 2, 4, 1
            };
        case 1:
            return {
                    0, 1e-10, 1e-10, 1e-10,
                    1e-10, 1e-10, 1e30, 1e20,

                    1, 3, 3, 3,
                    2, 3, 2, 4,
            };
        case 2:
            return {
                    0, 0, 1, 1,
                    2, 2, 3, 3,

                    0, 0, 1, 1,
                    2, 2, 1, 1,

                    2, 2, 1, 1,
                    0, 0, 1, 1,

                    1e-20, 1e-20, 1e-15, 1e-15,
                    1e-15, 1e-15, 1e-10, 1e-10,

                    1e-20, 1e-20, 1e-15, 1e-15,
                    1e-14, 1e-14, 1e-10, 1e-10

            };
        case 3:
            return vector<double>(MAXN);
        case 4: {
            vector<double> res(MAXN);
            for (auto &r : res) {
                r = fRand(0.0001, 0.0002);
            }
            return res;
        }
        case 5:
            return {
                    0, 0, 1, 1,
                    2, 2, 3, 1,

                    0, 0, 1, 1,
                    1, 1, 2, 1,
            };
        default:
            return {};
    }
}

#endif