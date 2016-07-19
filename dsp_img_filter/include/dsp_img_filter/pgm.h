#ifndef DSP_IMG_FILTER__PGM_H__INCLUDED
#define DSP_IMG_FILTER__PGM_H__INCLUDED

typedef struct _PGMData {
  int    rows;
  int    cols;
  int    thld;
  int ** data;
}
PGMData;


PGMData * pgm_new(
  int rows,
  int cols,
  int thld);

void pgm_delete(
  PGMData       * pgm);

PGMData * pgm_read(
  const char    * file_name);

int pgm_write(
  const PGMData * pgm,
  const char    * file_name);

#endif // DSP_IMG_FILTER__PGM_H__INCLUDED
