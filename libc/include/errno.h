#ifndef _ERRNO_H_
#define _ERRNO_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <errno.h>.
   Last modified: Jule 17, 2024.
***************************************************************************** */

#define errno __errno

#define EDOM   __EDOM
#define ERANGE __ERANGE

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 199409L
   #define EILSEQ __EILSEQ
#endif /* __STDC_VERSION__ >= 199409L */
#endif /* defined(__STDC_VERSION__) */

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _ERRNO_H_ */
