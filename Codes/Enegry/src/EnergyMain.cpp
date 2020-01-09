#include "energy.h"

using namespace cv;
using namespace std;

int energy(cv::Mat &src)
{   
    uint8_t color=1;
    Energy energy(color);
    energy.run(src);
    return 0;
}
