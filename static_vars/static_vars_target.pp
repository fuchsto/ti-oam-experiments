#define __signed_chars__ 1	/* Predefined */
#define _OPENMP 200805	/* Predefined */
#define __DATE__ "Jul 25 2016"	/* Predefined */
#define __TIME__ "01:31:39"	/* Predefined */
#define __STDC__ 1	/* Predefined */
#define __STDC_VERSION__ 199409L	/* Predefined */
#define __TI_INT40_T__ 1	/* Predefined */
#define __TI_C99_COMPLEX_ENABLED__ 1	/* Predefined */
#define __edg_front_end__ 1	/* Predefined */
#define __EDG_VERSION__ 404	/* Predefined */
#define __EDG_SIZE_TYPE__ unsigned int	/* Predefined */
#define __EDG_PTRDIFF_TYPE__ int	/* Predefined */
#define __GNUC_GNU_INLINE__ 1	/* Predefined */
#define __GNUC_MINOR__ 3	/* Predefined */
#define __GNUC_PATCHLEVEL__ 0	/* Predefined */
#define __VERSION__ "EDG gcc 4.3 mode"	/* Predefined */
#define __TI_COMPILER_VERSION__ 8001000	/* Predefined */
#define __COMPILER_VERSION__ 8001000	/* Predefined */
#define _TMS320C6X 1	/* Predefined */
#define __TMS320C6X__ 1	/* Predefined */
#define _TMS320C6600 1	/* Predefined */
#define _TMS320C6740 1	/* Predefined */
#define _TMS320C6700_PLUS 1	/* Predefined */
#define _TMS320C67_PLUS 1	/* Predefined */
#define _TMS320C6700 1	/* Predefined */
#define _TMS320C64_PLUS 1	/* Predefined */
#define _TMS320C6400_PLUS 1	/* Predefined */
#define _TMS320C6400 1	/* Predefined */
#define _LITTLE_ENDIAN 1	/* Predefined */
#define __TI_EABI__ 1	/* Predefined */
#define __TI_ELFABI__ 1	/* Predefined */
#define __TI_TLS__ 1	/* Predefined */
#define __TI_USE_TLS 1	/* Predefined */
#define __TI_32BIT_LONG__ 1	/* Predefined */
#define __SIZE_T_TYPE__ unsigned	/* Predefined */
#define __PTRDIFF_T_TYPE__ int	/* Predefined */
#define __WCHAR_T_TYPE__ unsigned short	/* Predefined */
#define __little_endian__ 1	/* Predefined */
#define __TI_STRICT_ANSI_MODE__ 0	/* Predefined */
#define __TI_WCHAR_T_BITS__ 16	/* Predefined */
#define __TI_GNU_ATTRIBUTE_SUPPORT__ 1	/* Predefined */
#define __TI_STRICT_FP_MODE__ 1	/* Predefined */
#define _INLINE 1
#define COMM_ASYNC_TARGET_H_ 	/* static_vars_target.h */
#define BASE__OAM_TYPES_H__INCLUDED 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_types.h */
#define DSP_IMG_FILTER__CONFIG_H_ 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_config.h */
#define ARCH_BOGOMIPS 1229	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_config.h */
#define OAM_TASK__MAX_THREADS 2	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_config.h */
#define SYMMETRIC_MEMORY__USE_DDR 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_config.h */
#define DSP_IMG_FILTER__MACRO_H__INCLUDED 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h */
#define wno_unused_var_(var) (void)(var)	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h */
#define swap_(x,y) do { unsigned char tmp[sizeof(x) == sizeof(y) ? (signed)(sizeof(x)) : -1]; memcpy(tmp, &y, sizeof(x)); memcpy(&y, &x, sizeof(x)); memcpy(&x, tmp, sizeof(x)); } while (0)	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h */
#define rint_(f) ((int)(f))	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h */
#define min_(a,b) ((a) < (b) ? (a) : (b))	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h */
#define BASE__OAM_TASK_H__INCLUDED 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_task.h */
#define BASE__OAM_TASK_TARGET_H__INCLUDED 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_task_target.h */
#define DSP_IMG_FILTER__LOGGING_H__INCLUDED 	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h */
#define LOG_HOST__FILE_FIELD_WIDTH 32	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h */
#define LOG_HOST__LINE_FIELD_WIDTH 5	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h */
#define LOG_HOST__MAX_MESSAGE_SIZE 128	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h */
#define LOG_HOST_DEBUG(__VA_ARGS__...) do { char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; int sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); if (sn_ret < 0) { break; } const char * path = __FILE__; char * base = strrchr(path, '/'); printf( "[ DEBUG ] [  HOST  ] %-*s:%-*d | %s\n", LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), LOG_HOST__LINE_FIELD_WIDTH, __LINE__, msg_buf); } while (0)	/* /home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h */
#define NValues 100	/* static_vars_target.c */
#define _STDLIB 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define __ATTRIBUTE(attr) __attribute__(attr)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define _LLONG_AVAILABLE 1	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define MB_CUR_MAX 1	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define NULL 0	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define _SIZE_T 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define _WCHAR_T 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define EXIT_FAILURE 1	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define EXIT_SUCCESS 0	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define RAND_MAX 32767	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h */
#define _LINKAGE 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _CODE_ACCESS 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _DATA_ACCESS __far	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _DATA_ACCESS_NEAR __near	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _IDECL static __inline	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _IDEFN static __inline	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h */
#define _STDIO 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _STDARG 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define va_end(_ap) ((void)0)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define va_start(_ap,_parmN) (_ap = ((char *)__va_parmadr(_parmN)) + (__va_argref(_parmN) ? sizeof(&_parmN) : sizeof(_parmN)))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_SIZEOF_SCALAR_STR(_type) ((sizeof(_type) > 8) ? 16 : (sizeof(_type) > 4) ? 8 : (sizeof(_type) > 2) ? 4 : (sizeof(_type) > 1) ? 2 : 1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_ALIGNOF_SCALAR_STR(_type) ((sizeof(_type) > 8) ? 16 : (sizeof(_type) > 4) ? 8 : (sizeof(_type) > 2) ? 4 : (sizeof(_type) > 1) ? 2 : 1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_ALIGNOF_PROMOTED_TYPE(_type) (__va_argref(_type) ? __alignof__(_type *) : __va_argcsu(_type) ? __TI_ALIGNOF_SCALAR_STR(_type) : (sizeof(_type) < 4) ? __alignof__(int) : __alignof__(_type))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_ALIGN(e,_type) ((char *)(((unsigned)(e) + __TI_ALIGNOF_PROMOTED_TYPE(_type) - 1) & ~(__TI_ALIGNOF_PROMOTED_TYPE(_type) - 1)))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_SIZEOF_PROMOTED_TYPE(_type) (__va_argref(_type) ? sizeof(_type *) : __va_argcsu(_type) ? __TI_SIZEOF_SCALAR_STR(_type) : (sizeof(_type) < 4) ? sizeof(int) : sizeof(_type))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define __TI_DEREF(e,_type) (__va_argref(_type) ? **(_type **)(e) : *(_type *)(e))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define va_arg(_ap,_type) (_ap = __TI_ALIGN(_ap, _type), _ap += __TI_SIZEOF_PROMOTED_TYPE(_type), __TI_DEREF(_ap - __TI_SIZEOF_PROMOTED_TYPE(_type), _type))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h */
#define _FPOS_T 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _IOFBF 0x0001	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _IOLBF 0x0002	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _IONBF 0x0004	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _BUFFALOC 0x0008	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _MODER 0x0010	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _MODEW 0x0020	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _MODERW 0x0040	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _MODEA 0x0080	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _MODEBIN 0x0100	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _STATEOF 0x0200	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _STATERR 0x0400	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _UNGETC 0x0800	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _TMPFILE 0x1000	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _SET(_fp,_b) (((_fp)->flags) |= (_b))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _UNSET(_fp,_b) (((_fp)->flags) &= ~(_b))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _STCHK(_fp,_b) (((_fp)->flags) & (_b))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _BUFFMODE(_fp) (((_fp)->flags) & (_IOFBF | _IOLBF | _IONBF))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _ACCMODE(_fp) (((_fp)->flags) & (_MODER | _MODEW))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define BUFSIZ 256	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define FOPEN_MAX _NFILE	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define FILENAME_MAX 256	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define TMP_MAX 65535	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define stdin (&_ftable[0])	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define stdout (&_ftable[1])	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define stderr (&_ftable[2])	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define L_tmpnam _LTMPNAM	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define SEEK_SET (0x0000)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define SEEK_CUR (0x0001)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define SEEK_END (0x0002)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define EOF (-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define P_tmpdir ""	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _NFILE 20	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _LTMPNAM 16	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _getchar() getc(stdin)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _putchar(_x) putc((_x), stdout)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _clearerr(_fp) ((void) ((_fp)->flags &= ~(_STATERR | _STATEOF)))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _ferror(_x) ((_x)->flags & _STATERR)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define _remove(_fl) (unlink((_fl)))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h */
#define OMP_H_ 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/omp.h */
#define _STDINT_H_ 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT8_MAX 0x7f	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT8_MIN (-INT8_MAX-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT8_MAX 0xff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT16_MAX 0x7fff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT16_MIN (-INT16_MAX-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT16_MAX 0xffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT32_MAX 0x7fffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT32_MIN (-INT32_MAX-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT32_MAX 0xffffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT40_MAX 0x7fffffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT40_MIN (-INT40_MAX-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT40_MAX 0xffffffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT64_MAX 0x7fffffffffffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT64_MIN (-INT64_MAX-1)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT64_MAX 0xffffffffffffffff	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST8_MAX (INT8_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST8_MIN (INT8_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_LEAST8_MAX (UINT8_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST16_MAX (INT16_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST16_MIN (INT16_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_LEAST16_MAX (UINT16_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST32_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST32_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_LEAST32_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST40_MAX (INT40_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST40_MIN (INT40_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_LEAST40_MAX (UINT40_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST64_MAX (INT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_LEAST64_MIN (INT64_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_LEAST64_MAX (UINT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST8_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST8_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_FAST8_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST16_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST16_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_FAST16_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST32_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST32_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_FAST32_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST40_MAX (INT40_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST40_MIN (INT40_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_FAST40_MAX (UINT40_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST64_MAX (INT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT_FAST64_MIN (INT64_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT_FAST64_MAX (UINT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INTPTR_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INTPTR_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINTPTR_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INTMAX_MIN (INT64_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INTMAX_MAX (INT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINTMAX_MAX (UINT64_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define PTRDIFF_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define PTRDIFF_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define SIG_ATOMIC_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define SIG_ATOMIC_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define SIZE_MAX (UINT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define WCHAR_MAX 0xffffu	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define WCHAR_MIN 0	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define WINT_MIN (INT32_MIN)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define WINT_MAX (INT32_MAX)	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT8_C(value) ((int_least8_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT8_C(value) ((uint_least8_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT16_C(value) ((int_least16_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT16_C(value) ((uint_least16_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT32_C(value) ((int_least32_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT32_C(value) ((uint_least32_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT40_C(value) ((int_least40_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT40_C(value) ((uint_least40_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INT64_C(value) ((int_least64_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINT64_C(value) ((uint_least64_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define INTMAX_C(value) ((intmax_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define UINTMAX_C(value) ((uintmax_t)(value))	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h */
#define _TI_OMP_DEVICE_H_ 	/* /opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/ti_omp_device.h */
