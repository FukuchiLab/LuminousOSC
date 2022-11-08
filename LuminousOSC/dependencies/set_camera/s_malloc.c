/************************************************************************
 s_malloc �T�C�o���� malloc�֌W�����T�u���[�`�� Ver.1.06c
 ************************************************************************
 �o��
 Ver.1.00(95/08/18)	s_string�n��蕪��
 Ver.1.01(95/10/25)	xmalloc�̃G���[�\����ύX
 Ver.1.02(96/06/14)	memset��ǉ�(0�ŏ���������)
 Ver.1.03(98/07/21)	malloc->calloc�ɕύX
 Ver.1.04(99/07/07)	xalloc_char(NULL)�Ή�
 Ver.1.05(08/09/12)	xalloc_char_se()�ǉ�,string.h��ˑ���
 Ver.1.06(11/01/18)	xalloc_char�n���𕪗�,_WINDOWS�Ή�
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"s_malloc.h"
#ifdef _WINDOWS
#include	<windows.h>
#endif

/* ���S�΍􂵂�malloc */
void *s_malloc(unsigned int n, size_t t)
{
  void *point;
  point = calloc (n, t);
  if (point == NULL)
    {
#ifdef _WINDOWS
      MessageBox(NULL,"s_malloc",
		 "s_malloc : Insufficient Memory. Stop.",MB_OK);
#else
#if 01
      fputs("s_malloc : Insufficient Memory. Stop.\n", stderr);
#else
      // printf�̕����v���O�����T�C�Y���ł����Ȃ邪�E�E�E�Ӗ��Ȃ�����
      printf("s_malloc : Insufficient Memory. (allocate %d x %d bytes)\n",
	     t, n);
#endif
#endif
      exit (2);
    }
#if 0
  printf("Allocated Memory. (allocate %d x %d bytes)\n", t, n);
#endif
  memset(point, 0, n * t);	/* 0�ŏ����� */
  return(point);
}
