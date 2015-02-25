#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "mmul.h"

namespace {
void sampleMultiply(int s, float *dst, float const *a, float const *b) {
    for(int r = 0; r < s; ++r) {
        for(int c = 0; c < s; ++c) {
            int index = c + r * s;
            dst[index] = 0;
            for(int k = 0; k < s; ++k) {
                dst[index] += a[k + r * s] * b[c + k * s];
            }
        }
    }
}


float eps = 1e-5;

void test(decltype(sampleMultiply) fsample, decltype(sampleMultiply) ftestee) {
    using namespace std;
    srand(time(0));

    int testsPerSize = 5;
    int maxSize = 400;
    int sizeStep = 20;
    bool fail = false;
    for(int size = 4; size < maxSize && !fail; size += sizeStep) {
        cout << "Testing n=" << size << endl;
        float *sampleDest = new float[size*size];
        float *testeeDest = new float[size*size];
        float *a = new float[size*size];
        float *b = new float[size*size];

        for(int test = 0; test < testsPerSize; ++test) {
            for(int n = 0; n < size * size; ++n) {
                a[n] = static_cast<float>(rand()) / RAND_MAX * 100.0; 
                b[n] = static_cast<float>(rand()) / RAND_MAX * 100.0; 
            }
            clock_t clStart = clock(); 
            fsample(size, sampleDest, a, b);
            clock_t clSample = clock();
            ftestee(size, testeeDest, a, b);
            clock_t clTestee = clock();
            bool ok = true;
            for(int n = 0; n < size * size; ++n) {
                if(abs(sampleDest[n] - testeeDest[n]) > eps) {
                    ok = false;
                    break;
                }
            }
            if(ok)
                cout << "Test passed " << double(clSample - clStart) / CLOCKS_PER_SEC << '\t' << double(clTestee - clSample) / CLOCKS_PER_SEC << endl;
            else {
                cout << "Test failed" << endl;
                fail = true;
                break;
            }
        }
        delete sampleDest;
        delete testeeDest;
        delete a;
        delete b;
    }
}
}

int main() {
    test(sampleMultiply, squareMatrixMultply);
    return 0;
}
