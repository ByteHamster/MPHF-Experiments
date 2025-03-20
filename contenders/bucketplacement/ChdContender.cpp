#include "ChdContender.h"

void chdContenderRunner(size_t N, double loadFactor) {
    for (int keysPerBucket = 1; keysPerBucket < 8; keysPerBucket++) {
        {ChdContender(N, loadFactor, loadFactor, keysPerBucket, false).run();}
        {ChdContender(N, loadFactor, loadFactor, keysPerBucket, true).run();}
    }
}
