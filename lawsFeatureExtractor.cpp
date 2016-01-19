/*



Name       : Ravi Kant
USC ID     : 7945-0425-48	
e-mail     : rkant@usc.edu	
Submission : Oct 3, 2015

Input Format: programName Data_Location ClassA_name number_of_classA_samples
				classB_name number_of_classB_samples number_of_test_samples

Default Dimension of samples is 128 X 128. Change height, width to change
default height and width respectively.
Default number of channels nChannel = 1


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include "Image.h"

using namespace cv;
using namespace std;


int main(int argc,char *argv[])
{
	if(argc<7) {
		cout<<"Too few arguments";
		cout<<"\n programName Data_Location ClassA_name number_of_classA_samples "
				"classB_name number_of_classB_samples number_of_test_samples";
		exit(1);
	}
	char *location			= argv[1];
	char *classA_name		= argv[2];
	int  classA_numSamples	= atoi(argv[3]);
	char *classB_name		= argv[4];
	int  classB_numSamples  = atoi(argv[5]);
	int	 numTestSamples		= atoi(argv[6]);
	int	 height				= 128;
	int  width				= 128;
	int  nChannels			= 1;


	// Read Images one by one. First we class A images, then class B
	// and finally test samples.
	// Declare featureMatrix of size totalNumberSamples X 25, to store
	// 25 global energy values for each sample



	double featureMatrix[classA_numSamples+classB_numSamples+numTestSamples][25];
	int sampleCount = 1;
	// The outermost loop iterates over the training class A, then training class B
	// and finally the test samples.

	for(int curCase = 1; curCase <= 3; curCase++) {		//begin case loop

		int numSamples;
		char *className;
		if(curCase == 1) {
			numSamples = classA_numSamples;
			className  = "grass";
		}
		if(curCase == 2) {
			numSamples = classB_numSamples;
			className  = "straw";
		}
		if(curCase == 3) {
			numSamples = numTestSamples;
			className  = "unknown";
		}

		int curImage = 1;


		for(curImage=1; curImage <= numSamples; curImage++) {	// begin case images loop

			char folderLoc[100];
			strcpy(folderLoc,location);

			if(curImage<10) {
				strcat(folderLoc,"/");
				strcat(folderLoc,className);
				strcat(folderLoc,"_0");
				char intStrng[3];
				sprintf(intStrng, "%d", curImage);
				strcat(folderLoc,intStrng);
				strcat(folderLoc,".raw");
			}
			else {
				strcat(folderLoc,"/");
				strcat(folderLoc,className);
				strcat(folderLoc,"_");
				char intStrng[3];
				sprintf(intStrng, "%d", curImage);
				strcat(folderLoc,intStrng);
				strcat(folderLoc,".raw");
			}


			Image img(width,height,nChannels,folderLoc);

			// create a 2D array to store result image

			Image outputImage(height,width,1);
			// set image padding limit to 2; this will allow a row
			// column padding of 2 around the image
			img.setPadImageLimit(2);

			// Pre-processing Step: Local mean subtraction
			// Traverse through each pixel and compute its mean local mean
			// using a 5X5 window.
			int row=0,col=0;

			for(row = 0; row < height; row++) {
				for(col = 0; col < width; col++){

					// this is the local 5X5 window
					float localWindowSum = 0;
					int i,j;

					for(i = -2; i <= 2; i++) {
						for(j = -2; j <=2; j++) {
							localWindowSum = localWindowSum + img.getPixel(row+i,col+j);
						}
					}
					// subtracting the local mean from pixel intensity
					float newIntensity = img.getPixel(row,col) - (localWindowSum / 25.0);
					outputImage.setPixel(row,col,0,newIntensity);
				}
			}

			// Storing the Laws 1D kernels in a single array
			// int L5[5]={1,4,6,4,1};
			// int E5[5]={-1,-2,0,2,1};
			// int S5[5]={-1,0,2,0,-1};
			// int W5[5]={-1,2,0,-2,1};
			// int R5[5]={1,-4,6,-4,1};

			int kernels[5][5]={1,4,6,4,1,-1,-2,0,2,1,-1,0,2,0,-1,-1,2,0,-2,1,1,-4,6,-4,1};

			// tensor product: row 0 with row 0, then row 0 with row 1...
			// until row5 with row 5 i.e. L5 with L5, L5 with E5...so on
			// till R5 with R5
			// curFilterCount keeps track of the filter number

			int curFilterCount = 0;
			int ki,kj;
			for(ki = 0; ki < 5; ki++) {				// begin filters loop
				for(kj = 0; kj < 5; kj++) {

					// kernel 1 and 2 are rows of the kernel matrix
					int kernel1[5]={kernels[ki][0],kernels[ki][1],kernels[ki][2],kernels[ki][3],kernels[ki][4]};
					int kernel2[5]={kernels[kj][0],kernels[kj][1],kernels[kj][2],kernels[kj][3],kernels[kj][4]};

					// Allocate save for the 5X5 filter obtained by tensor product
					int filter[5][5];
					int a,b;
					for(a = 0; a < 5; a++){
						for(b = 0; b < 5; b++){
							filter[a][b] = kernel1[a] * kernel2[b];
						}
					}

					// Now we will apply this filter to the mean corrected image
					// and store the result in maskedOutputImage
					// set maskedLimit as we will require mirroring

					Image filteredImage(height,width,1);
					outputImage.setPadImageLimit(2);

					// loop to traverse the maskedImage
					int mi,mj;
					for(mi = 0; mi < height; mi++){
						for(mj = 0; mj < width; mj++) {
							// this is the local 5X5 window
							float localWindowSum = 0;
							int i,j;

							for(i = -2; i <= 2; i++) {
								for(j = -2; j <=2; j++) {
									localWindowSum = localWindowSum + (((float)filter[i+2][j+2])* (outputImage.getPixel(mi+i,mj+j)));
								}
							}
							filteredImage.setPixel(mi,mj,0,localWindowSum);
						}
					}


					 // Since each Image contains only one texture, we can directly
					 // compute global energy of the filtered image

					int fi,fj;
					double globalEnergy=0;
					for(fi = 0; fi < height; fi++){
						for(fj = 0; fj < width; fj++) {
							globalEnergy = globalEnergy + ( filteredImage.getPixel(fi,fj)*filteredImage.getPixel(fi,fj));
						}
					}

					double avgGlobalEnergy = globalEnergy / double(filteredImage.size());
					featureMatrix[sampleCount-1][curFilterCount]=avgGlobalEnergy;

					curFilterCount++;
				}

			}//end of filters loop
			sampleCount++;

		} //end of case images loop

	}// end case loop



	// At this point we have all the feature vectors
	// Normalizing feature space
	double maxMin[2][25];

	for(int fcol=0;fcol<25;fcol++){

		double max = featureMatrix[0][fcol];
		double min = featureMatrix[0][fcol];

		for(int frow=0;frow<96;frow++){

			if(featureMatrix[frow][fcol] >= max)
				max = featureMatrix[frow][fcol];

			if(featureMatrix[frow][fcol] <= min)
				min = featureMatrix[frow][fcol];
		}

		maxMin[0][fcol] = max;
		maxMin[1][fcol] = min;

	}


	// Applying normalization

	for(int fcol=0;fcol<25;fcol++){

		double max = maxMin[0][fcol];
		double min = maxMin[1][fcol];

		for(int frow=0;frow<96;frow++){

			featureMatrix[frow][fcol] = (featureMatrix[frow][fcol] - min)/(max - min);
		}
	}


	ofstream fout;
	fout.open ("featureMatrix.txt", ios::out);
	for(int i=0;i< 96;i++){
		for(int j=0;j<25;j++){
			fout<<featureMatrix[i][j]<<" ";
		}
		fout<<"\n";
	}


	fout.close();


}// end of main



*/
