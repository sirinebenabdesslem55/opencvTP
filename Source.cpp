/**
 * @file compareHist_Demo.cpp
 * @brief Sample code to use the function compareHist
 * @author OpenCV team
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

double moyenne(const Mat& src, int i, int j)
{

	double res = (double)(src.at<uchar>(i, j + 1) + src.at<uchar>(i + 1, j + 1) + src.at<uchar>(i + 1, j) + src.at<uchar>(i, j)) / 4.0;
	return res;
}

int bin2dec(int* tab)
{
	int rest = 0;
	for (int i = 0; i < 4; i++)
	{
		rest += (int)(tab[i] * pow(2, i));


	}

	return rest;
}

void MeanLBP(const Mat& src, Mat& dst) {
	src.copyTo(dst);
	for (int i = 1; i < (src.rows - 1); i++)
		for (int j = 1; j < (src.cols - 1); j++) {
			double moy = moyenne(src, i, j);
			int tab[4];
			if ((double)src.at<uchar>(i + 1, j) < moy)
				tab[0] = 0;
			else
				tab[0] = 1;
			if ((double)src.at<uchar>(i, j) < moy)
				tab[1] = 0;
			else
				tab[1] = 1;
			if ((double)src.at<uchar>(i, j + 1) < moy)
				tab[2] = 0;
			else
				tab[2] = 1;
			if ((double)src.at<uchar>(i + 1, j + 1) < moy)
				tab[3] = 0;
			else
				tab[3] = 1;


			dst.at<unsigned char>(i, j) = (unsigned char)bin2dec(tab);

		}
}




int main(int argc, char** argv)
{
	Mat image1, image2;
	string str1, str2;
	cout << "le nom de l'image 1 avec sont extensions " << "\n";
	cin >> str1;
	image1 = imread(str1);
	cout << "le nom de l'image 1 avec sont extensions " << "\n";
	cin >> str2;
	image2 = imread(str2);
	Mat dst1 ,dst2;
	LBP(image1, dst1);
	LBP(image2, dst2);
	

	/// Using 50 bins for hue and 60 for saturation
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
		float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };
		int channels[] = { 0, 1 };


	/// Histograms
	MatND hist_image1;
	MatND hist_image2;
	//normalize(dst1, dst1, 0, 255, NORM_MINMAX);
	//normalize(dst2, dst2, 0, 255, NORM_MINMAX);
	calcHist(&dst1, 1, channels, Mat(), hist_image1, 2, histSize, ranges, true, false);
	normalize(hist_image1, hist_image1, 0, 1, NORM_MINMAX, -1, Mat());
	calcHist(&dst2, 1, channels, Mat(), hist_image2, 2, histSize, ranges, true, false);
	normalize(hist_image2, hist_image2, 0, 1, NORM_MINMAX, -1, Mat());
	
	double Correlation = compareHist(hist_image1, hist_image2, 0);
	if (Correlation <= 1 && Correlation > 0.940766)
	{
		cout << Correlation << endl;
		cout << " le deux images sont similaires avec la m�thode de correlation  " << endl;
	}
	else
	{

		cout << Correlation << endl;
		cout << " le deux images ne sont pas similaires avec la m�thode de correlation  " << endl;
	}
	double Intersection = compareHist(hist_image1, hist_image2, 2);
	if (Intersection <= 1.08314 && Intersection >1)
	{

		cout << Intersection << endl;
		cout << " le deux images sont similaires avec la m�thode de Intersection  " << endl;
	}
	else
	{

		cout << Intersection << endl;
		cout << " le deux images ne sont pas similaires avec la m�thode de Intersection  " << endl;
	}
    double	Bhattacharyya  = compareHist(hist_image1, hist_image2, 3);
	if (Bhattacharyya >= 0.0 && Bhattacharyya < 0.3)
	{

		cout << Bhattacharyya << endl;
		cout << " le deux images sont similaires avec la m�thode de Bhattacharyya  " << endl;
	}
	else
	{
		cout << Bhattacharyya << endl;
		cout << " le deux images ne sont pas similaires avec la m�thode de Bhattacharyya  " << endl;
	}
	
	while (true)
	{

	}

}