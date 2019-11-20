#ifndef GEN_AND_DETECTOR_HPP
#define GEN_AND_DETECTOR_HPP

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace cv;

class Generator
{
private:
    Scalar randomColor(RNG &rng);
    Point randomPoint(RNG &rng, Size size);
    Mat getBorder(Mat img);
    bool outOfMask(Mat img, Mat mask);
    bool outOfBorder(Mat img, Mat border);
    void randomTriangle(Mat img, RNG &rng);
    void randomTrapezoid(Mat img, RNG &rng);
    bool tooSmall(Mat img);
public:
    Generator() {}
    void picGenerator(Mat img, RNG &rng);
};

class Detector
{
private:
    double vec2f_distance(Vec2f a, Vec2f b);
public:
    Detector() {}
    void detect(Mat img);
};

#endif // GEN_AND_DETECTOR_HPP
