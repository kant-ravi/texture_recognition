/*

Name       : Ravi Kant
USC ID     : 7945-0425-48	
e-mail     : rkant@usc.edu	
Submission : Oct 7, 2015

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc,char *argv[])
{
	// Declaration to store the necessary matrices
	Mat_<float> orignalfeatureMatrix(96,25);
	int labels[96];

	// Populating the matrices by reading one value at a time
	// from featureMatrix.txt created at the end of previous program
	double value;
	ifstream fin ("featureMatrix.txt", std::ios_base::in);

	for(int row=0;row<96;row++){

		if(row < 36){				    // first 36 images are of class A
			labels[row] = 0;
		}
		if(row > 35 && row < 72){
			labels[row] = 0;
		}
		if(row > 71){
			labels[row] = 0;
		}

		for(int col=0;col<25;col++){	// storing the features
			fin >> value;
			orignalfeatureMatrix(row,col) = float(value);
		}
	}
	fin.close();



	// Here we perform the PCA to convert 25 dimensional feature space
	// to 1D feature space.
	int reducedDimension = 1;

	PCA pca(orignalfeatureMatrix, Mat(), CV_PCA_DATA_AS_ROW, reducedDimension);

	Mat_<float> projected(96,reducedDimension);

	pca.project(orignalfeatureMatrix,projected);

	Mat_<float> projectedFeatureMatrix;
	projected.convertTo(projectedFeatureMatrix,CV_32F);

	// write the projected features to file
	ofstream fout;
	fout.open ("pcaFeatureMatrix.txt", ios::out);
	for(int i=0;i< 96;i++){
		for(int j=0;j<reducedDimension;j++){
			fout<<projectedFeatureMatrix(i,j)<<" ";
		}
		fout<<"\n";
	}

	cout<<projectedFeatureMatrix;
	fout.close();





}
*/
