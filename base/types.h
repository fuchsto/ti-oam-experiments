#ifndef DSP_IMG_FILTER__TYPES_H__INCLUDED
#define DSP_IMG_FILTER__TYPES_H__INCLUDED

typedef char CPicturePixel;

typedef struct CPicture_t
{
  CPicturePixel * data;
  CPicturePixel   thld;
  unsigned        rows;
  unsigned        cols;
}
CPicture;

typedef struct RunParameters_t
{
  CPicture        image_in;
  CPicture        image_op;
  CPicture        image_out;
  int             operator_0;
  int             operator_1;
  int             num_threads;
}
RunParameters;

typedef enum DSP_TARGET__RET_t
{
  DSP_TARGET__OK         =  0,
  DSP_TARGET__ERR_OTHER  = -1,
  DSP_TARGET__ERR_MALLOC = -2,
  DSP_TARGET__ERR_BUS    = -3
}
DSP_TARGET__RET;

typedef double ts_t;

typedef void (*StencilOperator)(
                  const CPicturePixel *, /* image_in_buf    */
                  CPicturePixel *,       /* image_out_buf   */
                  CPicturePixel,         /* image_threshold */
                  int,                   /* nrows           */
                  int,                   /* ncols           */
                  int,                   /* i               */
                  int,                   /* j               */
                  int                    /* core_id         */
              );

typedef void (*ReduceOperator)(
                  const CPicturePixel *, /* image_in_buf    */
                  const CPicturePixel *, /* image_op_buf    */
                  CPicturePixel *,       /* image_out_buf   */
                  CPicturePixel,         /* image_threshold */
                  int,                   /* nrows           */
                  int,                   /* ncols           */
                  int,                   /* i               */
                  int,                   /* j               */
                  int                    /* core_id         */
              );

#endif // DSP_IMG_FILTER__TYPES_H__INCLUDED
