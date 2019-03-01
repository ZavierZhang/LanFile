
//#include "oicqcrypt.h"

#ifndef _WIN32
#include <string.h>
#ifndef ULONG
#define ULONG unsigned long
#endif
#endif

#ifdef _WIN32
#include <atlbase.h>
#else
#ifndef bool
#define bool int
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#include <stdio.h>
#include <stdlib.h>

#endif 

#define MD5_LBLOCK	16
typedef struct MD5state_st
	{
	unsigned long A,B,C,D;
	unsigned long Nl,Nh;
	unsigned long data[MD5_LBLOCK];
	int num;
	} MD5_CTX; 

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

inline void MD5_Init(MD5_CTX *c)
{
	memset((void*)c,0,sizeof(MD5_CTX));
	c->A=INIT_DATA_A;
	c->B=INIT_DATA_B;
	c->C=INIT_DATA_C;
	c->D=INIT_DATA_D;
	c->Nl=0;
	c->Nh=0;
	c->num=0;
}



/*
	MD5鏇存柊
*/
#define MD5_CBLOCK	64

#define c2l(c,l)	(l = ((unsigned long)(*((c)++)))     , l|=(((unsigned long)(*((c)++)))<< 8), l|=(((unsigned long)(*((c)++)))<<16), 	 l|=(((unsigned long)(*((c)++)))<<24))

#define p_c2l(c,l,n)	{ switch (n) { 	case 0: l =((unsigned long)(*((c)++))); case 1: l|=((unsigned long)(*((c)++)))<< 8; case 2: l|=((unsigned long)(*((c)++)))<<16; case 3: l|=((unsigned long)(*((c)++)))<<24; } 			}

#define p_c2l_p(c,l,sc,len) { switch (sc) { case 0: l =((unsigned long)(*((c)++))); if (--len == 0) break; case 1: l|=((unsigned long)(*((c)++)))<< 8; if (--len == 0) break; case 2: l|=((unsigned long)(*((c)++)))<<16; } }

#define c2l_p(c,l,n)	{ l=0; (c)+=n; switch (n) { case 3: l =((unsigned long)(*(--(c))))<<16; case 2: l|=((unsigned long)(*(--(c))))<< 8; case 1: l|=((unsigned long)(*(--(c))))    ; } }


#define	F(b,c,d)	((((c) ^ (d)) & (b)) ^ (d))
#define	G(b,c,d)	((((b) ^ (c)) & (d)) ^ (c))
#define	H(b,c,d)	((b) ^ (c) ^ (d))
#define	I(b,c,d)	(((~(d)) | (b)) ^ (c))

/*
鍦╓in32涓嬪姞閫?
#if defined(WIN32)
#define ROTATE(a,n)     _lrotl(a,n)
#else
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffffL)>>(32-(n))))
#endif
*/


#if defined(WIN32)
#define ROTATE(a,n)     (((a)<<(n))|((a)>>(32-(n))))
#else
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffffL)>>(32-(n))))
#endif


#define R0(a,b,c,d,k,s,t) { a+=((k)+(t)+F((b),(c),(d))); a=ROTATE(a,s); a+=b; };

#define R1(a,b,c,d,k,s,t) { a+=((k)+(t)+G((b),(c),(d))); a=ROTATE(a,s); a+=b; };

#define R2(a,b,c,d,k,s,t) { a+=((k)+(t)+H((b),(c),(d))); a=ROTATE(a,s); a+=b; };

#define R3(a,b,c,d,k,s,t) { a+=((k)+(t)+I((b),(c),(d))); a=ROTATE(a,s); a+=b; };

#ifndef MD5_ASM

