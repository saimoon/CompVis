#include "Histogram3C.h"

// *** CONSTRUCTORS ***
Histogram3C::Histogram3C()
{
    //ctor
}

Histogram3C::~Histogram3C()
{
    //dtor
}

Histogram3C::Histogram3C(const char* imagePath, int numBins)
{
    std::cout << "DEBUG: Histogram3C::Histogram3C()" << std::endl;

    // image linked to histogram
    m_Image = cv::imread(imagePath);
    if (m_Image.empty())
    {
        std::cerr << "Can not load image " << imagePath << std::endl;
        throw CANNOT_LOAD_IMAGE;
    }

    // number of bins in which group colors
    m_Bins = numBins;

    // Reserve 3-channels histogram
    m_Histograms.reserve(3);
}

// Calculate 3-channel Histogram
void Histogram3C::Calculate()
{
    std::cout << "DEBUG: Histogram3C::Calculate()" << std::endl;

    // Separate source image in its color channels
    std::vector<cv::Mat> planes(m_Image.channels());
    cv::split(m_Image, planes);

    // Array of the dims arrays of the histogram bin boundaries in each dimension
    float range[] = { 0, 255 } ;
    const float* ranges = { range };

    bool uniform = true;
    bool accumulate = false;

    std::vector<cv::Mat> hists(m_Image.channels());
    cv::calcHist( &planes[0], 1, 0, cv::Mat(), hists[0], 1, &m_Bins, &ranges, uniform, accumulate );
    cv::calcHist( &planes[1], 1, 0, cv::Mat(), hists[1], 1, &m_Bins, &ranges, uniform, accumulate );
    cv::calcHist( &planes[2], 1, 0, cv::Mat(), hists[2], 1, &m_Bins, &ranges, uniform, accumulate );

    m_Histograms.push_back(hists[0]);
    m_Histograms.push_back(hists[1]);
    m_Histograms.push_back(hists[2]);
}

// Calculate 3D Histogram
void Histogram3C::Calculate3D()
{
    std::cout << "DEBUG: Histogram3C::Calculate3D()" << std::endl;

    int channel_numbers[] = { 0, 1, 2 };

    int *number_bins = new int[m_Image.channels()];
    for (int i=0; i<m_Image.channels(); i++)
    {
        number_bins[i] = m_Bins;
    }

    float ch_range[] = { 0.0, 255.0 };
    const float *channel_ranges[] = {ch_range, ch_range, ch_range};

    bool uniform = true;
    bool accumulate = false;

    calcHist(&m_Image, 1, channel_numbers, cv::Mat(), m_Histogram3D, m_Image.channels(), number_bins, channel_ranges, uniform, accumulate);
}

// Draw histogram
void Histogram3C::Draw()
{
    std::cout << "DEBUG: Histogram3C::Draw()" << std::endl;

    cv::namedWindow("Histogram3C", cv::WINDOW_AUTOSIZE);

    // Get image of each channel's histogram
    std::vector<cv::Mat> images(m_Image.channels());
    images[0] = DrawImage(m_Histograms[0], cv::Scalar(255));
    images[1] = DrawImage(m_Histograms[1], cv::Scalar(255));
    images[2] = DrawImage(m_Histograms[2], cv::Scalar(255));

    // Merge different histograms, one for each channel
    cv::Mat outImages;
    cv::merge(images, outImages);

    cv::imshow("Histogram3C", outImages);
}

void Histogram3C::Draw1C(int channel)
{
    std::cout << "DEBUG: Histogram3C::Draw1C()" << std::endl;

    if (channel >= m_Histograms.size())
    {
        std::cerr << "Out of index: " << channel << std::endl;
        throw OUT_OF_INDEX;
    }

    std::ostringstream winName;
    winName << "Histogram3C-1C-" << channel;
    cv::namedWindow(winName.str(), cv::WINDOW_AUTOSIZE);

    // Get image
    cv::Mat image = DrawImage(m_Histograms[channel], cv::Scalar(255));

    // Show image
    cv::imshow(winName.str(), image);
}

// Write m_Histogram3D member to xml file
void Histogram3C::WriteHistograms(const char* xmlPath)
{
    std::cout << "DEBUG: Histogram3C::WriteHistograms()" << std::endl;

    cv::FileStorage fs(xmlPath, cv::FileStorage::WRITE);
    fs << "Histograms" << m_Histograms;
    fs.release();
}

// Write m_Histogram3D member to xml file
void Histogram3C::WriteHistogram3D(const char* xmlPath)
{
    std::cout << "DEBUG: Histogram3C::WriteHistogram3D()" << std::endl;

    cv::FileStorage fs(xmlPath, cv::FileStorage::WRITE);
    fs << "Histogram3D" << m_Histogram3D;
    fs.release();
}

// *** PROTECTED ***
cv::Mat Histogram3C::DrawImage(cv::Mat histogram, cv::Scalar color)
{
    int width = 300, height = 200;
    cv::Mat histogramImage(height, width, CV_8UC1, cv::Scalar(0));
    cv::Mat normHistogram;

    // histogram width step
    int bin_w = cvRound((double) histogramImage.cols/m_Bins);

    // normalize the result to [ 0, HistogramImage.rows ]
    cv::normalize(histogram, normHistogram, 0, histogramImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

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
    cv::fillPoly(histogramImage, ppt, npt, 1, color, 8);

    return histogramImage;
}
