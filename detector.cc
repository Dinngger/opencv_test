#include "gen_and_detector.hpp"

using namespace cv;
using namespace std;

double Detector::vec2f_distance(Vec2f a, Vec2f b)
{
    double dist = 0;
    for (int i=0; i < 2; i++) {
        dist += pow((a[i] - b[i]), 2);
    }
    return sqrt(dist);
}

void Detector::detect(Mat img)
{
    Mat img_gray;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    vector<vector<Point>> contours;
    findContours(img_gray, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    string s[contours.size()];
    int lines_n[contours.size()];
    for (size_t i = 0; i< contours.size(); i++)
    {
        Point left_top = *min_element(contours[i].begin(), contours[i].end(), [](Point a, Point b)->bool{return a.x + a.y < b.x + b.y;});
        Mat drawing = Mat::zeros(img_gray.size(), CV_8U);
        drawContours(drawing, contours, (int)i, Scalar(255), 1);
        Mat mask = Mat::zeros(img_gray.size(), CV_8U);;
        drawContours(mask, contours, (int)i, Scalar(255), FILLED);
        Scalar color = mean(img, mask);

        Moments moment = moments(contours[i], false);
        int x, y;
		if (moment.m00 != 0)
		{
			x = cvRound(moment.m10 / moment.m00);
			y = cvRound(moment.m01 / moment.m00);
        }

        // Standard Hough Line Transform
        vector<Vec2f> lines; // will hold the results of the detection
        HoughLines(drawing, lines, 1.5, 1.5*CV_PI/180, 100); // runs the actual detection
        int lines_num=1;
        vector<Vec2f> real_lines;
        for (int i=0; i<lines.size(); i++) {
            bool real = true;
            for (int j=0; j<real_lines.size(); j++) {
                if (vec2f_distance(lines[i], real_lines[j]) < 20) {
                    real = false;
                    break;
                }
            }
            if (real) {
                real_lines.push_back(lines[i]);
                lines_num++;
            }
        }

        if (lines_num - 1 == 3) {
            x = left_top.x;
            y = left_top.y;
        }

        stringstream ss;
        ss << lines_num << " lines. " << "color: " << color << "\tposition: (" << x << ", " << y << ")\n";
        circle(img, Point(x, y), 2, Scalar(255, 255, 255));
        getline(ss, s[i]);
        lines_n[i] = lines_num;
    }
    imshow("img", img);
    int lines_n_sorted[contours.size()];
    bool printed[contours.size()];
    for (int i=0; i<contours.size(); i++) {
        lines_n_sorted[i] = lines_n[i];
        printed[i] = false;
    }
    sort(lines_n_sorted, lines_n_sorted + contours.size());
    int cnt = 1;
    for (int i=0; i<contours.size(); i++) {
        for (int j=0; j<contours.size(); j++) {
            if (lines_n[j] <= lines_n_sorted[i] && !printed[j]) {
                std::cout << "contour " << cnt << " : " << s[j] << endl;
                printed[j] = true;
                cnt++;
            }
        }
    }
}
