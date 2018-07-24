#ifndef OAM__OAM_PROFILING_H__INCLUDED
#define OAM__OAM_PROFILING_H__INCLUDED

#if 0

#ifdef __cplusplus
extern "C" {
#endif

#if MV_PROFILING__ENABLE

void __entry_hook(const char *name);
void __exit_hook(const char *name);

#endif /* MV_PROFILING__ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif /* OAM__OAM_PROFILING_H__INCLUDED */