inline void md5_block(MD5_CTX *c, register ULONG *X, int num)
{
	register ULONG A,B,C,D;
	
	A=c->A;
	B=c->B;
	C=c->C;
	D=c->D;
	for (;;)
	{
		num-=64;
		if (num < 0) break;
		/* Round 0 */
		R0(A,B,C,D,X[ 0], 7,0xd76aa478L);
		R0(D,A,B,C,X[ 1],12,0xe8c7b756L);
		R0(C,D,A,B,X[ 2],17,0x242070dbL);
		R0(B,C,D,A,X[ 3],22,0xc1bdceeeL);
		R0(A,B,C,D,X[ 4], 7,0xf57c0fafL);
		R0(D,A,B,C,X[ 5],12,0x4787c62aL);
		R0(C,D,A,B,X[ 6],17,0xa8304613L);
		R0(B,C,D,A,X[ 7],22,0xfd469501L);
		R0(A,B,C,D,X[ 8], 7,0x698098d8L);
		R0(D,A,B,C,X[ 9],12,0x8b44f7afL);
		R0(C,D,A,B,X[10],17,0xffff5bb1L);
		R0(B,C,D,A,X[11],22,0x895cd7beL);
		R0(A,B,C,D,X[12], 7,0x6b901122L);
		R0(D,A,B,C,X[13],12,0xfd987193L);
		R0(C,D,A,B,X[14],17,0xa679438eL);
		R0(B,C,D,A,X[15],22,0x49b40821L);
		/* Round 1 */
		R1(A,B,C,D,X[ 1], 5,0xf61e2562L);
		R1(D,A,B,C,X[ 6], 9,0xc040b340L);
		R1(C,D,A,B,X[11],14,0x265e5a51L);
		R1(B,C,D,A,X[ 0],20,0xe9b6c7aaL);
		R1(A,B,C,D,X[ 5], 5,0xd62f105dL);
		R1(D,A,B,C,X[10], 9,0x02441453L);
		R1(C,D,A,B,X[15],14,0xd8a1e681L);
		R1(B,C,D,A,X[ 4],20,0xe7d3fbc8L);
		R1(A,B,C,D,X[ 9], 5,0x21e1cde6L);
		R1(D,A,B,C,X[14], 9,0xc33707d6L);
		R1(C,D,A,B,X[ 3],14,0xf4d50d87L);
		R1(B,C,D,A,X[ 8],20,0x455a14edL);
		R1(A,B,C,D,X[13], 5,0xa9e3e905L);
		R1(D,A,B,C,X[ 2], 9,0xfcefa3f8L);
		R1(C,D,A,B,X[ 7],14,0x676f02d9L);
		R1(B,C,D,A,X[12],20,0x8d2a4c8aL);
		/* Round 2 */
		R2(A,B,C,D,X[ 5], 4,0xfffa3942L);
		R2(D,A,B,C,X[ 8],11,0x8771f681L);
		R2(C,D,A,B,X[11],16,0x6d9d6122L);
		R2(B,C,D,A,X[14],23,0xfde5380cL);
		R2(A,B,C,D,X[ 1], 4,0xa4beea44L);
		R2(D,A,B,C,X[ 4],11,0x4bdecfa9L);
		R2(C,D,A,B,X[ 7],16,0xf6bb4b60L);
		R2(B,C,D,A,X[10],23,0xbebfbc70L);
		R2(A,B,C,D,X[13], 4,0x289b7ec6L);
		R2(D,A,B,C,X[ 0],11,0xeaa127faL);
		R2(C,D,A,B,X[ 3],16,0xd4ef3085L);
		R2(B,C,D,A,X[ 6],23,0x04881d05L);
		R2(A,B,C,D,X[ 9], 4,0xd9d4d039L);
		R2(D,A,B,C,X[12],11,0xe6db99e5L);
		R2(C,D,A,B,X[15],16,0x1fa27cf8L);
		R2(B,C,D,A,X[ 2],23,0xc4ac5665L);
		/* Round 3 */
		R3(A,B,C,D,X[ 0], 6,0xf4292244L);
		R3(D,A,B,C,X[ 7],10,0x432aff97L);
		R3(C,D,A,B,X[14],15,0xab9423a7L);
		R3(B,C,D,A,X[ 5],21,0xfc93a039L);
		R3(A,B,C,D,X[12], 6,0x655b59c3L);
		R3(D,A,B,C,X[ 3],10,0x8f0ccc92L);
		R3(C,D,A,B,X[10],15,0xffeff47dL);
		R3(B,C,D,A,X[ 1],21,0x85845dd1L);
		R3(A,B,C,D,X[ 8], 6,0x6fa87e4fL);
		R3(D,A,B,C,X[15],10,0xfe2ce6e0L);
		R3(C,D,A,B,X[ 6],15,0xa3014314L);
		R3(B,C,D,A,X[13],21,0x4e0811a1L);
		R3(A,B,C,D,X[ 4], 6,0xf7537e82L);
		R3(D,A,B,C,X[11],10,0xbd3af235L);
		R3(C,D,A,B,X[ 2],15,0x2ad7d2bbL);
		R3(B,C,D,A,X[ 9],21,0xeb86d391L);
		
		A+=c->A&0xffffffffL;
		B+=c->B&0xffffffffL;
		c->A=A;
		c->B=B;
		C+=c->C&0xffffffffL;
		D+=c->D&0xffffffffL;
		c->C=C;
		c->D=D;
		X+=16;
	}
}
#endif


