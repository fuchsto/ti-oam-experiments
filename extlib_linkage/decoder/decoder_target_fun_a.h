#ifndef TASK_EXTLIB__DECODER__DECODER_TARGET_FUN_A_H__INCLUDED
#define TASK_EXTLIB__DECODER__DECODER_TARGET_FUN_A_H__INCLUDED

#pragma omp declare target
int decoder_fun_a(int value);
#pragma omp end declare target

#endif /* TASK_EXTLIB__DECODER__DECODER_TARGET_FUN_A_H__INCLUDED */
