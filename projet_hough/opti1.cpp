#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>


#define atn(i, a, b) i[a*n+b]
#define atm(i, a, b) i[a*m+b]
#define at(i, a, b, c) i[a*c+b]

using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

double* deriche_GL(uchar* I, double gamma, int n, int m);
void roberts(const double *Is, double *Igx, double *Igy, int n, int m);
double* hough_exh(uchar*Ib,int n,int m, int *truc);
int* imregionalmax(double* Acc, int n, int m);
Mat hough_peaks_lines(double* Acc, Mat RGB, int votes, int maxrho);
int main(int argc, char const *argv[])
{
  VideoCapture cap(0);
  if(!cap.isOpened()){  // check if we succeeded
    std::cout<<"merde\n";
        return -1;
    }
  namedWindow( "Original image", WINDOW_AUTOSIZE );
  namedWindow( "Smoothed image", WINDOW_AUTOSIZE );
  /*namedWindow( "Gradient", WINDOW_AUTOSIZE );
  namedWindow( "Binary contours", WINDOW_AUTOSIZE );
  namedWindow( "Houhgh accumulator space", WINDOW_AUTOSIZE );
*/
    Mat ii;
    Mat mat;
    unsigned char th  = 160;
  while(1==1){  
    cap>>mat;
    cvtColor(mat, ii, CV_RGB2GRAY);
    imshow( "Original image", mat ); 
    //imshow( "Smoothed image", ii ); 
    
    double gamma = 0.2;
    int votes = 100;
    int n = ii.rows;
    int m = ii.cols;
    uchar* I =ii.data;
    double *Is = deriche_GL(I,gamma,n,m);

    double *Igx = (double*) calloc(n*m,sizeof(double));
    double *Igy = (double*) calloc(n*m,sizeof(double));
    roberts(Is,Igx,Igy,n,m);
    int i;
    double *Ig = (double*) malloc(n*m*sizeof(double));
    uchar* Ib = (uchar*) malloc(n*m*sizeof(uchar));
    for(i=0;i<n*m;i++) {
      double t1 = Igx[i];
      double t2 = Igy[i];
      double t = t1*t1+t2*t2;
      Ig[i] = (t>255)? 255:t;
      Ib[i]=(t>th)?255:0;
    }

    int maxrho;
    //double* Acc = hough_exh(Ib,n,m,&maxrho);

    Mat IIs = ii.clone();
    Mat IIb = ii.clone();
    Mat IIg = ii.clone();
    for(i=0;i<n*m;i++){
      int t = Is[i];
      IIs.data[i] = (t>255)? 255:t;
      t = Ig[i];
      IIg.data[i] = (t>255)? 255:t;
      t = Ib[i];
      IIb.data[i] = (t>255)? 255:t;
    }

    //hough_peaks_lines(Ig, mat,50);

    imshow( "Smoothed image",IIs);
    imshow( "Gradient",IIg);
    imshow( "Binary contours",IIb);

    double* Acc= hough_exh(Ib,n,m,&maxrho);

    int* Accmax = imregionalmax(Acc,n,m);
    for(i=0;i<n*m;i++){
      if(Accmax[i] = 0) Acc[i] = 0;
    }

    Mat aa(maxrho,180,0);
    for(i=0;i<180*maxrho;i++){
      aa.data[i] = (unsigned char) Acc[i];
    }

    Mat ab = hough_peaks_lines(Acc,ii,votes,maxrho);
    imshow( "Acc",aa);
    imshow( "Lines",ab);


    free(Igx);
    free(Igy);
    free(Ig);
    free(Ib);
    free(Is);
    char c = waitKey(30);
    if(c == ' ') {
      char cc = waitKey(30);
      while(cc != ' ') {
        if(cc = 's') imwrite("out.png",mat);
        cc = 'j';
        cc = waitKey(30);
      }
    }
    else if(c=='R'){
      c='t';
      th++;
    }
    else if(c=='T'){
      c='t';
      th--;
    }
    else if(c>=0) {
      break;
    }
  }
  mat.release();
  ii.release();
  return 0;
}

