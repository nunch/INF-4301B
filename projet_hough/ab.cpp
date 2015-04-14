/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: hough_exh.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 13-Apr-2015 17:01:02
 */

/* Include Files */
 

/* Function Declarations */
static double rt_remd_snf(double u0, double u1);
static double rt_roundd_snf(double u);

/* Function Definitions */

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_remd_snf(double u0, double u1)
{
  double y;
  double b_u1;
  double tr;
  if (!((!rtIsNaN(u0)) && (!rtIsInf(u0)) && ((!rtIsNaN(u1)) && (!rtIsInf(u1)))))
  {
    y = rtNaN;
  } else {
    if (u1 < 0.0) {
      b_u1 = ceil(u1);
    } else {
      b_u1 = floor(u1);
    }

    if ((u1 != 0.0) && (u1 != b_u1)) {
      tr = u0 / u1;
      if (fabs(tr - rt_roundd_snf(tr)) <= DBL_EPSILON * fabs(tr)) {
        y = 0.0;
      } else {
        y = fmod(u0, u1);
      }
    } else {
      y = fmod(u0, u1);
    }
  }

  return y;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * Arguments    : const double Ib[307200]
 *                double Acc[144000]
 * Return Type  : void
 */
void hough_exh(const double Ib[307200], double Acc[144000])
{
  int i;
  int j;
  int angle;
  boolean_T exitg1;
  double x;
  signed char n;
  double b_x;
  signed char b_n;
  double c_n;
  double d_n;
  int rho_idx;

  /*  Accumulator initialization */

  /*  Compute Hough transform for every Ib(x,y)>0 */
  for (i = 0; i < 638; i++) {
    for (j = 0; j < 478; j++) {
      if (Ib[(i + 640 * (j + 1)) + 1] > 0.0) {
        angle = 0;
        exitg1 = false;
        while ((!exitg1) && (angle < 180)) {
          x = rt_remd_snf(1.0 + (double)angle, 360.0);
          if (x <= 45.0) {
            x *= 0.017453292519943295;
            n = 0;
          } else if (x <= 135.0) {
            if (x > 0.0) {
              x = 0.017453292519943295 * (x - 90.0);
              n = 1;
            } else {
              x = 1.5707963267948966;
              n = -1;
            }
          } else if (x > 0.0) {
            x = 0.017453292519943295 * (x - 180.0);
            n = 2;
          } else {
            x = 3.1415926535897931;
            n = -2;
          }

          b_x = rt_remd_snf(1.0 + (double)angle, 360.0);
          if (b_x <= 45.0) {
            b_x *= 0.017453292519943295;
            b_n = 0;
          } else if (b_x <= 135.0) {
            if (b_x > 0.0) {
              b_x = 0.017453292519943295 * (b_x - 90.0);
              b_n = 1;
            } else {
              b_x = 1.5707963267948966;
              b_n = -1;
            }
          } else if (b_x > 0.0) {
            b_x = 0.017453292519943295 * (b_x - 180.0);
            b_n = 2;
          } else {
            b_x = 3.1415926535897931;
            b_n = -2;
          }

          if (n == 0) {
            c_n = cos(x);
          } else if (n == 1) {
            c_n = -sin(x);
          } else if (n == -1) {
            c_n = sin(x);
          } else {
            c_n = -cos(x);
          }

          if (b_n == 0) {
            d_n = sin(b_x);
          } else if (b_n == 1) {
            d_n = cos(b_x);
          } else if (b_n == -1) {
            d_n = -cos(b_x);
          } else {
            d_n = -sin(b_x);
          }

          rho_idx = (int)rt_roundd_snf(((2.0 + (double)j) * c_n + (2.0 + (double)
            i) * d_n) / 2.0 + 400.0);

          /* if rho_idx ~= 0, */
          if (rho_idx > 0) {
            Acc[angle + 180 * (rho_idx - 1)]++;
            angle++;
          } else {
            exitg1 = true;
          }
        }
      }
    }
  }
}

/*
 * File trailer for hough_exh.c
 *
 * [EOF]
 */
