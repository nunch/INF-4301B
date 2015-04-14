#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

#define atn(i,a, b) i[a*n+b]
#define atm(i,a, b) i[a*m+b]
#define at(i,a, b, c) i[a*c+b]

using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

double* deriche_GL(uchar* I, double gamma, int n, int m);
void roberts(double *Is, double *Igx, double *Igy, int n, int m);
double* hough_exh(uchar*Ib,int n,int m, int *truc);
int* imregionalmax(double* Acc, int n, int m);
Mat hough_peaks_lines(double* Acc, Mat RGB, int votes);
int main(int argc, char const *argv[])
{
	Mat mat = imread("lena.jpg",1);
	
	Mat ii;
	cvtColor(mat, ii, CV_RGB2GRAY);
	namedWindow( "Original image", WINDOW_AUTOSIZE );
	namedWindow( "Smoothed image", WINDOW_AUTOSIZE );
	namedWindow( "Gradient", WINDOW_AUTOSIZE );
	namedWindow( "Binary contours", WINDOW_AUTOSIZE );
	namedWindow( "Houhgh accumulator space", WINDOW_AUTOSIZE );
	imshow( "Original image", ii ); 
	
	double gamma = 0.2;
	int th  = 160;
	int votes = 100;
	//Mat Is = smoothing_GL(I,gamma);
	int n = ii.rows;
	int m = ii.cols;
	uchar* I =ii.data;
	double *Is = deriche_GL(I,gamma,n,m);

	double *Igx = (double*) malloc(n*m*sizeof(double));
	double *Igy = (double*) malloc(n*m*sizeof(double));
	roberts(Is,Igx,Igy,n,m);
	int i;
	double *Ig = (double*) malloc(n*m*sizeof(double));
	uchar* Ib = (uchar*) malloc(n*m*sizeof(uchar));
	for(i=0;i<n*m;i++) {
		double t1 = Igx[i];
		double t2 = Igy[i];
		double t = t1*t1+t2*t2;
		Ig[i] = (t>255)?255:t;
		Ib[i]=(t>th)?255:0;
	}



	/*int maxrho;
	double* Acc = hough_exh(Ib,n,m,&maxrho);

	int* Accmax = imregionalmax(Acc,n,m);

	for(i=0;i<n*m;i++){
		if(Accmax[i]==0) Acc[i]=0;
	}
	*/
	Mat IIs = ii.clone();
	Mat IIg = ii.clone();
	Mat IIb = ii.clone();
	for(i=0;i<n*m;i++){
		int t = Is[i];
		IIs.data[i] = (t>255)? 255:t;
		std::cout<<IIs.data[i]<<std::endl;
		IIg.data[i] = Igx[i];
		IIb.data[i] = Ib[i];
	}

	imshow( "Smoothed image",IIs);
	imshow( "Gradient",IIg);
	imshow( "Binary contours",IIb);
	waitKey(0);
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
	
	for (i=0;i<n;i++) {
		atm(Ic,i,0) = g1*atm(I,i,0);
		atm(Ic,i,1) = g1*atm(I,i,1)+ (g2 * atm(Ic,i,0));
		for(j=2;j<m;j++){
		   atm(Ic,i,j)= (g1*atm(I,i,j)) + (g2 * atm(Ic,i,j-1)) + (gg*atm(Ic,i,j-2));
		}
		// Filtre anticausal
			atm(Ia,i,n-1) = g1*atm(Ic,i,n-1);
			atm(Ia,i,n-2) = g1*atm(Ic,i,n-2) + g2*atm(Ia,i,n-1);
		for (j=m-3;j>-1;j--){
			atm(Ia,i,j) = g1*atm(Ic,i,j) + g2*atm(Ia,i,j+1) + gg*atm(Ia,i,j+2);
		}
	}
	for(i=0;i<n;i++) for(j=0;j<m;j++)
		atn(Ib,j,i) =atm(Ia,i,j);

	for (i=0;i<m;i++) {
		atn(Ic,i,0) = g1*atn(Ib,i,0);
		atn(Ic,i,1) = g1*atn(Ib,i,1)+ (g2 * atn(Ic,i,0));
		for(j=2;j<n;j++){
		   atn(Ic,i,j)= (g1*atn(Ib,i,j)) + (g2 * atn(Ic,i,j-1)) + (gg*atn(Ic,i,j-2));
		}
		// Filtre anticausal
			atn(Ia,i,n-1) = g1*atn(Ic,i,n-1);
			atn(Ia,i,n-2) = g1*atn(Ic,i,n-2) + g2*atn(Ia,i,n-1);
		for (j=n-3;j>-1;j--){
			atn(Ia,i,j) = g1*atn(Ic,i,j) + g2*atn(Ia,i,j+1) + gg*atn(Ia,i,j+2);
		}
	}

	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			atm(Ic,j,i) = atn(Ia,i,j);
		}
	}
	//for(i=0;i<n*m;i++) Ia[i] = Ic[i];

	free(Ib);
	free(Ia);

	return Ic;
}

void roberts(double *Is, double *Igx, double *Igy, int n, int m){
	int i,j;
	for(i=1;i<n-1;i++){
		for(j=1;j<m-1;j++){
			atm(Igx,i,j) = -atm(Is,i,j) + atm(Is,i+1,j) - atm(Is,i,j+1) +atm(Is,i+1,j+1);
			atm(Igy,i,j) = -atm(Is,i,j) - atm(Is,i+1,j) + atm(Is,i,j+1) +atm(Is,i+1,j+1);
		}
	}
}

double* hough_exh(uchar* Ib,int n,int m, int *truc){
	int maxrho = (int) ceil(sqrt(n*n+m*m));
	double* Acc = (double*) malloc(180*maxrho*sizeof(double));
	*truc = maxrho;
	int i,j,angle;

	for(i=1;i<n-1;i++){
		for(j=1;m-1;j++){
			if(atm(Ib,i,j) >0){
				for(angle=0;angle<180;angle++){
					double rad = angle*1.0/180;
					double rho = j*cos(rad) + i*sin(rad);
					int rho_idx = round(rho/2 + maxrho/2);
					if(rho_idx>0){
						if(angle*maxrho+rho_idx>180*maxrho) std::cout<< "("<<angle<<","<<rho_idx<<")\n";
						at(Acc,angle,rho_idx,maxrho)++;
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


Mat hough_peaks_lines(double* Acc, Mat RGB, int votes){
	Mat res(RGB.rows,RGB.cols,CV_8UC3);
	int n = res.rows;
	int m = res.cols;
	int maxrho = ceil(sqrt(m*m + n*n));
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			if(atm(Acc,i,j)>votes){
				double rho = j*2-maxrho;
				double angle = i*1.0/180;
				if(angle>0){
					double m = -cos(angle)/sin(angle);
					double c = rho/sin(angle);
					line(res,Point_<int>(0,m),Point_<int>(n+c,m*n+c),CV_RGB(255,255,255));
				}
			}
		}
	}
	//line(res,Point_<int>(0,0),Point_<int>(500,500),CV_RGB(255,255,255));
	return res;
}