double* deriche_GL(uchar* I, double gamma, int n, int m){
  double g = gamma;
  double g1 = (1-g)*(1-g);
  double g2 = 2*g;
  double gg = g*g;
  double *Ia = (double*) malloc(n*m*sizeof(double));
  double *Ib = (double*) malloc(n*m*sizeof(double));
  double *Ic = (double*) malloc(n*m*sizeof(double));
  int i,j;
  double m1,m2;
  for (i=0;i<n;i++) {
    atm(Ic,i,0) = g1*atm(I,i,0);
    atm(Ic,i,1) = g1*atm(I,i,1)+ (g2 * atm(Ic,i,0));
    m1 = atm(Ic,i,1);
    m2 = atm(Ic,i,0);
    atm(Ic,i,2)= (g1*atm(I,i,2)) + (g2 * m1) + (gg*m2);
    for(j=3;j<m;j++){
      m2 = m1; m2 = atm(Ic,i,j-2);
      atm(Ic,i,j)= (g1*atm(I,i,j)) + (g2 * m1) + (gg*m2);
    }
    // Filtre anticausal
    atm(Ia,i,n-1) = g1*atm(Ic,i,n-1);
    atm(Ia,i,n-2) = g1*atm(Ic,i,n-2) + g2*atm(Ia,i,n-1);
    
    m1 = atm(Ia,i,m-2); 
    m2 = atm(Ia,i,m-1);
    atm(Ia,i,m-3) = g1*atm(Ic,i,m-3) + g2*m1 + gg*m2;

    for (j=m-4;j>-1;j--){
      m1 = m2; m2 = atm(Ia,i,j+2);
      atm(Ia,i,j) = g1*atm(Ic,i,j) + g2*m1 + gg*m2;
    }
  }


  for (i=0;i<m;i++) {
    atn(Ic,i,0) = g1*atm(I,0,i);
    atn(Ic,i,1) = g1*atm(I,1,i)+ (g2 * atn(Ic,i,0));

    m1 = atn(Ic,i,1);
    m2 = atn(Ic,i,0);
    atn(Ic,i,2)= (g1*atm(I,i,2)) + (g2 * m1) + (gg*m2);
    for(j=3;j<n;j++){
      m2 = m1; m2 = atn(Ic,i,j-2);
      atn(Ic,i,j)= (g1*atm(I,j,i)) + (g2 * atn(Ic,i,j-1)) + (gg*atn(Ic,i,j-2));
    }
    // Filtre anticausal
      atn(Ia,i,n-1) = g1*atn(Ic,i,n-1);
      atn(Ia,i,n-2) = g1*atn(Ic,i,n-2) + g2*atn(Ia,i,n-1);

      
    m1 = atm(Ia,i,n-2); 
    m2 = atm(Ia,i,n-1);
    atn(Ia,i,n-3) = g1*atn(Ic,i,n-3) + g2*m1 + gg*m2;

    for (j=n-4;j>-1;j--){
      m1 = m2; m2 = atn(Ia,i,j+2);
      atn(Ia,i,j) = g1*atn(Ic,i,j) + g2*atn(Ia,i,j+1) + gg*atn(Ia,i,j+2);
    }
  }

  for(i=0;i<n;i++){
    for(j=0;j<m;j++){
      atm(Ic,i,j) = atn(Ia,j,i);
    }
  }

  free(Ib);
  free(Ia);

  return Ic;
}

void roberts(const double *I, double *Igx, double *Igy, int n, int m){
  int i,j;
  for (i = 0; i < m-1; i++) {
    for (j = 0; j < n-1; j++) {
      Igx[i + m * j] = ((-I[i + m * j] + I[(i + m * j) + 1]) - I[i + m * (1 + j)]) + I[(i + m * (1 + j)) + 1];
      Igy[i + m * j] = ((-I[i + m * j] + I[i + m * (1 + j)]) - I[(i + m * j) + 1]) + I[(i + m * (1 + j)) + 1];
    }
  }
}

