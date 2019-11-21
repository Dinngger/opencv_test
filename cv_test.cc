#include "gen_and_detector.hpp"
#include <time.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    unsigned int rng_init;
    if (argc < 2) {
        rng_init = 0xffffffff & time(NULL);
    } else {
        stringstream ss;
        ss << argv[1];
        ss >> hex >> rng_init;
        std::cout << "seed: " << hex << rng_init << endl;
    }
    RNG rng(rng_init);

    char window[] = "Drawing";
    Mat image = Mat::zeros(800, 800, CV_8UC3);

    Generator gen;
    gen.picGenerator(image, rng);
    imshow(window, image);
    Detector detector;
    detector.detect(image);
    waitKey(0);
    return (0);
}
