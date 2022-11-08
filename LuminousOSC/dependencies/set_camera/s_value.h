/*
  �萔�̒�`�t�@�C�� Ver.1.04
*/

/* TRUE,FALSE���` */
#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

/* ���w��̒萔���` */
#ifndef M_PI
#define M_PI            3.14159265358979323846  /* pi */
#endif
#ifndef M_E
#define M_E             2.7182818284590452354   /* e */
#endif

/* �t�@�C�����̍ő咷�� MAXPATHLEN���` */
#ifndef         MAXPATHLEN
/* _WINDOWS �̎��� MAX_PATH ���L�� */
#ifdef  MAX_PATH
#define MAXPATHLEN      MAX_PATH
#else
#define MAXPATHLEN      256
#endif
#endif

/* Windows���L�̐ݒ� */
#ifdef _WIN32
/* strcpy()����Warning�}�~ */
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif
#endif

/* �@��ʂ̐ݒ� */
// MS-DOS�̎��F __MSDOS__
// Windows�̎��F _WIN32,_CONSOLE
// Windows�̎��F _WIN32,_WINDOWS
// FreeBSD�̎��F __unix__,__FreeBSD__
// linux�̎��F   __unix__,__linux__
