#include <iostream>
#include <fstream>
using namespace std;


void pixel_filter(double rsltPixel[3], int x, int y, const unsigned char* origImg, int imgWidth, int imgHeight,
    const double* kernel, int knlWidth, int knlHeight,
    double scale, double offset)
{
  int xoff, yoff, i;
  cout << "\nPixel: x="; cout << x; cout<< ", y="; cout << y;
  for (xoff = 0; xoff < knlHeight; xoff++)
  {
    for (yoff = 0; yoff < knlWidth; yoff++)
    {
      int t = 3 * ((x + xoff - 1) * imgWidth + (y + yoff - 1));
      for(int z = 0; z < 3; z++)
      {
        if (t >= 0 && t < (imgWidth * imgHeight * 3 - 2))
        {
          rsltPixel[z] += kernel[(knlHeight*xoff) + yoff] * origImg[t + z];
        }
      }
    }
  }
  for(i = 0; i < 3; i++)
  {
    cout << "\nResult: "; cout << rsltPixel[i];
    rsltPixel[i] /= scale;
    rsltPixel[i] += offset;
  }
}



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
      if (selection[(j + i * imgWidth)])
      {
        pixel_filter(res, i, j, origImg, imgWidth, imgHeight, kernel, knlWidth, knlHeight, scale, offset);
        rsltImg[i * imgWidth + j] = 0;
        for (int o = 0; o < 3; o++)
        {
          rsltImg[(i * imgWidth + j)] += res[o];
        }
        rsltImg[i * imgWidth + j] /= 3;
      }
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

int main () {
  const unsigned char origImg[243] = {2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  const double kernel[9] =  {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  double result[81] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  const unsigned char selection[81] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
;

  image_filter(result, origImg, selection, 9, 9, kernel, 3, 3, 1.0, 0.0);
  for (int i = 0; i < 9; i++)
  {
    cout << '\n';
    for (int j = 0; j < 9; j++)
    {
      cout << result[i*9 + j];
      cout << ',';
    }
  }
}



