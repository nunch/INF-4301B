/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_hough_peaks_lines_mex.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 14-Apr-2015 13:30:48
 */

/* Include Files */
#include "_coder_hough_peaks_lines_api.h"
#include "_coder_hough_peaks_lines_mex.h"

/* Function Declarations */
static void hough_peaks_lines_mexFunction(int32_T nlhs, int32_T nrhs);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                int32_T nrhs
 * Return Type  : void
 */
static void hough_peaks_lines_mexFunction(int32_T nlhs, int32_T nrhs)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 0, 4,
                        17, "hough_peaks_lines");
  }

  if (nlhs > 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 17,
                        "hough_peaks_lines");
  }

  /* Call the function. */
  hough_peaks_lines_api();

  /* Module termination. */
  hough_peaks_lines_terminate();
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray * const plhs[]
 *                int32_T nrhs
 *                const mxArray * const prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  (void)plhs;
  (void)prhs;

  /* Initialize the memory manager. */
  mexAtExit(hough_peaks_lines_atexit);

  /* Module initialization. */
  hough_peaks_lines_initialize();

  /* Dispatch the entry-point. */
  hough_peaks_lines_mexFunction(nlhs, nrhs);
}

/*
 * File trailer for _coder_hough_peaks_lines_mex.c
 *
 * [EOF]
 */
