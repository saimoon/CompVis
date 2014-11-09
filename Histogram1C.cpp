#include "Histogram1C.h"

// *** CONSTRUCTORS ***
Histogram1C::Histogram1C()
{
    //ctor
}

Histogram1C::~Histogram1C()
{
    //dtor
}

Histogram1C::Histogram1C(const char* imagePath, int numBins)
{
    std::cout << "DEBUG: Histogram1C::Histogram1C()" << std::endl;

    // image linked to histogram
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cerr << "Can not load image " << imagePath << std::endl;
        throw CANNOT_LOAD_IMAGE;
    }

    // convert to GrayScale if needed
    if (image.type() != CV_8UC1) {
        cvtColor(image, m_Image, CV_RGB2GRAY);
    }

    // number of bins in which group colors
    m_Bins = numBins;
}



// *** METHODS ***

// Calculate 1-channel Histogram
void Histogram1C::Calculate()
{
    std::cout << "DEBUG: Histogram1C::CalculateGray()" << std::endl;

    // Array of the dims arrays of the histogram bin boundaries in each dimension
    float range[] = { 0, 255 } ;
    const float* ranges = { range };

    bool uniform = true;
    bool accumulate = false;

    cv::calcHist(&m_Image, 1, 0, cv::Mat(), m_Histogram, 1, &m_Bins, &ranges, uniform, accumulate);
}


// Draw histogram
void Histogram1C::Draw()
{
    std::cout << "DEBUG: Histogram1C::Draw()" << std::endl;

    int width = 300, height = 200;
    cv::Scalar Color = (255,255,255);
    cv::Mat histogramImage(height, width, CV_8UC1, cv::Scalar(0,0,0));
    cv::Mat normHistogram;

    // histogram width step
    int bin_w = cvRound((double) histogramImage.cols/m_Bins);

    // normalize the result to [ 0, HistogramImage.rows ]
    cv::normalize(m_Histogram, normHistogram, 0, histogramImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    // set histogram polygon points
    cv::Point histPoints[1][m_Bins+1];
    histPoints[0][0] = cv::Point(0, histogramImage.rows);
    for(int i = 0; i < m_Bins; i++)
    {
        histPoints[0][i+1] = cv::Point(bin_w*(i), histogramImage.rows - cvRound(normHistogram.at<float>(i)) );
    }

    const cv::Point* ppt[1] = { histPoints[0] };
    int npt[] = { m_Bins+1 };

    // fill histogram polygon
    cv::fillPoly(histogramImage, ppt, npt, 1, Color, 8);

    cv::namedWindow("Histogram1C", cv::WINDOW_AUTOSIZE);
    cv::imshow("Histogram1C", histogramImage);
}

/**
  * Compare histograms.
  * method:
  *     CV_COMP_CORREL Correlation
  *     CV_COMP_CHISQR Chi-Square
  *     CV_COMP_INTERSECT Intersection
  *     CV_COMP_BHATTACHARYYA Bhattacharyya distance
  *     CV_COMP_HELLINGER Synonym for CV_COMP_BHATTACHARYYA
  **/
double Histogram1C::Compare(cv::Mat histogram, int method)
{
    std::cout << "DEBUG: Histogram1C::Compare()" << std::endl;

    cv::Mat histSrc, histDst;
    cv::normalize(m_Histogram, histSrc, 1.0 );
    cv::normalize(histogram, histDst, 1.0 );

    return cv::compareHist(histSrc, histDst, method );
}
