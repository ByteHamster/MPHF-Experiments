#include "MphfWbpmContender.h"

void mphfWbpmContenderRunner(size_t N) {
    {MphfWbpmContender(N, MPHFDWPaperParameters).run();}
    {MphfWbpmContender(N, MPHFPaperParameters).run();}

    // The "fast" methods are somehow significantly slower
    //{MphfWbpmContender(N, MPHFDWFastParameters).run();}
    //{MphfWbpmContender(N, MPHFFastParameters).run();}
}