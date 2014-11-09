#ifndef HISTOGRAM3C_H
#define HISTOGRAM3C_H


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#define CANNOT_LOAD_IMAGE   101
#define HISTOGRAM_NOT_EXISTS 102
#define OUT_OF_INDEX 103


class Histogram3C
{
    public:
        Histogram3C();
        Histogram3C(const char* imagePath, int numBins);
        ~Histogram3C();
        int GetBins() { return m_Bins; }
        void SetBins(int val) { m_Bins = val; }
        cv::Mat GetImage() { return m_Image; }
        void SetImage(cv::Mat val) { m_Image = val; }
        std::vector<cv::Mat> GetHistograms() { return m_Histograms; }
        cv::Mat GetHistogram3D() { return m_Histogram3D; }
        void Calculate();
        void Calculate3D();
        void Draw();
        void Draw1C(int channel);
        void WriteHistograms(const char* xmlPath);
        void WriteHistogram3D(const char* xmlPath);
//        double Compare(cv::Mat histogram, int method);

    protected:
        cv::Mat DrawImage(cv::Mat histogram, cv::Scalar color);
    private:
        int m_Bins;
        cv::Mat m_Image;
        cv::Mat m_Histogram3D;
        std::vector<cv::Mat> m_Histograms;
};

#endif // HISTOGRAM3C_H
