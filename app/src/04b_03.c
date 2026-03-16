/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --stride 1 --align 1 --font 04B_03__.TTF --symbols !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÄÖÜäöüß€°± --font fontawesome-webfont.ttf --range 61931,61553,62016,62018 --format lvgl -o 04b_03.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef FONT04B03
#define FONT04B03 1
#endif

#if FONT04B03

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0021 "!" */
    0xff, 0xc3, 0xc0,

    /* U+0022 "\"" */
    0xde, 0xf6,

    /* U+0023 "#" */
    0x33, 0x19, 0xbf, 0xff, 0xf3, 0x37, 0xff, 0xfe,
    0x66, 0x33, 0x0,

    /* U+0024 "$" */
    0x8, 0x10, 0xf9, 0xff, 0x1, 0xc3, 0xfc, 0xf8,
    0x10, 0x20,

    /* U+0025 "%" */
    0xc6, 0x63, 0x1, 0x80, 0x0, 0x80, 0x40, 0xc0,
    0x63, 0x31, 0x80,

    /* U+0026 "&" */
    0x38, 0x1c, 0x30, 0x18, 0x3, 0x9f, 0xff, 0x18,
    0x73, 0x39, 0x80,

    /* U+0027 "'" */
    0xfc,

    /* U+0028 "(" */
    0x33, 0xcc, 0xcc, 0xc3, 0x30,

    /* U+0029 ")" */
    0xcc, 0x33, 0x33, 0x3c, 0xc0,

    /* U+002A "*" */
    0xce, 0x4c, 0x6c, 0xe4,

    /* U+002B "+" */
    0x37, 0xfe, 0x63, 0x0,

    /* U+002C "," */
    0x33, 0xcc,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x1, 0x80, 0xc1, 0x80, 0xc0, 0x81, 0x80, 0xc1,
    0x80, 0xc0, 0x0,

    /* U+0030 "0" */
    0x38, 0x73, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,

    /* U+0031 "1" */
    0xff, 0x33, 0x33, 0x33, 0x30,

    /* U+0032 "2" */
    0xf9, 0xf0, 0x18, 0x33, 0x9f, 0x30, 0x7f, 0xfe,

    /* U+0033 "3" */
    0xf9, 0xf0, 0x18, 0x37, 0x80, 0xc1, 0xfc, 0xf8,

    /* U+0034 "4" */
    0x18, 0x30, 0xe1, 0xcd, 0x9f, 0xff, 0x8c, 0x18,

    /* U+0035 "5" */
    0xff, 0xff, 0x6, 0xf, 0x80, 0xc1, 0xfc, 0xf8,

    /* U+0036 "6" */
    0x38, 0x73, 0x6, 0xf, 0x9f, 0xf1, 0x9c, 0x38,

    /* U+0037 "7" */
    0xff, 0xfc, 0x8, 0x10, 0xc1, 0x8c, 0x18, 0x30,

    /* U+0038 "8" */
    0x38, 0x73, 0x1e, 0x33, 0x9f, 0xf1, 0x9c, 0x38,

    /* U+0039 "9" */
    0x38, 0x73, 0x1e, 0x33, 0xe7, 0xc1, 0x9c, 0x38,

    /* U+003A ":" */
    0xf0, 0xf0,

    /* U+003B ";" */
    0xf0, 0xfc,

    /* U+003C "<" */
    0x8, 0x4c, 0x6c, 0x18, 0xc1, 0x8,

    /* U+003D "=" */
    0xff, 0xc0, 0xf, 0xfc,

    /* U+003E ">" */
    0xc6, 0xc, 0x60, 0x98, 0xd8, 0xc0,

    /* U+003F "?" */
    0xfd, 0xf8, 0x8, 0x13, 0xc7, 0x80, 0x18, 0x30,

    /* U+0040 "@" */
    0x3e, 0x1f, 0x30, 0x78, 0x3d, 0xfe, 0xff, 0x66,
    0x7c, 0x3e, 0x0,

    /* U+0041 "A" */
    0x38, 0x73, 0x1e, 0x3c, 0x7f, 0xff, 0xe3, 0xc6,

    /* U+0042 "B" */
    0xf9, 0xf3, 0x1e, 0x3f, 0x9f, 0xf1, 0xfc, 0xf8,

    /* U+0043 "C" */
    0x39, 0xf1, 0x8c, 0x63, 0x7, 0x38,

    /* U+0044 "D" */
    0xf9, 0xf3, 0x1e, 0x3c, 0x78, 0xf1, 0xfc, 0xf8,

    /* U+0045 "E" */
    0xff, 0xfc, 0x30, 0xff, 0xfc, 0x3f, 0xfc,

    /* U+0046 "F" */
    0xff, 0xfc, 0x30, 0xff, 0xfc, 0x30, 0xc0,

    /* U+0047 "G" */
    0x3e, 0x7f, 0x6, 0xd, 0xfb, 0xf1, 0x9f, 0x3e,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3f, 0xff, 0xf1, 0xe3, 0xc6,

    /* U+0049 "I" */
    0xff, 0xcc, 0x63, 0x18, 0xdf, 0xf8,

    /* U+004A "J" */
    0x1e, 0x3c, 0x18, 0x30, 0x78, 0xf1, 0x9c, 0x38,

    /* U+004B "K" */
    0xc7, 0x8f, 0x66, 0xcf, 0x1b, 0x36, 0x63, 0xc6,

    /* U+004C "L" */
    0xc3, 0xc, 0x30, 0xc3, 0xc, 0x3f, 0xfc,

    /* U+004D "M" */
    0xc1, 0xe0, 0xfd, 0xfe, 0xfd, 0x9e, 0xcf, 0x7,
    0x83, 0xc1, 0x80,

    /* U+004E "N" */
    0xc7, 0x8f, 0xdf, 0xbd, 0xfb, 0xf1, 0xe3, 0xc6,

    /* U+004F "O" */
    0x38, 0x73, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,

    /* U+0050 "P" */
    0xf9, 0xf3, 0x1e, 0x3c, 0x7f, 0x3e, 0x60, 0xc0,

    /* U+0051 "Q" */
    0x38, 0x73, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,
    0xc, 0x18,

    /* U+0052 "R" */
    0xf9, 0xf3, 0x1e, 0x3c, 0x7f, 0x3e, 0x63, 0xc6,

    /* U+0053 "S" */
    0x3e, 0x7f, 0x6, 0x3, 0x80, 0xc1, 0xfc, 0xf8,

    /* U+0054 "T" */
    0xff, 0xcc, 0x63, 0x18, 0xc6, 0x30,

    /* U+0055 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,

    /* U+0056 "V" */
    0xc7, 0x8f, 0x1e, 0xc, 0x99, 0x32, 0x18, 0x30,

    /* U+0057 "W" */
    0xc1, 0xe0, 0xf2, 0x79, 0x3c, 0x9e, 0x4f, 0x26,
    0x6c, 0x36, 0x0,

    /* U+0058 "X" */
    0xc7, 0x8f, 0x18, 0x3, 0x87, 0x31, 0xe3, 0xc6,

    /* U+0059 "Y" */
    0xc7, 0x8f, 0x18, 0x33, 0xe7, 0xc1, 0x9c, 0x38,

    /* U+005A "Z" */
    0xff, 0xc2, 0x13, 0x7b, 0x1f, 0xf8,

    /* U+005B "[" */
    0xff, 0xcc, 0xcc, 0xcf, 0xf0,

    /* U+005C "\\" */
    0xc0, 0x60, 0xc, 0x6, 0x0, 0x80, 0x30, 0x18,
    0x3, 0x1, 0x80,

    /* U+005D "]" */
    0xff, 0x33, 0x33, 0x3f, 0xf0,

    /* U+005E "^" */
    0x31, 0xb3, 0x90,

    /* U+005F "_" */
    0xff, 0xfc,

    /* U+0060 "`" */
    0xcc, 0x33,

    /* U+0061 "a" */
    0x3e, 0x7f, 0x1e, 0x3c, 0x67, 0xcf, 0x80,

    /* U+0062 "b" */
    0xc1, 0x83, 0xe7, 0xcc, 0x78, 0xf1, 0xfc, 0xf8,

    /* U+0063 "c" */
    0x39, 0xf1, 0x8c, 0x1c, 0xe0,

    /* U+0064 "d" */
    0x6, 0xc, 0xf9, 0xfc, 0x78, 0xf1, 0x9f, 0x3e,

    /* U+0065 "e" */
    0x38, 0x73, 0x7e, 0xff, 0x7, 0xe, 0x0,

    /* U+0066 "f" */
    0x8, 0x4c, 0x6f, 0xfc, 0xc6, 0x30,

    /* U+0067 "g" */
    0x3f, 0xff, 0x1e, 0x34, 0x67, 0xcf, 0x83, 0x38,
    0x70,

    /* U+0068 "h" */
    0xc1, 0x83, 0xe7, 0xcc, 0x78, 0xf1, 0xe3, 0xc6,

    /* U+0069 "i" */
    0xf0, 0xff, 0xc0,

    /* U+006A "j" */
    0x33, 0x3, 0x33, 0x33, 0x33, 0xcc,

    /* U+006B "k" */
    0xc1, 0x83, 0x1e, 0x3d, 0x9f, 0x3e, 0x63, 0xc6,

    /* U+006C "l" */
    0xff, 0xff, 0xc0,

    /* U+006D "m" */
    0xfe, 0x7f, 0x36, 0x7b, 0x3d, 0x9e, 0xcf, 0x66,

    /* U+006E "n" */
    0xf9, 0xf3, 0x1e, 0x3c, 0x78, 0xf1, 0x80,

    /* U+006F "o" */
    0x38, 0x73, 0x1e, 0x3c, 0x67, 0xe, 0x0,

    /* U+0070 "p" */
    0xf9, 0xff, 0x1e, 0x3c, 0x9f, 0x3e, 0x60, 0xc1,
    0x80,

    /* U+0071 "q" */
    0x3f, 0xff, 0x1e, 0x34, 0x67, 0xcf, 0x83, 0x6,
    0xc,

    /* U+0072 "r" */
    0xde, 0xfd, 0xec, 0x63, 0x0,

    /* U+0073 "s" */
    0x3e, 0x7f, 0xc0, 0x70, 0xff, 0x3e, 0x0,

    /* U+0074 "t" */
    0x31, 0xbf, 0xf3, 0x18, 0xc1, 0x8,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x67, 0xcf, 0x80,

    /* U+0076 "v" */
    0xc7, 0x8f, 0x1e, 0x4c, 0x86, 0xc, 0x0,

    /* U+0077 "w" */
    0xc9, 0xe4, 0xf2, 0x60, 0x3, 0x61, 0xb0, 0xd8,

    /* U+0078 "x" */
    0xce, 0x4c, 0x63, 0x67, 0x20,

    /* U+0079 "y" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x67, 0xcf, 0x83, 0x3e,
    0x70,

    /* U+007A "z" */
    0xff, 0xfc, 0x21, 0xc3, 0x1f, 0xff, 0x80,

    /* U+007B "{" */
    0x3c, 0xf3, 0xc, 0xc3, 0xc3, 0xf, 0x3c,

    /* U+007C "|" */
    0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xf3, 0xc3, 0xc, 0xc, 0xf3, 0x3c, 0xf0,

    /* U+007E "~" */
    0x36, 0x6f, 0x26, 0x40,

    /* U+F071 "" */
    0x3, 0x0, 0xc, 0x0, 0x78, 0x3, 0xe0, 0xc,
    0xc0, 0x73, 0x81, 0xce, 0xf, 0x3c, 0x3f, 0xf1,
    0xf3, 0xe7, 0xcf, 0xbf, 0xff, 0xff, 0xfc,

    /* U+F1EB "" */
    0xf, 0xe0, 0x7f, 0xf1, 0xe0, 0xf7, 0x3e, 0x71,
    0xff, 0x7, 0x7, 0x5, 0xf4, 0x7, 0xf0, 0x4,
    0x40, 0x7, 0x0, 0x4, 0x0,

    /* U+F240 "" */
    0xff, 0xff, 0xa0, 0x0, 0x2b, 0xff, 0xea, 0xff,
    0xfb, 0xbf, 0xfe, 0x6f, 0xff, 0x9b, 0xff, 0xee,
    0xff, 0xfa, 0x80, 0x0, 0xbf, 0xff, 0xe0,

    /* U+F242 "" */
    0xff, 0xff, 0xa0, 0x0, 0x2b, 0xf8, 0xa, 0xfe,
    0x3, 0xbf, 0x80, 0x6f, 0xe0, 0x1b, 0xf8, 0xe,
    0xfe, 0x2, 0x80, 0x0, 0xbf, 0xff, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 112, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 5, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 140, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 26, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 56, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 49, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 112, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 63, .adv_w = 112, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 67, .adv_w = 84, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 69, .adv_w = 112, .box_w = 5, .box_h = 2, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 71, .adv_w = 56, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 83, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 136, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 56, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 162, .adv_w = 56, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 112, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 174, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 221, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 273, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 140, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 333, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 371, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 390, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 409, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 425, .adv_w = 112, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 428, .adv_w = 140, .box_w = 7, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 84, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 432, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 439, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 112, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 460, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 140, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 482, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 493, .adv_w = 84, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 499, .adv_w = 140, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 168, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 532, .adv_w = 140, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 541, .adv_w = 140, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 550, .adv_w = 112, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 555, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 562, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 568, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 582, .adv_w = 168, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 112, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 140, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 604, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 140, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 632, .adv_w = 224, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 655, .adv_w = 256, .box_w = 15, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 676, .adv_w = 288, .box_w = 18, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 699, .adv_w = 288, .box_w = 18, .box_h = 10, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x17a, 0x1cf, 0x1d1
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 33, .range_length = 94, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 61553, .range_length = 466, .glyph_id_start = 95,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};

extern const lv_font_t lv_font_montserrat_14;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_font04b03 = {
#else
lv_font_t lv_font_font04b03 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 0,
#endif
    .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_14,
#endif
    .user_data = NULL,
};



#endif /*#if FONT04B03*/
