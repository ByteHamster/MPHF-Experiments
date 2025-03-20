#include "RustPtrHashContender.h"

void rustPtrHashContenderRunner(size_t N) {
    for (double lambda = 0.7; lambda <= 4.01; lambda += 0.2) {
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_CUBIC_VEC, lambda).run();
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_CUBIC_EF, lambda).run();
    }
    for (double lambda = 0.7; lambda <= 3.41; lambda += 0.2) {
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_LINEAR_VEC, lambda).run();
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_LINEAR_EF, lambda).run();
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_SQUARE_VEC, lambda).run();
        RustPtrHashContender(N, RustPtrHashContender::VARIANT_SQUARE_EF, lambda).run();
    }
}
