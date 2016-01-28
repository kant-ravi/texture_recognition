/*


Name       : Ravi Kant	
e-mail     : rkant@usc.edu	
Submission : Oct 6, 2015
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

// Valid Input: programName featureMatrix_file feature_dimension ClassA_name number_of_classA_samples ...
//                            classB_name number_of_classB_samples number_of_test_samples
// featureMatrix_file = featureMatrix.txt for 2)
// dimension is the number of columns in a feature vector
// featureMatrix_file : name of the file that has the features on which classification
// has to be done

int main(int argc,char *argv[])
{
	if(argc < 8) {
		cout<<"Too few arguments";
		cout<<"\n programName featureMatrix_file feature_dimension ClassA_name number_of_classA_samples "
				"classB_name number_of_classB_samples number_of_test_samples";
		exit(1);
	}
	char *fileName			= argv[1];
	int  dimension 			= atoi(argv[2]);
	char *classA_name		= argv[3];
	int  classA_numSamples	= atoi(argv[4]);
	char *classB_name		= argv[5];
	int  classB_numSamples  = atoi(argv[6]);
	int	 numTestSamples		= atoi(argv[7]);
	int	 height				= 128;
	int  width				= 128;
	int  nChannels			= 1;



	// Declaration to store the necessary matrices
	Mat_<double> classAFeatures(classA_numSamples,dimension);
	Mat_<double> classBFeatures(classB_numSamples,dimension);
	Mat_<double> testFeatures(numTestSamples,dimension);

	double classALabels[classA_numSamples],classBLabels[classB_numSamples];

	// Populating the matrices by reading one value at a time
	// from featureMatrix.txt created at the end of previous program
	double value;
	ifstream fin (fileName, std::ios_base::in);

	for(int row = 0; row < classA_numSamples; row++){		// reading class A samples
		//classALabels(row,0) = 0;
		classALabels[row] = 0;
		for(int col = 0; col < dimension; col++){
			fin >> value;
			classAFeatures(row,col) = float(value);
		}
	}
	for(int row = 0; row < classB_numSamples; row++){		// reading class B samples
		//classBLabels(row,0) = 1;
		classBLabels[row] = 1;
		for(int col = 0; col < dimension; col++){
			fin >> value;
			classBFeatures(row,col) = float(value);
		}
	}
	for(int row = 0; row < numTestSamples; row++){		// reading test samples

			for(int col = 0; col < dimension; col++){
				fin >> value;
				testFeatures(row,col) = float(value);
			}
		}

	fin.close();

	// In order to calculate Mahalanobis distance, we need the
	// inverse co-variance matrices of the two classes

	Mat covarMatClassA, invCovarMatClassA, meanClassA;
	Mat covarMatClassB, invCovarMatClassB, meanClassB;

	cv::calcCovarMatrix(classAFeatures, covarMatClassA, meanClassA, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	cv::calcCovarMatrix(classBFeatures, covarMatClassB, meanClassB, CV_COVAR_NORMAL | CV_COVAR_ROWS);

	invert(covarMatClassA, invCovarMatClassA, DECOMP_LU);
	invert(covarMatClassB, invCovarMatClassB, DECOMP_LU);

	// Predict the classes on training samples of class A
	// A given sample belongs to the class, it is nearest to.
	// if distance from A < distance from B, then sample is
	// classified as belonging to A
	// A miss classification occurs when the predicted and the
	// true label of the sample are not the same.
	// Error = sum(miss-classified samples)

	double predictedClassALabels[classA_numSamples];
	double predictedClassBLabels[classB_numSamples];
	double predictedTestLabels  [numTestSamples];

	int errorOnClassifyingClassA = 0;

	for(int curTrainingSample = 0; curTrainingSample < classA_numSamples; curTrainingSample++) {

		Mat sampleFeatureVector;
		double distanceFromMeanClassA, distanceFromMeanClassB;

		classAFeatures.row(curTrainingSample).copyTo(sampleFeatureVector);

		distanceFromMeanClassA = Mahalanobis(meanClassA, sampleFeatureVector, invCovarMatClassA);
		distanceFromMeanClassB = Mahalanobis(meanClassB, sampleFeatureVector, invCovarMatClassB);

		if(distanceFromMeanClassA <= distanceFromMeanClassB) { // sample belongs to class A
			predictedClassALabels[curTrainingSample] = 0;
			//cout<<"Grass\n";
		}
		else {												   // sample belongs to class B
			predictedClassALabels[curTrainingSample] = 1;
			//cout<<"Straw\n";
		}

		if (predictedClassALabels[curTrainingSample] != 0) {    // see if the classification was right
						errorOnClassifyingClassA++;
		}
	}

	// Predict the classes on training samples of class B

	int errorOnClassifyingClassB = 0;
	for(int curTrainingSample = 0; curTrainingSample < classB_numSamples; curTrainingSample++) {

		Mat sampleFeatureVector;
		double distanceFromMeanClassA, distanceFromMeanClassB;

		classBFeatures.row(curTrainingSample).copyTo(sampleFeatureVector);

		distanceFromMeanClassA = Mahalanobis(meanClassA, sampleFeatureVector, invCovarMatClassA);
		distanceFromMeanClassB = Mahalanobis(meanClassB, sampleFeatureVector, invCovarMatClassB);

		if(distanceFromMeanClassA <= distanceFromMeanClassB) { // sample belongs to class A
			predictedClassBLabels[curTrainingSample] = 0;
			//cout<<"Grass\n";
		}
		else {												   // sample belongs to class B
			predictedClassBLabels[curTrainingSample] = 1;
			//cout<<"Straw\n";
		}
		// see if the classification was right
		if (predictedClassBLabels[curTrainingSample] != 1) {    // see if the classification was right
						errorOnClassifyingClassB++;
		}
	}


	// Predict testLabels by classifying the test samples

	for(int curTestSample = 0; curTestSample < numTestSamples; curTestSample++) {
		Mat sampleFeatureVector;
		double distanceFromMeanClassA, distanceFromMeanClassB;

		testFeatures.row(curTestSample).copyTo(sampleFeatureVector);

		distanceFromMeanClassA = Mahalanobis(meanClassA, sampleFeatureVector, invCovarMatClassA);
		distanceFromMeanClassB = Mahalanobis(meanClassB, sampleFeatureVector, invCovarMatClassB);

		if(distanceFromMeanClassA <= distanceFromMeanClassB) { // sample belongs to class A
			predictedTestLabels[curTestSample] = 0;
		}
		else {												   // sample belongs to class B
			predictedTestLabels[curTestSample] = 1;
		}

	}

	// error on training data

	float total_error = float(errorOnClassifyingClassA + errorOnClassifyingClassB) / float(classA_numSamples + classB_numSamples);
	total_error = total_error * 100;

	ofstream fout("minMeanClassifierPrediction.txt");
	fout<<"\t\t Minimum Distance to Class Mean Classifier\n";
	fout<<"\nError on training data set: "<<total_error<<" percent\n\n";
	fout<<"Predicted Labels for Test Dataset:\n";
	for(int curSample = 0; curSample < numTestSamples; curSample++) {
		if(predictedTestLabels[curSample] == 0)
			fout<<"Sample "<<curSample<<": "<<argv[3]<<"\n";
		else
			fout<<"Sample "<<curSample<<": "<<argv[5]<<"\n";
	}
	fout.close();
	return(0);
}

