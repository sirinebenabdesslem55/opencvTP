#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

//cacul de moyenne pour MeanLBP
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

//LBP comparaison des 8 pixel autour par rapport au centre (i,j)
void LBP(const Mat& src, Mat& dst) {
	src.copyTo(dst);
	for (int i = 1; i < (src.rows - 1); i++)
		for (int j = 1; j < (src.cols - 1); j++) {
			double centre = src.at<uchar>(i,j);
			int tab[8];
			if ((double)src.at<uchar>(i -1, j-1) < centre)
				tab[0] = 0;
			else
				tab[0] = 1;
			if ((double)src.at<uchar>(i-1, j) < centre)
				tab[1] = 0;
			else
				tab[1] = 1;
			if ((double)src.at<uchar>(i-1,j+1) < centre)
				tab[2] = 0;
			else
				tab[2] = 1;
			if ((double)src.at<uchar>(i, j+1) < centre)
				tab[3] = 0;
			else
				tab[3] = 1;
			if ((double)src.at<uchar>(i+1 , j+1) < centre)
				tab[4] = 0;
			else
				tab[4] = 1;

			if ((double)src.at<uchar>(i+1, j) < centre)
				tab[5] = 0;
			else
				tab[5] = 1;

			if ((double)src.at<uchar>(i+1, j-1) < centre)
				tab[6] = 0;
			else
				tab[6] = 1;

			if ((double)src.at<uchar>(i, j-1) < centre)
				tab[7] = 0;
			else
				tab[7] = 1;
			dst.at<unsigned char>(i, j) = (unsigned char)bin2dec(tab);

		}
}

//MeanLBP tab[2] car pixel 2*2, on les compare par rapport à la moyenne
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

void Histogram(Mat dst, MatND hist_img){
	/// Using 50 bins for hue and 60 for saturation
	//constante nécessaire pour calcul histogram
	int h_bins = 50;
	int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	int channels[] = { 0, 1 };

	/// Histograms
	calcHist(&dst, 1, channels, Mat(), hist_img, 2, histSize, ranges, true, false);
}

int main(int argc, char** argv)
{
	Mat imageTest;
	string str1;
	cout << "*****Histograme avec LBP*****" << "\n";
	cout << "Le nom de l'image avec sont extensions " << "\n";
	cin >> str1;
	imageTest = imread(str1);
	Mat dstLBP;
	LBP(imageTest, dstLBP);
	MatND hist_LBP;
	Histogram(dstLBP, hist_LBP);

	cout << "*****Histograme avec Mean-LBP*****" << "\n";
	Mat dstMLBP;
	LBP(imageTest, dstMLBP);
	MatND hist_image;
	Histogram(dstMLBP, hist_image);

	/*int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	int channels[] = { 0, 1 };
	calcHist(&dst1, 1, channels, Mat(), hist_image1, 2, histSize, ranges, true, false);
	*/

	cout << "Test de comparaison de deux images avec MeanLBP a travers trois méthodes differentes" << "\n";
	Mat image1, image2;
	string str1, str2;
	cout << "le nom de l'image 1 avec sont extensions " << "\n";
	cin >> str1;
	image1 = imread(str1);
	cout << "le nom de l'image 1 avec sont extensions " << "\n";
	cin >> str2;
	image2 = imread(str2);
	Mat dst1 ,dst2;
	MeanLBP(image1, dst1);
	MeanLBP(image2, dst2);
	MatND hist_image1;
	MatND hist_image2;
	Histogram(dst1, hist_image1);
	normalize(hist_image1, hist_image1, 0, 1, NORM_MINMAX, -1, Mat());
	Histogram(dst2, hist_image2);
	normalize(hist_image2, hist_image2, 0, 1, NORM_MINMAX, -1, Mat());

	double Correlation = compareHist(hist_image1, hist_image2, 0);
	if (Correlation <= 1 && Correlation > 0.940766)
	{
		cout << Correlation << endl;
		cout << " le deux images sont similaires avec la methode de correlation  " << endl;
	}
	else
	{
		cout << Correlation << endl;
		cout << " le deux images ne sont pas similaires avec la methode de correlation  " << endl;
	}

	double Intersection = compareHist(hist_image1, hist_image2, 2);
	if (Intersection <= 1.08314 && Intersection >1)
	{
		cout << Intersection << endl;
		cout << " le deux images sont similaires avec la methode de Intersection  " << endl;
	}
	else
	{

		cout << Intersection << endl;
		cout << " le deux images ne sont pas similaires avec la methode de Intersection  " << endl;
	}
    double	Bhattacharyya  = compareHist(hist_image1, hist_image2, 3);
	if (Bhattacharyya >= 0.0 && Bhattacharyya < 0.3)
	{
		cout << Bhattacharyya << endl;
		cout << " le deux images sont similaires avec la methode de Bhattacharyya  " << endl;
	}
	else
	{
		cout << Bhattacharyya << endl;
		cout << " le deux images ne sont pas similaires avec la methode de Bhattacharyya  " << endl;
	}
	
	while (true)
	{

	}


}