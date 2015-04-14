/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_hough_peaks_lines_api.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 14-Apr-2015 13:30:48
 */

/* Include Files */
#include "tmwtypes.h"
#include "_coder_hough_peaks_lines_api.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true, false, 131418U, NULL,
  "hough_peaks_lines", NULL, false, { 2045744189U, 2170104910U, 2743257031U,
    4284093946U }, NULL };

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void hough_peaks_lines_api(void)
{
  /* Invoke the target function */
  hough_peaks_lines();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void hough_peaks_lines_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  hough_peaks_lines_xil_terminate();
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void hough_peaks_lines_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void hough_peaks_lines_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_hough_peaks_lines_api.c
 *
 * [EOF]
 */
