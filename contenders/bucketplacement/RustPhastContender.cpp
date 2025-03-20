#include "RustPhastContender.h"

void rustPHastContenderRunner(size_t N) {
    RustPhastContender(N, 6, 320).run();
    RustPhastContender(N, 7, 370).run();
    RustPhastContender(N, 8, 410).run();
    RustPhastContender(N, 8, 430).run();
    RustPhastContender(N, 8, 450).run();
    RustPhastContender(N, 9, 510).run();
    RustPhastContender(N, 9, 530).run();
    RustPhastContender(N, 10, 620).run();
    RustPhastContender(N, 10, 650).run();
}
