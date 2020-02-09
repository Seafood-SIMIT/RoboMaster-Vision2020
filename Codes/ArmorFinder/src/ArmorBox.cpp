#include <ArmorFinder.h>

ArmorBox::ArmorBox(const cv::Rect &pos, const LightBlobs &blobs, uint8_t color) :
        rect(pos), light_blobs(blobs), box_color(color) {};