inline void MD5_Update(MD5_CTX *c, const register unsigned char *data, unsigned long len)
{
	register ULONG *p;
	int sw,sc;
	ULONG l;
	
	if (len == 0) return;
	
	l=(c->Nl+(len<<3))&0xffffffffL;
	/* 95-05-24 eay Fixed a bug with the overflow handling, thanks to
	* Wei Dai <weidai@eskimo.com> for pointing it out. */
	if (l < c->Nl) /* overflow */
		c->Nh++;
	c->Nh+=(len>>29);
	c->Nl=l;
	
	if (c->num != 0)
	{
		p=c->data;
		sw=c->num>>2;
		sc=c->num&0x03;
		
		if ((c->num+len) >= MD5_CBLOCK)
		{
			l= p[sw];
			p_c2l(data,l,sc);
			p[sw++]=l;
			for (; sw<MD5_LBLOCK; sw++)
			{
				c2l(data,l);
				p[sw]=l;
			}
			len-=(MD5_CBLOCK-c->num);
			
			md5_block(c,p,64);
			c->num=0;
			/* drop through and do the rest */
		}
		else
		{
			int ew,ec;
			
			c->num+=(int)len;
			if ((sc+len) < 4) /* ugly, add char's to a word */
			{
				l= p[sw];
				p_c2l_p(data,l,sc,len);
				p[sw]=l;
			}
			else
			{
				ew=(c->num>>2);
				ec=(c->num&0x03);
				l= p[sw];
				p_c2l(data,l,sc);
				p[sw++]=l;
				for (; sw < ew; sw++)
				{ c2l(data,l); p[sw]=l; }
				if (ec)
				{
					c2l_p(data,l,ec);
					p[sw]=l;
				}
			}
			return;
		}
	}

	/* we now can process the input data in blocks of MD5_CBLOCK
	* chars and save the leftovers to c->data. */
#ifdef L_ENDIAN
	if ((((unsigned int)data)%sizeof(ULONG)) == 0)
	{
		sw=len/MD5_CBLOCK;
		if (sw > 0)
		{
			sw*=MD5_CBLOCK;
			md5_block(c,(ULONG *)data,sw);
			data+=sw;
			len-=sw;
		}
	}
#endif
	p=c->data;
	while (len >= MD5_CBLOCK)
	{
#if defined(L_ENDIAN) || defined(B_ENDIAN)
		if (p != (unsigned long *)data)
			memcpy(p,data,MD5_CBLOCK);
		data+=MD5_CBLOCK;
#ifdef B_ENDIAN
		for (sw=(MD5_LBLOCK/4); sw; sw--)
		{
			Endian_Reverse32(p[0]);
			Endian_Reverse32(p[1]);
			Endian_Reverse32(p[2]);
			Endian_Reverse32(p[3]);
			p+=4;
		}
#endif
#else
		for (sw=(MD5_LBLOCK/4); sw; sw--)
		{
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l; 
		} 
#endif
		p=c->data;
		md5_block(c,p,64);
		len-=MD5_CBLOCK;
	}
	sc=(int)len;
	c->num=sc;
	if (sc)
	{
		sw=sc>>2;	/* words to copy */
#ifdef L_ENDIAN
		p[sw]=0;
		memcpy(p,data,sc);
#else
		sc&=0x03;
		for ( ; sw; sw--)
		{ c2l(data,l); *(p++)=l; }
		c2l_p(data,l,sc);
		*p=l;
#endif
	}
}



