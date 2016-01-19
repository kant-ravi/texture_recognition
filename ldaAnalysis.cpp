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
	Mat_<float> trainingMatrix(72,25);
	Mat_<float> testingMatrix(24,25);
	Mat_<float> trainingLabels(72,1);

	// Populating the matrices by reading one value at a time
	// from featureMatrix.txt created at the end of previous program
	double value;
	ifstream fin ("featureMatrix.txt", std::ios_base::in);

	for(int row=0;row<96;row++) {

		if(row < 36) {				    // first 36 images are of class A
			trainingLabels(row,0) = 0;
			for(int col=0;col<25;col++){	// storing the features
				fin >> value;
				trainingMatrix(row,col) = float(value);
			}
		}
		if(row > 35 && row < 72) {
			trainingLabels(row,0) = 1;
			for(int col=0;col<25;col++) {	// storing the features
				fin >> value;
				trainingMatrix(row,col) = float(value);
			}
		}
		if(row > 71) {
			for(int col=0;col<25;col++){	// storing the features
				fin >> value;
				testingMatrix(row-72,col) = float(value);
			}
		}

	}
	fin.close();



	// Here we perform the PCA to convert 25 dimensional feature space
	// to 1D feature space.
	int reducedDimension = 1;

	LDA lda(trainingMatrix, trainingLabels, 0);

	Mat_<float> projected(72,reducedDimension);

	projected=lda.project(trainingMatrix);

	Mat_<float> projectedFeatureMatrix;
	projected.convertTo(projectedFeatureMatrix,CV_32F);


	// write the projected features to file
	ofstream fout;
	fout.open ("ldaFeatureMatrix.txt", ios::out);
	for(int i=0;i< 72;i++){
		for(int j=0;j<reducedDimension;j++){
			fout<<projectedFeatureMatrix(i,j)<<" ";
		}
		fout<<"\n";
	}

	Mat_<float> projectedTest(24,reducedDimension);
	projectedTest = lda.project(testingMatrix);

	Mat_<float> projectedTestMatrix;
	projectedTest.convertTo(projectedTestMatrix,CV_32F);

	for(int i=0;i< 24;i++){
		for(int j=0;j<reducedDimension;j++){
			fout<<projectedTestMatrix(i,j)<<" ";
		}
		fout<<"\n";
	}

	fout.close();

}

*/
