#ifndef HISTOGRAM1C_H
#define HISTOGRAM1C_H


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#define CANNOT_LOAD_IMAGE   101
#define HISTOGRAM_NOT_EXISTS 102


class Histogram1C
{
    public:
        Histogram1C();
        Histogram1C(const char* imagePath, int numBins);
        ~Histogram1C();
        int GetBins() { return m_Bins; }
        void SetBins(int val) { m_Bins = val; }
        cv::Mat GetImage() { return m_Image; }
        void SetImage(cv::Mat val) { m_Image = val; }
        cv::Mat GetHistogram() { return m_Histogram; }
        void SetHistogram(cv::Mat val) { m_Histogram = val; }
        void Calculate();
        void Draw();
        double Compare(cv::Mat histogram, int method);
    protected:
    private:
        int m_Bins;
        cv::Mat m_Image;
        cv::Mat m_Histogram;
};

#endif // HISTOGRAM1C_H