/*
	MD5缁撴潫
*/
#define MD5_LAST_BLOCK  56

#define l2c(l,c)	(*((c)++)=(unsigned char)(((l)     )&0xff), *((c)++)=(unsigned char)(((l)>> 8L)&0xff), *((c)++)=(unsigned char)(((l)>>16L)&0xff), *((c)++)=(unsigned char)(((l)>>24L)&0xff))


inline void MD5_Final(unsigned char *md, MD5_CTX *c)
{
	register int i,j;
	register ULONG l;
	register ULONG *p;
	static unsigned char end[4]={0x80,0x00,0x00,0x00};
	unsigned char *cp=end;
	
	/* c->num should definitly have room for at least one more byte. */
	p=c->data;
	j=c->num;
	i=j>>2;
	
	/* purify often complains about the following line as an
	* Uninitialized Memory Read.  While this can be true, the
	* following p_c2l macro will reset l when that case is true.
	* This is because j&0x03 contains the number of 'valid' bytes
	* already in p[i].  If and only if j&0x03 == 0, the UMR will
	* occur but this is also the only time p_c2l will do
	* l= *(cp++) instead of l|= *(cp++)
	* Many thanks to Alex Tang <altitude@cic.net> for pickup this
	* 'potential bug' */
#ifdef PURIFY
	if ((j&0x03) == 0) p[i]=0;
#endif
	l=p[i];
	p_c2l(cp,l,j&0x03);
	p[i]=l;
	i++;
	/* i is the next 'undefined word' */
	if (c->num >= MD5_LAST_BLOCK)
	{
		for (; i<MD5_LBLOCK; i++)
			p[i]=0;
		md5_block(c,p,64);
		i=0;
	}
	for (; i<(MD5_LBLOCK-2); i++)
		p[i]=0;
	p[MD5_LBLOCK-2]=c->Nl;
	p[MD5_LBLOCK-1]=c->Nh;
	md5_block(c,p,64);
	cp=md;
	l=c->A; l2c(l,cp);
	l=c->B; l2c(l,cp);
	l=c->C; l2c(l,cp);
	l=c->D; l2c(l,cp);
	
	/* clear stuff, md5_block may be leaving some stuff on the stack
	* but I'm not worried :-) */
	c->num=0;
	/*	memset((char *)&c,0,sizeof(c));*/
}



/*
	MD5 Hash function
*/

inline void Md5HashBufferA( BYTE *outBuffer, const BYTE *inBuffer, int length)
/*void md5HashBuffer( void *hashInfo, BYTE *outBuffer, BYTE *inBuffer,
					int length, const HASH_STATE hashState )
*/
{
	/*MD5_CTX *md5Info = ( MD5_CTX * ) hashInfo, md5InfoBuffer;*/
	MD5_CTX *md5Info, md5InfoBuffer;
	md5Info = &md5InfoBuffer;
	

	/* If the user has left it up to us to allocate the hash context buffer,
	   use the internal buffer */
	/*
	if( md5Info == NULL )
		md5Info = &md5InfoBuffer;
	*/

	MD5_Init( md5Info );
	MD5_Update( md5Info, inBuffer, length );
	MD5_Final( outBuffer, md5Info );


	/* Clean up */
	/*zeroise( &md5InfoBuffer, sizeof( MD5_CTX ) );*/
}



/*
**********************************************************************************************
	MD5鑷祴璇?
**********************************************************************************************
*/

#ifdef _DEBUG1