double* hough_exh(uchar* Ib,int n,int m, int *truc){
  int maxrho = (int) ceil(sqrt(n*n+m*m));
  std::cout << maxrho << std::endl;
  double* Acc = (double*) calloc(180*maxrho,sizeof(double));
  *truc = maxrho;
  int i,j,angle;

  for(i=1;i<n-1;i++){
    for(j=1;j<m-1;j++){
      if(Ib[i*m+j] >0){
        for(angle=0;angle<180;angle++){
          double rad = angle*M_PI/180;
          double rho = j*cos(rad) + i*sin(rad);
          int rho_idx = round(rho/2 + maxrho*1.0/2);
          if(rho_idx>0){
            at(Acc,rho_idx,angle,180) = at(Acc,rho_idx,angle,180)+1;
          }
          else if(rho_idx<0){
            at(Acc,rho_idx+maxrho,angle,180)=at(Acc,rho_idx+maxrho,angle,180)+1;
          }
        }
      }
    }
  }
  return Acc;
}

int* imregionalmax(double* I, int n, int m){
  int* res = (int*) malloc(n*m*sizeof(int));
  int i,j;
  double MG = I[0];
  double MM = I[1];
  double MD = I[1];
  double BG = atm(I,1,0);
  double BM = atm(I,1,1);
  double BD = atm(I,1,2);
  double HG = MG;
  double HM = MM;
  double HD = MD;
  res[0] = (MM>=MD && MM>=BM && MM>=BD)? 1:0;
  for(i=1;i<m-1;i++){
    MG=MM; MM=MD; MD=I[i+1];
    BG=BM; BM=BG; BD=I[i+m+1];
    res[i] = (MM>=MG && MM>=MD && MM>BG && MM>=BM && MM>=BD)? 1:0;
  }
  res[m-1] = (MD>=MM && MD>=BM && MD>=BD);
  for(i=m;i<n*(m-1);i++){
    if(i%m==0){
      HM=I[i-m]; HD=I[i-m+1];
      MM=I[i];   MD=I[i+1];
      BM=I[i+m]; BD=I[i+m+1];
      res[i] = (MM>=HM && MM>=HD && MM>=MD && MM>=BM && MM>=BD)? 1:0;
    }else if(i%m==m-1){
      res[i] = (MD>=HM && MD>=HD && MD>=MM && MD>=BM && MD>=BD)? 1:0;
    }else{
      HG=HM;HM=HD;HD=I[i-m+1];
      MG=MM;MM=MD;MD=I[i+1];
      BG=BM;BM=BD;BD=I[i+m+1];
      res[i] = (MM>=HG && MM>=HM && MM>=HD && MM>=MG && MM>=MD && MM>=BG && MM>=BM && MM>=BD)? 1:0;
    }
  }
  HM=atm(I,n-2,0);
  HD=atm(I,n-2,1);
  MM=atm(I,n-1,0);
  MD=atm(I,n-1,1);
  atm(res,n-1,0) = (MM>=HM && MM>=HD && MM>=MD)? 1:0;
  for(i=n*(m-1)+1;i<n*m-1;i++){
    HG=HM;HM=HD;HD=I[i-m+1];
    MG=MM;MM=MD;MD=I[i+1];
    res[i] = (MM>=HG && MM>=HM && MM>=HD && MM>=MG && MM>=MD)? 1:0;
  }
  atm(res,n-1,m-1) = (MD>=HM && MD>=HD && MD>=MM)? 1:0;
  return res;
}


Mat hough_peaks_lines(double* Acc, Mat RGB, int votes, int maxrho2){
  Mat res(RGB);
  int n2 = res.rows;
  int m2 = res.cols;
  int n = 180;
  int m = maxrho2;
  int maxrho = ceil(sqrt(m*m + n*n));
  int i,j;
  for(i=0;i<m;i++){
    for(j=0;j<n;j++){
      if(atm(Acc,j,i)>votes){
        double rho = j*2-maxrho;
        double angle = i*1.0/180;
        if(angle>0){
          double mm = -cos(angle)/sin(angle);
          double c = rho/sin(angle);
          line(res,Point_<int>(1,mm+c),Point_<int>(n2,mm*n2+c),255);
        }

      }
    }
  }
  return res;
}