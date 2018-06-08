//yuv420sp:nv21
//yyyyyyyyvuvu


#include <stdio.h>

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

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


void yuv420sp_to_rgb_fast_asm(const unsigned char* yuv420sp, int w, int h, unsigned char* rgb)
{
    const unsigned char* yptr = yuv420sp;
    const unsigned char* vuptr = yuv420sp + w * h;

    int8x8_t _v128 = vdup_n_s8(128);
    int8x8_t _v90 = vdup_n_s8(90);
    int8x8_t _v46 = vdup_n_s8(46);
    int8x8_t _v22 = vdup_n_s8(22);
    int8x8_t _v113 = vdup_n_s8(113);

    for (int y=0; y<h; y+=2)
    {
        const unsigned char* yptr0 = yptr;
        const unsigned char* yptr1 = yptr + w;
        unsigned char* rgb0 = rgb;
        unsigned char* rgb1 = rgb + w*3;

#if __ARM_NEON
        int nn = w >> 3;
        int remain = w - (nn << 3);
#else
        int remain = w;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
        for (; nn>0; nn--)
        {
            int16x8_t _yy0 = vreinterpretq_s16_u16(vshll_n_u8(vld1_u8(yptr0), 6));
            int16x8_t _yy1 = vreinterpretq_s16_u16(vshll_n_u8(vld1_u8(yptr1), 6));

            int8x8_t _vvuu = vsub_s8(vreinterpret_s8_u8(vld1_u8(vuptr)), _v128);
            int8x8x2_t _vvvvuuuu = vtrn_s8(_vvuu, _vvuu);
            int8x8_t _vv = _vvvvuuuu.val[0];
            int8x8_t _uu = _vvvvuuuu.val[1];

            int16x8_t _r0 = vmlal_s8(_yy0, _vv, _v90);
            int16x8_t _g0 = vmlsl_s8(_yy0, _vv, _v46);
            _g0 = vmlsl_s8(_g0, _uu, _v22);
            int16x8_t _b0 = vmlal_s8(_yy0, _uu, _v113);

            int16x8_t _r1 = vmlal_s8(_yy1, _vv, _v90);
            int16x8_t _g1 = vmlsl_s8(_yy1, _vv, _v46);
            _g1 = vmlsl_s8(_g1, _uu, _v22);
            int16x8_t _b1 = vmlal_s8(_yy1, _uu, _v113);

            uint8x8x3_t _rgb0;
            _rgb0.val[0] = vqshrun_n_s16(_r0, 6);
            _rgb0.val[1] = vqshrun_n_s16(_g0, 6);
            _rgb0.val[2] = vqshrun_n_s16(_b0, 6);

            uint8x8x3_t _rgb1;
            _rgb1.val[0] = vqshrun_n_s16(_r1, 6);
            _rgb1.val[1] = vqshrun_n_s16(_g1, 6);
            _rgb1.val[2] = vqshrun_n_s16(_b1, 6);

            vst3_u8(rgb0, _rgb0);
            vst3_u8(rgb1, _rgb1);

            yptr0 += 8;
            yptr1 += 8;
            vuptr += 8;
            rgb0 += 24;
            rgb1 += 24;
        }
#else
        if (nn > 0)
        {
        asm volatile(
            "pld        [%3, #128]          \n"
            "vld1.u8    {d2}, [%3]!         \n"
            "vsub.s8    d2, d2, %12         \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.u8    {d0}, [%1]!         \n"
            "pld        [%2, #128]          \n"
            "vld1.u8    {d1}, [%2]!         \n"
            "vshll.u8   q2, d0, #6          \n"
            "vorr       d3, d2, d2          \n"
            "vshll.u8   q3, d1, #6          \n"
            "vorr       q9, q2, q2          \n"
            "vtrn.s8    d2, d3              \n"
            "vorr       q11, q3, q3         \n"
            "vmlsl.s8   q9, d2, %14         \n"
            "vorr       q8, q2, q2          \n"
            "vmlsl.s8   q11, d2, %14        \n"
            "vorr       q10, q3, q3         \n"
            "vmlal.s8   q8, d2, %13         \n"
            "vmlal.s8   q2, d3, %16         \n"
            "vmlal.s8   q10, d2, %13        \n"
            "vmlsl.s8   q9, d3, %15         \n"
            "vmlal.s8   q3, d3, %16         \n"
            "vmlsl.s8   q11, d3, %15        \n"
            "vqshrun.s16 d24, q8, #6        \n"
            "vqshrun.s16 d26, q2, #6        \n"
            "vqshrun.s16 d4, q10, #6        \n"
            "vqshrun.s16 d25, q9, #6        \n"
            "vqshrun.s16 d6, q3, #6         \n"
            "vqshrun.s16 d5, q11, #6        \n"
            "pld        [%3, #128]          \n"
            "vld1.u8    {d2}, [%3]!         \n"
            "subs       %0, #1              \n"
            "vst3.u8    {d24-d26}, [%4]!    \n"
            "vsub.s8    d2, d2, %12         \n"
            "vst3.u8    {d4-d6}, [%5]!      \n"
            "bne        0b                  \n"
            "sub        %3, #8              \n"
            : "=r"(nn),     // %0
              "=r"(yptr0),  // %1
              "=r"(yptr1),  // %2
              "=r"(vuptr),  // %3
              "=r"(rgb0),   // %4
              "=r"(rgb1)    // %5
            : "0"(nn),
              "1"(yptr0),
              "2"(yptr1),
              "3"(vuptr),
              "4"(rgb0),
              "5"(rgb1),
              "w"(_v128),   // %12
              "w"(_v90),    // %13
              "w"(_v46),    // %14
              "w"(_v22),    // %15
              "w"(_v113)    // %16
            : "cc", "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "d26"
        );
        }
#endif // __aarch64__
#endif // __ARM_NEON

        for (; remain>0; remain--)
        {
            // R = 1.164 * yy + 1.596 * vv
            // G = 1.164 * yy - 0.813 * vv - 0.391 * uu
            // B = 1.164 * yy              + 2.018 * uu

            // R = Y + (1.370705 * (V-128))
            // G = Y - (0.698001 * (V-128)) - (0.337633 * (U-128))
            // B = Y + (1.732446 * (U-128))

            // R = ((Y << 6) + 87.72512 * (V-128)) >> 6
            // G = ((Y << 6) - 44.672064 * (V-128) - 21.608512 * (U-128)) >> 6
            // B = ((Y << 6) + 110.876544 * (U-128)) >> 6

            // R = ((Y << 6) + 90 * (V-128)) >> 6
            // G = ((Y << 6) - 46 * (V-128) - 22 * (U-128)) >> 6
            // B = ((Y << 6) + 113 * (U-128)) >> 6

            // R = (yy + 90 * vv) >> 6
            // G = (yy - 46 * vv - 22 * uu) >> 6
            // B = (yy + 113 * uu) >> 6

            int v = vuptr[0] - 128;
            int u = vuptr[1] - 128;

            int ruv = 90 * v;
            int guv = -46 * v + -22 * u;
            int buv = 113 * u;

            int y00 = yptr0[0] << 6;
            rgb0[0] = cv::saturate_cast<uchar>((y00 + ruv) >> 6);
            rgb0[1] = cv::saturate_cast<uchar>((y00 + guv) >> 6);
            rgb0[2] = cv::saturate_cast<uchar>((y00 + buv) >> 6);

            int y01 = yptr0[1] << 6;
            rgb0[3] = cv::saturate_cast<uchar>((y01 + ruv) >> 6);
            rgb0[4] = cv::saturate_cast<uchar>((y01 + guv) >> 6);
            rgb0[5] = cv::saturate_cast<uchar>((y01 + buv) >> 6);

            int y10 = yptr1[0] << 6;
            rgb1[0] = cv::saturate_cast<uchar>((y10 + ruv) >> 6);
            rgb1[1] = cv::saturate_cast<uchar>((y10 + guv) >> 6);
            rgb1[2] = cv::saturate_cast<uchar>((y10 + buv) >> 6);

            int y11 = yptr1[1] << 6;
            rgb1[3] = cv::saturate_cast<uchar>((y11 + ruv) >> 6);
            rgb1[4] = cv::saturate_cast<uchar>((y11 + guv) >> 6);
            rgb1[5] = cv::saturate_cast<uchar>((y11 + buv) >> 6);

            yptr0 += 2;
            yptr1 += 2;
            vuptr += 2;
            rgb0 += 6;
            rgb1 += 6;
        }

        yptr += 2*w;
        rgb += 2*3*w;
    }
}

int main(int argc, char** argv)
{
    const char* imagepath = argv[1];

    int w = 600;
    int h = 960;

    std::string nv21_buf = read_file(imagepath);
    cv::Mat nv21(h + h/2, w, CV_8UC1, (void*)nv21_buf.data());

    cv::Mat rgb;
    cv::cvtColor(nv21, rgb, cv::COLOR_YUV2RGB_NV21);

    cv::Mat rgb4(h, w, CV_8UC3);
    yuv420sp_to_rgb_fast_asm(nv21.data, w, h, rgb4.data);

    return 0;
}