static struct {
	char *data;						/* Data to hash */
	int length;						/* Length of data */
	BYTE digest[ MD5_DIGEST_LENGTH ];	/* Digest of data */
	} digestValues[] = {
	{ "", 0,
	  { 0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04,
		0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E } },
	{ "a", 1,
	  { 0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8,
		0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61 } },
	{ "abc", 3,
	  { 0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0,
		0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72 } },
	{ "message digest", 14,
	  { 0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D,
		0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0 } },
	{ "abcdefghijklmnopqrstuvwxyz", 26,
	  { 0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00,
		0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B } },
	{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", 62,
	  { 0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5,
		0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F } },
	{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890", 80,
	  { 0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55,
		0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A } },
	{ NULL, 0, { 0 } }
	};


bool Md5Test( void )
{
	BYTE digest[ MD5_DIGEST_LENGTH ];
	int i;
	
	/* Test MD5 against the test vectors given in RFC 1320 */
	for( i = 0; digestValues[ i ].data != NULL; i++ )
	{
		/*void Md5HashBuffer( BYTE *outBuffer, BYTE *inBuffer, int length)*/
		/*md5HashBuffer( NULL, digest, ( BYTE * ) digestValues[ i ].data,
			digestValues[ i ].length, HASH_ALL );*/
		Md5HashBuffer( digest, ( BYTE * ) digestValues[ i ].data,
			digestValues[ i ].length);

		if( memcmp( digest, digestValues[ i ].digest, MD5_DIGEST_LENGTH ) )
			return( false );
	}
	
	return( true );
}

#endif /* #ifdef _DEBUG */


#define RL(x, y) (((x) << (y)) | ((x) >> (32 - (y)))) //x向左循环移y位

#define PP(x) (x<<24)|((x<<8)&0xff0000)|((x>>8)&0xff00)|(x>>24) //将x高低位互换,例如PP(aabbccdd)=ddccbbaa

#define FF(a, b, c, d, x, s, ac) a = b + (RL((a + F(b,c,d) + x + ac),s))
#define GG(a, b, c, d, x, s, ac) a = b + (RL((a + G(b,c,d) + x + ac),s))
#define HH(a, b, c, d, x, s, ac) a = b + (RL((a + H(b,c,d) + x + ac),s))
#define II(a, b, c, d, x, s, ac) a = b + (RL((a + I(b,c,d) + x + ac),s))

struct MD5MSG
{
	unsigned A, B, C, D, a, b, c, d, i, len, flen[2], x[16];   //i临时变量,len文件长,flen[2]为64位二进制表示的文件初始长度
	MD5MSG()
	{
		A = B = C = D = a = b = c = d = i = len = 0;
	}
};

