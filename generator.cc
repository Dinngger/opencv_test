#include "gen_and_detector.hpp"

using namespace cv;
using namespace std;

Scalar Generator::randomColor(RNG &rng)
{
    int r = rng.uniform(0, 255);
    int g = rng.uniform(0, 255 - r);
    return Scalar(r, g, 255 - r - g);
}

Point Generator::randomPoint(RNG &rng, Size size)
{
    int weight = size.width;
    int height = size.height;
    return Point(rng.uniform(0, weight), rng.uniform(0, height));
}

Mat Generator::getBorder(Mat img)
{
    Rect r(1, 1, img.size().width - 2, img.size().height - 2);
    Mat mask = Mat::ones(img.size(), CV_8U) * 255;
    mask(r).setTo(0);
    return mask;
}

bool Generator::outOfMask(Mat img, Mat mask)
{
    double max, min;
    Point min_loc, max_loc;
    minMaxLoc(img, &min, &max, &min_loc, &max_loc, mask);
    return max > 0 ? true : false;
}

bool Generator::outOfBorder(Mat img, Mat border)
{
    Mat img_gray;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    return outOfMask(img_gray, border);
}

void Generator::randomTriangle(Mat img, RNG &rng)
{
    Point rook_points[1][3];
    rook_points[0][0] = randomPoint(rng, img.size());
    rook_points[0][1] = randomPoint(rng, img.size());
    rook_points[0][2] = randomPoint(rng, img.size());
    const Point *ppt[1] = {rook_points[0]};
    int npt[] = {3};
    fillPoly(img,
             ppt,
             npt,
             1,
             randomColor(rng),
             LINE_8);
}

void Generator::randomTrapezoid(Mat img, RNG &rng)
{
    Point rook_points[1][4];
    rook_points[0][0] = randomPoint(rng, img.size());
    rook_points[0][1] = randomPoint(rng, img.size());
    rook_points[0][2] = randomPoint(rng, img.size());
    rook_points[0][3] = rook_points[0][2] + rng.uniform(0.5, 1.0) * (rook_points[0][0] - rook_points[0][1]);
    const Point *ppt[1] = {rook_points[0]};
    int npt[] = {4};
    fillPoly(img,
             ppt,
             npt,
             1,
             randomColor(rng),
             LINE_8);
}

bool Generator::tooSmall(Mat img)
{
    double light = mean(img)[0];
    return light < 0.5;
}

void Generator::picGenerator(Mat img, RNG &rng)
{
    Mat border = getBorder(img);
    for (int i = 0; i < 5; i++) {
        Mat _img;
        do {
            _img = Mat::zeros(img.size(), CV_8UC3);
            if (i < 3) {
                circle(_img,
                    randomPoint(rng, img.size()),
                    rng.uniform(6, img.size().height / 2),  // can't be too small
                    randomColor(rng),
                    FILLED,
                    LINE_8);
            } else if (i < 4) {
                randomTriangle(_img, rng);
            } else {
                randomTrapezoid(_img, rng);
            }
        } while (outOfBorder(_img, border) || tooSmall(_img));
        Mat _img_gray;
        cvtColor(_img, _img_gray, CV_BGR2GRAY);
        Mat _img_mask;
        threshold(_img_gray, _img_mask, 1, 255, CV_THRESH_BINARY);
        add(img, _img, img, _img_mask);
        border += _img_mask;
    }
}
