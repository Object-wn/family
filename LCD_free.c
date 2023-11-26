/***************
 * @File Name: free_1.c
 * @Author: wn
 * @Version: 1.0
 * @Email: wn418@outlook.com
 * @Creat Time: 2023.9.29 9:48
****************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "main.h"
#include <ft2build.h>
#include <wchar.h>
#include FT_FREETYPE_H

#define FB_DEV "/dev/fb0" //LCD设备

#define argb8888_to_rgb565(color) ({ \
                unsigned int temp = (color); \
                ((temp & 0xF80000UL) >> 8) | \
                ((temp & 0xFC00UL) >> 5) | \
                ((temp & 0xf8UL) >> 3); \
                })

static unsigned int width; //LCD宽
static unsigned int height;//LCD高
static unsigned short *screen_base = NULL; //LCD显存基址
static unsigned long screen_size;
static int fd = -1; 


static FT_Library library;
static FT_Face face;

static int fb_dev_init(void)
{
    struct fb_var_screeninfo fb_var;
    struct fb_fix_screeninfo fb_fix;

    fd = open(FB_DEV, O_RDWR);
    if (fd < 0)
    {
        perror("open error");
        return -1;
    }
    
    ioctl(fd, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fd, FBIOGET_FSCREENINFO, &fb_fix);
    
    screen_size = fb_var.yres * fb_fix.line_length;
    width = fb_var.xres;
    height = fb_var.yres;

    /* 映射LCD基地址 */
    screen_base = mmap(NULL, screen_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (screen_base == MAP_FAILED)
    {
        perror("mmap error");
        close(fd);
        return -1;
    }

    memset(screen_base, 0xFF, screen_size);
    return 0;
}

static int freetype_init(const char *font, int angle)
{
    FT_Error    error;//错误对象
    FT_Vector   pen; //xy坐标
    FT_Matrix   matrix; //2*2简单矩阵
    float       rad; //旋转角度

    /* freeType init */
     FT_Init_FreeType(&library);
    
    /* load face */
    error = FT_New_Face(library, font, 0, &face);
    if (error)
    {
        fprintf(stderr, "FT_New_Face error:%s\n", error);
        exit(EXIT_SUCCESS);
    }

    /* 原点坐标 */
    pen.x = 0 * 64;
    pen.y = 0 * 64;

    rad = (1.0 * angle / 180) * M_PI;

    #if 0       //非水平方向
    matrix.xx = (FT_Fixed)( cos(rad) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(rad) * 0x10000L);
    matrix.yx = (FT_Fixed)( sin(rad) * 0x10000L);
    matrix.yy = (FT_Fixed)( cos(rad) * 0x10000L);
    #endif
    #if 1
        matrix.xx = (FT_Fixed)(cos(rad) * 0x10000L);
        matrix.xy = (FT_Fixed)(sin(rad) * 0x10000L);
        matrix.yx = (FT_Fixed)(0 * 0x10000L);
        matrix.yy = (FT_Fixed)(1 * 0x10000L);
    #endif
    //setting
    FT_Set_Transform(face, &matrix, &pen);
    //字体大小
    FT_Set_Pixel_Sizes(face, 50, 0);

    return 0;
}

static void lcd_draw_character(int x, int y, const wchar_t *str, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color);
    FT_GlyphSlot slot = face->glyph; //插槽 字体的处理结果保存在这
    size_t len = wcslen(str);
    long int temp; //保存当前的显存基址
    int n; //字符遍历
    /*
    @parameter
        i: 每行的起始位置
        j: 列的起始位置
        p: 定位到当前行 之后再加上偏移量q就是查看当前像素点是否为1
        q: y一次的偏移量
    */
    int i, j, p, q;
    int max_x, max_y, start_x, start_y;

    //循环加载各个字符
    for (n = 0; n < len; n++)
    {
        if (FT_Load_Char(face, str[n], FT_LOAD_RENDER))
            continue;
        start_y = y - slot->bitmap_top;
        if (0 > start_y) //为负数
        {
            q = -start_y;
            temp = 0;
            j = 0;
        }
        else //正数 
        {
            q = 0;
            temp = width * start_y;
            j = start_y;
        }

        max_y = start_y + slot->bitmap.rows;
        if (max_y > (int)height)
        {
            max_y = height;
        }
        for (; j < max_y; j++, q++, temp += width)
        {
            start_x = x + slot->bitmap_left; //起点地址加上左边空余部分
            if (start_x < 0)
            {
                p = -start_x;
                i = 0;
            }
            else
            {
                p = 0;
                i = start_x;
            }
            max_x = start_x + slot->bitmap.width;
            if (max_x > (int)width)
            {
                max_x = width;
            }
            for (; i < max_x; i++, p++)
            {
                if (slot->bitmap.buffer[q * slot->bitmap.width + p])
                {
                    screen_base[temp + i] = rgb565_color;
                    screen_base[temp + i + 3] = rgb565_color;
                }
            }
        }
        //调整下一个字符原点
        x += slot->advance.x / 64;
        y += slot->advance.y / 64;
    }
}

int LCD_main(char *ch)
{
   wchar_t wch[8];
   int i = 0;
   for(i = 0; i < 8; i++)
    wch[i] = ch[i];
    /* LCD Init */
    if (fb_dev_init())
    {
        exit(EXIT_FAILURE);
    }
     /* freetype init */
    if (freetype_init("/home/root/zi/STSONG.TTF", 0))
    {
        exit(EXIT_FAILURE);
    }

    /* 显示中文 */
    int y = height * 0.15;
   
    //lcd_draw_character(50, y+100, L"实验楼B401", 0x9900FF);
    switch (ch[1])
    {
        case 'H':
             lcd_draw_character(50, 100, wch,  0x9900FF);
             break;
        case 'T':
             lcd_draw_character(50, y+100, wch,  0x9900FF);
             break;
    }

    /* exit */
//     FT_Done_Face(face);
//     FT_Done_FreeType(library);
//    munmap(screen_base, screen_size);
//    close(fd);
//    exit(EXIT_SUCCESS);
}
