/*
 * Image.h
 *
 *  Created on: Oct 3, 2015
 *      Author: ravi
 */

#ifndef IMAGE_H_
#define IMAGE_H_

class Image {
private:

	unsigned int  height;
	unsigned int  width;
	int channel;
	float *data;
	int padImageLimit; ;

public:

	Image(int h,int w,int c,char *location);
	Image(int h,int w,int c);
	Image();
	int rows();
	int cols();
	int size();
	int channels();

	/*
	 * this method gives the pixel value row,col,channel
	 * if gray scale image just enter row,column
	 */
	float getPixel(int row, int col, int c = 0);
	/*
	 * this method sets pixel value at given row,col,channel location where
	 * z is optional id its a gray image.
	 */
	void setPixel(int row, int col, int c,float value);

	/*
	 * set this if you want to crate a padding effect around the image.
	 * Ex. if padImageLimit = 2, then this creates a padding of 2 rows
	 * above and below the image matrix and a padding of 2 columns
	 * to the left and right of the image matrix.
	 * Note: this does not actually create new cells, instead it allows
	 * a mirroring operation to take place whenever a pixel outside the
	 * image matrix but inside the padImageLimit is being accessed.
	 */
	void setPadImageLimit(int limit);
	Image clone(Image);
	virtual ~Image();
	int getPad(){return padImageLimit;}

};

#endif /* IMAGE_H_ */
