//yyyyuv

#include <stdio.h>

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::string read_file(const char* path)
{
    std::string buf;

    FILE* fp = fopen(path, "rb");
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    rewind(fp);
    buf.resize(len);
    fread((void*)buf.data(), 1, len, fp);
    fclose(fp);

    return buf;
}


void YUV4202RGB888(const unsigned char *p_y,
                   const unsigned char *p_u,
                   const unsigned char *p_v,
                   unsigned char *p_rgb,
                   int width,
                   int height)
{
  const int16_t coef[5] = {298, 409, -100, -208, 516};

  __asm__ volatile(

     "mov	r6, %5	\n\t" // outer count
      "vld1.16	{d0, d1}, [%4]	\n\t" // coef
      "mov     r4, #16 \n\t"   //16
      "vdup.8  d2, r4 \n\t"

      "mov     r4, #128 \n\t"  //128
      "vdup.8  d3, r4 \n\t"

      "mov		r7, %1		\n\t" // p_u
      "mov		r8, %2		\n\t" // p_v

      /* enter outer loop */
      "1:\n\t"

      "mov	r5, %6, lsr #4	\n\t" // inner count
      "mov		%1, r7		\n\t" 
      "mov		%2, r8		\n\t" 

      /* enter outer loop */
      "0:\n\t"

      /* load data */
      "vld1.8		{q2}, [%0]!		\n\t" 
      "vld1.8		{d16}, [%1]!	\n\t" 
      "vld1.8		{d14}, [%2]!	\n\t" 

      /* calculate R G B */
      "vsubl.u8   q3,d4,d2      \n\t"  
      "vsubl.u8   q5,d5,d2      \n\t"  
      "vsubl.u8   q7,d14,d3     \n\t"  
      "vsubl.u8   q8,d16,d3      \n\t" 

      "vmull.s16	q4, d7, d0[0]	\n\t" 
      "vmull.s16	q3, d6, d0[0]	\n\t"
      "vmull.s16	q6, d11, d0[0]	\n\t" 
      "vmull.s16	q5, d10, d0[0]	\n\t"


      "vmov		q9, q7			\n\t"
      "vmov		q10, q8			\n\t"
      "vzip.16	q7, q9			\n\t" 
      "vzip.16	q8, q10			\n\t" 

      "vmov		q11, q3			\n\t"
      "vmov		q12, q4			\n\t"
      "vmov		q13, q3			\n\t"
      "vmov		q14, q4			\n\t"

      "vmlal.s16	q11, d14, d0[1]	\n\t" /* R1: c298+(cV-128)*409 */
      "vmlal.s16	q12, d15, d0[1]	\n\t"
      "vmlal.s16	q13, d16, d0[2]	\n\t" /* G1: c298-100*(cU-128) */
      "vmlal.s16	q14, d17, d0[2]	\n\t"
      "vmlal.s16	q13, d14, d0[3]	\n\t" /* G1 st2: c298-100*(cU-128)-208*(cV-128) */
      "vmlal.s16	q14, d15, d0[3]	\n\t"
      "vmlal.s16	q3, d16, d1[0]	\n\t" /* B1: c298+516*(cU-128) */
      "vmlal.s16	q4, d17, d1[0]	\n\t"
      "vqrshrun.s32	d22, q11, #8 	\n\t" /* get R1 */
      "vqrshrun.s32	d23, q12, #8 	\n\t"
      "vqrshrun.s32	d26, q13, #8 	\n\t" /* get G1 */
      "vqrshrun.s32	d27, q14, #8 	\n\t"
      "vqrshrun.s32	d6, q3, #8 		\n\t" /* get B1 */
      "vqrshrun.s32	d7, q4, #8 		\n\t"
      "vqmovun.s16	d29, q13		\n\t" // G1
      "vqmovun.s16	d28, q11		\n\t" // R1
      "vqmovun.s16	d30, q3			\n\t" // B1
      /* store */
      "vst3.8		{d28-d30}, [%3]!	\n\t"

      "vmov		q11, q5			\n\t"
      "vmov		q12, q6			\n\t"
      "vmov		q13, q5			\n\t"
      "vmov		q14, q6			\n\t"

      "vmlal.s16	q11, d18, d0[1]	\n\t" /* R2: c298+(cV-128)*409 */
      "vmlal.s16	q12, d19, d0[1]	\n\t"
      "vmlal.s16	q13, d20, d0[2]	\n\t" /* G2: c298-100*(cU-128) */
      "vmlal.s16	q14, d21, d0[2]	\n\t"
      "vmlal.s16	q13, d18, d0[3]	\n\t" /* G2 st2: c298-100*(cU-128)-208*(cV-128) */
      "vmlal.s16	q14, d19, d0[3]	\n\t"
      "vmlal.s16	q5, d20, d1[0]	\n\t" /* B2: c298+516*(cU-128) */
      "vmlal.s16	q6, d21, d1[0]	\n\t"
      "vqrshrun.s32	d22, q11, #8 	\n\t" /* get R2 */
      "vqrshrun.s32	d23, q12, #8 	\n\t"
      "vqrshrun.s32	d26, q13, #8 	\n\t" /* get G2 */
      "vqrshrun.s32	d27, q14, #8 	\n\t"
      "vqrshrun.s32	d10, q5, #8 	\n\t" /* get B2 */
      "vqrshrun.s32	d11, q6, #8 	\n\t"

      "vqmovun.s16	d29, q13		\n\t" // G2
      "vqmovun.s16	d28, q11		\n\t" // R2
      "vqmovun.s16	d30, q5			\n\t" // B2
      /* store */
      "vst3.8		{d28-d30}, [%3]!	\n\t"

      /* judge inner loop */
      "subs	r5, r5, #1	\n\t"
      "bne	0b			\n\t" // not equal to 0

      /* judge outer loop */
      "tst	r6, #1			\n\t" 
      "sub	r6, r6, #1		\n\t" 
      "beq	1b				\n\t"
      "add	r7, r7, %6, lsr #1		\n\t"
      "add	r8, r8, %6, lsr #1		\n\t"
      "cmp	r6, #0			\n\t"
      "bne	1b				\n\t"

  : "+r"(p_y),             // %0
    "+r"(p_u),             // %1
    "+r"(p_v),             // %2
    "+r"(p_rgb)            // %3   
  : "r"(coef),             // %4
    "r"(height),          // %5
    "r"(width)            // %6  
  : "cc", "memory",
      "r4","r5", "r6","r7","r8",
      "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8",
      "q9", "q10", "q11", "q12", "q13", "q14", "q15"  
  );
}

int main(int argc, char** argv)
{
    const char* imagepath = argv[1];

    int w = 1280;
    int h = 720;

    std::string nv21_buf = read_file(imagepath);
    cv::Mat nv21(h + h/2, w, CV_8UC1, (void*)nv21_buf.data());

    //cv::Mat rgb;
    //cv::cvtColor(nv21, rgb, cv::COLOR_YUV2RGB_NV21);

    cv::Mat rgb4(h, w, CV_8UC3);
    unsigned char* yuv_ptr = nv21.data;
    unsigned char* y_ptr = yuv_ptr;
    
    unsigned char* uvptr = yuv + w * h;

    YUV4202RGB888(yptr, uptr, vptr, rgb4.data, w, h);

    return 0;
}

