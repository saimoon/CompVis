#include <iostream>
#include "Histogram1C.h"
#include "Histogram3C.h"


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // Gray level histogram
    Histogram1C histogram("lena.jpg", 256);
    histogram.Calculate();
    Mat hist = histogram.GetHistogram();
    histogram.Draw();

    // Color histogram
    Histogram3C hist3C("lena.jpg", 256);
    hist3C.Calculate();
    vector<Mat> hists = hist3C.GetHistograms();
    hist3C.WriteHistograms("hists.xml");
    hist3C.Draw();
    hist3C.Draw1C(0);
    hist3C.Draw1C(1);
    hist3C.Draw1C(2);

    // 3D histogram
    Histogram3C hist3C3D("lena.jpg", 3);
    hist3C3D.Calculate3D();
    Mat hist3D = hist3C3D.GetHistogram3D();
    hist3C3D.WriteHistogram3D("hist3D.xml");


    waitKey(0);
    return 0;
}