void md5(MD5MSG &md5msg)
{
	md5msg.a = md5msg.A, md5msg.b = md5msg.B, md5msg.c = md5msg.C, md5msg.d = md5msg.D;
	/**//* Round 1 */
	FF(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[0], 7, 0xd76aa478); /**//* 1 */
	FF(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[1], 12, 0xe8c7b756); /**//* 2 */
	FF(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[2], 17, 0x242070db); /**//* 3 */
	FF(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[3], 22, 0xc1bdceee); /**//* 4 */
	FF(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[4], 7, 0xf57c0faf); /**//* 5 */
	FF(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[5], 12, 0x4787c62a); /**//* 6 */
	FF(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[6], 17, 0xa8304613); /**//* 7 */
	FF(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[7], 22, 0xfd469501); /**//* 8 */
	FF(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[8], 7, 0x698098d8); /**//* 9 */
	FF(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[9], 12, 0x8b44f7af); /**//* 10 */
	FF(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[10], 17, 0xffff5bb1); /**//* 11 */
	FF(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[11], 22, 0x895cd7be); /**//* 12 */
	FF(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[12], 7, 0x6b901122); /**//* 13 */
	FF(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[13], 12, 0xfd987193); /**//* 14 */
	FF(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[14], 17, 0xa679438e); /**//* 15 */
	FF(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[15], 22, 0x49b40821); /**//* 16 */

	/**//* Round 2 */
	GG(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[1], 5, 0xf61e2562); /**//* 17 */
	GG(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[6], 9, 0xc040b340); /**//* 18 */
	GG(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[11], 14, 0x265e5a51); /**//* 19 */
	GG(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[0], 20, 0xe9b6c7aa); /**//* 20 */
	GG(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[5], 5, 0xd62f105d); /**//* 21 */
	GG(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[10], 9, 0x02441453); /**//* 22 */
	GG(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[15], 14, 0xd8a1e681); /**//* 23 */
	GG(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[4], 20, 0xe7d3fbc8); /**//* 24 */
	GG(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[9], 5, 0x21e1cde6); /**//* 25 */
	GG(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[14], 9, 0xc33707d6); /**//* 26 */
	GG(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[3], 14, 0xf4d50d87); /**//* 27 */
	GG(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[8], 20, 0x455a14ed); /**//* 28 */
	GG(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[13], 5, 0xa9e3e905); /**//* 29 */
	GG(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[2], 9, 0xfcefa3f8); /**//* 30 */
	GG(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[7], 14, 0x676f02d9); /**//* 31 */
	GG(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[12], 20, 0x8d2a4c8a); /**//* 32 */

	/**//* Round 3 */
	HH(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[5], 4, 0xfffa3942); /**//* 33 */
	HH(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[8], 11, 0x8771f681); /**//* 34 */
	HH(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[11], 16, 0x6d9d6122); /**//* 35 */
	HH(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[14], 23, 0xfde5380c); /**//* 36 */
	HH(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[1], 4, 0xa4beea44); /**//* 37 */
	HH(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[4], 11, 0x4bdecfa9); /**//* 38 */
	HH(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[7], 16, 0xf6bb4b60); /**//* 39 */
	HH(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[10], 23, 0xbebfbc70); /**//* 40 */
	HH(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[13], 4, 0x289b7ec6); /**//* 41 */
	HH(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[0], 11, 0xeaa127fa); /**//* 42 */
	HH(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[3], 16, 0xd4ef3085); /**//* 43 */
	HH(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[6], 23, 0x04881d05); /**//* 44 */
	HH(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[9], 4, 0xd9d4d039); /**//* 45 */
	HH(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[12], 11, 0xe6db99e5); /**//* 46 */
	HH(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[15], 16, 0x1fa27cf8); /**//* 47 */
	HH(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[2], 23, 0xc4ac5665); /**//* 48 */

	/**//* Round 4 */
	II(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[0], 6, 0xf4292244); /**//* 49 */
	II(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[7], 10, 0x432aff97); /**//* 50 */
	II(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[14], 15, 0xab9423a7); /**//* 51 */
	II(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[5], 21, 0xfc93a039); /**//* 52 */
	II(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[12], 6, 0x655b59c3); /**//* 53 */
	II(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[3], 10, 0x8f0ccc92); /**//* 54 */
	II(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[10], 15, 0xffeff47d); /**//* 55 */
	II(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[1], 21, 0x85845dd1); /**//* 56 */
	II(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[8], 6, 0x6fa87e4f); /**//* 57 */
	II(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[15], 10, 0xfe2ce6e0); /**//* 58 */
	II(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[6], 15, 0xa3014314); /**//* 59 */
	II(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[13], 21, 0x4e0811a1); /**//* 60 */
	II(md5msg.a, md5msg.b, md5msg.c, md5msg.d, md5msg.x[4], 6, 0xf7537e82); /**//* 61 */
	II(md5msg.d, md5msg.a, md5msg.b, md5msg.c, md5msg.x[11], 10, 0xbd3af235); /**//* 62 */
	II(md5msg.c, md5msg.d, md5msg.a, md5msg.b, md5msg.x[2], 15, 0x2ad7d2bb); /**//* 63 */
	II(md5msg.b, md5msg.c, md5msg.d, md5msg.a, md5msg.x[9], 21, 0xeb86d391); /**//* 64 */

	md5msg.A += md5msg.a;
	md5msg.B += md5msg.b;
	md5msg.C += md5msg.c;
	md5msg.D += md5msg.d;
}