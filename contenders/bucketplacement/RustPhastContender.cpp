#include "RustPhastContender.h"

void rustPHastContenderRunner(size_t N) {
    for (size_t bucket_size_100 = 100; bucket_size_100 <= 280; bucket_size_100 += 20)
        RustPhastContender(N, 4, bucket_size_100).run();
    
    for (size_t bucket_size_100 = 160; bucket_size_100 <= 280; bucket_size_100 += 20)
        RustPhastContender(N, 5, bucket_size_100).run();
    RustPhastContender(N, 5, 290).run();

    RustPhastContender(N, 6, 260).run();
    RustPhastContender(N, 6, 280).run();
    for (size_t bucket_size_100 = 300; bucket_size_100 <= 330; bucket_size_100 += 10)
        RustPhastContender(N, 6, bucket_size_100).run();

    RustPhastContender(N, 7, 320).run();
    RustPhastContender(N, 7, 340).run();
    for (size_t bucket_size_100 = 360; bucket_size_100 <= 400; bucket_size_100 += 10)
        RustPhastContender(N, 7, bucket_size_100).run();

    RustPhastContender(N, 8, 350).run();
    RustPhastContender(N, 8, 370).run();
    for (size_t bucket_size_100 = 390; bucket_size_100 <= 460; bucket_size_100 += 10)
        RustPhastContender(N, 8, bucket_size_100).run();

    for (size_t bucket_size_100 = 470; bucket_size_100 <= 530; bucket_size_100 += 20)
        RustPhastContender(N, 9, bucket_size_100).run();

    for (size_t bucket_size_100 = 530; bucket_size_100 <= 590; bucket_size_100 += 20)
        RustPhastContender(N, 10, bucket_size_100).run();

    for (size_t bucket_size_100 = 590; bucket_size_100 <= 650; bucket_size_100 += 20)
        RustPhastContender(N, 11, bucket_size_100).run();
    RustPhastContender(N, 11, 660).run();
    RustPhastContender(N, 11, 665).run();

    for (size_t bucket_size_100 = 660; bucket_size_100 <= 720; bucket_size_100 += 15)
        RustPhastContender(N, 12, bucket_size_100).run();
    RustPhastContender(N, 12, 730).run();

    for (size_t bucket_size_100 = 730; bucket_size_100 <= 790; bucket_size_100 += 20)
        RustPhastContender(N, 13, bucket_size_100).run();
}
