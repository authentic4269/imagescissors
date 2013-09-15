#include "correlation.h"
#include <iostream>
#include <fstream>
using namespace std;

/************************ TODO 2 **************************/
/*
 *	INPUT:
 *		origImg:		the original image,
 *		imgWidth:		the width of the image
 *		imgHeight:		the height of the image
 *						the image is arranged such that
 *						origImg[3*(row*imgWidth+column)+0],
 *						origImg[3*(row*imgWidth+column)+1],
 *						origImg[3*(row*imgWidth+column)+2]
 *						are R, G, B values for pixel at (column, row).
 *
 *      kernel:			the 2D filter kernel,
 *		knlWidth:		the width of the kernel
 *		knlHeight:		the height of the kernel
 *
 *		scale, offset:  after correlating the kernel with the origImg,
 *						each pixel should be divided by scale and then added by offset
 *
 *		selection:      a byte array of the same size as the image,
 *						indicating where in the original image should be filtered, e.g.,
 *						selection[k] == 1 ==> pixel k should be filtered
 *                      selection[k] == 0 ==> pixel k should NOT be filtered
 *                      a special case is selection is a NULL pointer, which means all the pixels should be filtered.
 *
 *  OUTPUT:
 *		rsltImg:		the filtered image of the same size as original image.
 *						it is a valid pointer ( allocated already ).
 */

void image_filter(double* rsltImg, const unsigned char* origImg, const unsigned char* selection,
    int imgWidth, int imgHeight,
    const double* kernel, int knlWidth, int knlHeight,
    double scale, double offset)
{
  int i = 0;
  int j = 0;
  for(i = 0; i < imgHeight; i++)
  {
    for(j = 0; j < imgWidth; j++)
    {
      double res[3] = {0.0, 0.0, 0.0};
      if (!selection || selection[(j + i * imgWidth)])
      {
        pixel_filter(res, i, j, origImg, imgWidth, imgHeight, kernel, knlWidth, knlHeight, scale, offset);
        rsltImg[i * imgWidth + j] = 0;
        for (int o = 0; o < 3; o++)
        {
          if (res[0] < 0)
            res[0] *= -1.0;
          rsltImg[(i * imgWidth + j)] += res[o];
        }
        rsltImg[i * imgWidth + j] /= 3;
      }
      // Copy directly
      else
      {
        for (int o = 0; o < 3; o++)
        {
          rsltImg[i * imgWidth + j] += origImg[3*(i * imgWidth + j) + o];
        }
        rsltImg[i * imgWidth + j] /= 3;
      }
    }
  }
}

/************************ END OF TODO 2 **************************/


/************************ TODO 3 **************************/
/*
 *	INPUT:
 *      x:				a column index,
 *      y:				a row index,
 *		origImg:		the original image,
 *		imgWidth:		the width of the image
 *		imgHeight:		the height of the image
 *						the image is arranged such that
 *						origImg[3*(row*imgWidth+column)+0],
 *						origImg[3*(row*imgWidth+column)+1],
 *						origImg[3*(row*imgWidth+column)+2]
 *						are R, G, B values for pixel at (column, row).
 *
 *      kernel:			the 2D filter kernel,
 *		knlWidth:		the width of the kernel
 *		knlHeight:		the height of the kernel
 *
 *		scale, offset:  after correlating the kernel with the origImg,
 *						the result pixel should be divided by scale and then added by offset
 *
 *  OUTPUT:
 *		rsltPixel[0], rsltPixel[1], rsltPixel[2]:
 *						the filtered pixel R, G, B values at row y , column x;
 */

void pixel_filter(double rsltPixel[3], int x, int y, const unsigned char* origImg, int imgWidth, int imgHeight,
    const double* kernel, int knlWidth, int knlHeight,
    double scale, double offset)
{
  int xoff, yoff, i;
  for (xoff = 0; xoff < knlHeight; xoff++)
  {
    for (yoff = 0; yoff < knlWidth; yoff++)
    {
      for(int z = 0; z < 3; z++)
      {
        int t = 3 * ((x + xoff - 1) * imgWidth + (y + yoff - 1));
        if (t >= 0 && t < (imgWidth * imgHeight * 3 - 2))
        {
          rsltPixel[z] += kernel[(knlHeight*xoff) + yoff] * origImg[t + z];
        }
      }
    }
  }
  for(i = 0; i < 3; i++)
  {
    rsltPixel[i] /= scale;
    rsltPixel[i] += offset;
  }
}





/************************ END OF TODO 3 **************************/
