/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --stride 1 --align 1 --font spleen-8x16.woff --symbols !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ÄÖÜäöüß€°± --format lvgl -o spleen.c
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



#ifndef SPLEEN
#define SPLEEN 1
#endif

#if SPLEEN

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0021 "!" */
    0xff, 0xf0, 0xc0,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xb0,

    /* U+0023 "#" */
    0x79, 0xef, 0xde, 0x79, 0xef, 0xde, 0x78,

    /* U+0024 "$" */
    0x7d, 0x4d, 0x34, 0x7c, 0x71, 0xc7, 0xf8, 0x40,

    /* U+0025 "%" */
    0x8, 0xb4, 0x23, 0x19, 0x8d, 0xc0,

    /* U+0026 "&" */
    0x38, 0x51, 0xb3, 0x67, 0x9b, 0x72, 0x66, 0x7a,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x1c, 0xc6, 0x10, 0xc3, 0xc, 0x30, 0x60, 0xc1,
    0xc0,

    /* U+0029 ")" */
    0xe0, 0xc1, 0x82, 0xc, 0x30, 0xc3, 0x18, 0xce,
    0x0,

    /* U+002A "*" */
    0x6c, 0x73, 0xf8, 0x83, 0x8d, 0x80,

    /* U+002B "+" */
    0x33, 0xf3, 0xc,

    /* U+002C "," */
    0x78,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x7, 0x8, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60,
    0xc0,

    /* U+0030 "0" */
    0x7b, 0x3c, 0xf7, 0xef, 0xbc, 0xf3, 0x78,

    /* U+0031 "1" */
    0x30, 0xcf, 0xc, 0x30, 0xc3, 0xc, 0xfc,

    /* U+0032 "2" */
    0x7c, 0x8b, 0x18, 0x30, 0xc2, 0x18, 0x63, 0xfe,

    /* U+0033 "3" */
    0xfa, 0x28, 0xc3, 0x7c, 0x30, 0xe3, 0xf8,

    /* U+0034 "4" */
    0xc3, 0xd, 0xb6, 0xdb, 0xf1, 0x86, 0x18,

    /* U+0035 "5" */
    0xff, 0x3c, 0x30, 0xfc, 0x30, 0xf3, 0x78,

    /* U+0036 "6" */
    0x7b, 0x3c, 0x30, 0xff, 0x3c, 0xf3, 0x78,

    /* U+0037 "7" */
    0xff, 0x30, 0xc3, 0x10, 0xc3, 0xc, 0x30,

    /* U+0038 "8" */
    0x79, 0x2c, 0xf3, 0xff, 0x3c, 0xf3, 0x78,

    /* U+0039 "9" */
    0x7b, 0x3c, 0xf3, 0xcd, 0xf0, 0xf3, 0x78,

    /* U+003A ":" */
    0xc0, 0x30,

    /* U+003B ";" */
    0x60, 0x0, 0xf0,

    /* U+003C "<" */
    0xc, 0x23, 0x10, 0xc1, 0x6, 0x6, 0xc,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0xc1, 0x3, 0x2, 0xc, 0x21, 0x98, 0xc0,

    /* U+003F "?" */
    0x79, 0x2c, 0xc2, 0x10, 0xc0, 0x0, 0x30,

    /* U+0040 "@" */
    0x76, 0x7f, 0xff, 0xff, 0xe,

    /* U+0041 "A" */
    0x79, 0x2c, 0xf3, 0xff, 0x3c, 0xf3, 0xcc,

    /* U+0042 "B" */
    0xfb, 0x2c, 0xf3, 0xff, 0x3c, 0xf3, 0xf8,

    /* U+0043 "C" */
    0x7f, 0xc, 0x30, 0xc3, 0xc, 0x30, 0x7c,

    /* U+0044 "D" */
    0xfb, 0x3c, 0xf3, 0xcf, 0x3c, 0xf3, 0xf8,

    /* U+0045 "E" */
    0x7f, 0xc, 0x30, 0xfb, 0xc, 0x30, 0x7c,

    /* U+0046 "F" */
    0x7d, 0xc, 0x30, 0xfb, 0xc, 0x30, 0xc0,

    /* U+0047 "G" */
    0x7f, 0xc, 0x30, 0xdf, 0x3c, 0xf3, 0x7c,

    /* U+0048 "H" */
    0xcf, 0x3c, 0xf3, 0xff, 0x3c, 0xf3, 0xcc,

    /* U+0049 "I" */
    0xfc, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0xfc,

    /* U+004A "J" */
    0x7e, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0xf0,

    /* U+004B "K" */
    0xcf, 0x3c, 0xb4, 0xfb, 0x3c, 0xf3, 0xcc,

    /* U+004C "L" */
    0xc3, 0xc, 0x30, 0xc3, 0xc, 0x10, 0x7c,

    /* U+004D "M" */
    0xcf, 0x3f, 0xf3, 0xcf, 0x3c, 0xf3, 0xcc,

    /* U+004E "N" */
    0xcf, 0x3e, 0xfb, 0xdf, 0x3d, 0xf3, 0xcc,

    /* U+004F "O" */
    0x7b, 0x3c, 0xf3, 0xcf, 0x3c, 0xf3, 0x78,

    /* U+0050 "P" */
    0xfb, 0x2c, 0xf3, 0xfb, 0xc, 0x30, 0xc0,

    /* U+0051 "Q" */
    0x7b, 0x3c, 0xf3, 0xcf, 0x3d, 0xf7, 0x78, 0x40,
    0x80,

    /* U+0052 "R" */
    0xfb, 0x2c, 0xf3, 0xfb, 0x3c, 0xf3, 0xcc,

    /* U+0053 "S" */
    0x7d, 0xc, 0x30, 0x7c, 0x30, 0xc3, 0xf8,

    /* U+0054 "T" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18,

    /* U+0055 "U" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3c, 0xd3, 0x7c,

    /* U+0056 "V" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x34, 0x8c, 0x20,

    /* U+0057 "W" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3e, 0xff, 0xcc,

    /* U+0058 "X" */
    0xcf, 0x34, 0x8c, 0x7b, 0x3c, 0xf3, 0xcc,

    /* U+0059 "Y" */
    0xcf, 0x3c, 0xf3, 0x7c, 0x30, 0xc2, 0xf8,

    /* U+005A "Z" */
    0xfc, 0x30, 0xc6, 0x21, 0x8c, 0x30, 0xfc,

    /* U+005B "[" */
    0xfe, 0x31, 0x8c, 0x63, 0x18, 0xc7, 0xc0,

    /* U+005C "\\" */
    0xc0, 0x60, 0x30, 0x30, 0x18, 0x18, 0x8, 0x8,
    0x7,

    /* U+005D "]" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x1f, 0xc0,

    /* U+005E "^" */
    0x11, 0xec, 0xc0,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0xc9, 0x80,

    /* U+0061 "a" */
    0x78, 0x37, 0xf3, 0xcd, 0xf0,

    /* U+0062 "b" */
    0xc3, 0xc, 0x3e, 0xcf, 0x3c, 0xf3, 0xf8,

    /* U+0063 "c" */
    0x7f, 0xc, 0x30, 0xc1, 0xf0,

    /* U+0064 "d" */
    0xc, 0x30, 0xdf, 0xcf, 0x3c, 0xf3, 0x7c,

    /* U+0065 "e" */
    0x7f, 0x3f, 0xf0, 0xc1, 0xf0,

    /* U+0066 "f" */
    0x3c, 0x86, 0x18, 0xf9, 0x86, 0x18, 0x60,

    /* U+0067 "g" */
    0x7f, 0x3c, 0xf3, 0xcd, 0xe0, 0x83, 0xf8,

    /* U+0068 "h" */
    0xc3, 0xc, 0x3e, 0xcf, 0x3c, 0xf3, 0xcc,

    /* U+0069 "i" */
    0x60, 0xe, 0x66, 0x66, 0x70,

    /* U+006A "j" */
    0x30, 0x3, 0x33, 0x33, 0x33, 0x2e,

    /* U+006B "k" */
    0xc3, 0xc, 0x36, 0xd3, 0xcd, 0x36, 0xcc,

    /* U+006C "l" */
    0xcc, 0xcc, 0xcc, 0xc4, 0x70,

    /* U+006D "m" */
    0xeb, 0x7d, 0xf7, 0xdf, 0x30,

    /* U+006E "n" */
    0xfb, 0x3c, 0xf3, 0xcf, 0x30,

    /* U+006F "o" */
    0x7b, 0x3c, 0xf3, 0xcd, 0xe0,

    /* U+0070 "p" */
    0xfb, 0x3c, 0xf3, 0xcf, 0xec, 0x30, 0xc0,

    /* U+0071 "q" */
    0x7f, 0x3c, 0xf3, 0xcd, 0xf0, 0xc3, 0xc,

    /* U+0072 "r" */
    0x7f, 0x3c, 0x30, 0xc3, 0x0,

    /* U+0073 "s" */
    0x7f, 0x7, 0x82, 0xf, 0xe0,

    /* U+0074 "t" */
    0x61, 0x86, 0x3e, 0x61, 0x86, 0x8, 0x3c,

    /* U+0075 "u" */
    0xcf, 0x3c, 0xf3, 0xcd, 0xf0,

    /* U+0076 "v" */
    0xcf, 0x3c, 0xf3, 0x78, 0x80,

    /* U+0077 "w" */
    0xcf, 0x7d, 0xf7, 0xdd, 0xb0,

    /* U+0078 "x" */
    0xcd, 0xe3, 0xc, 0x7b, 0x30,

    /* U+0079 "y" */
    0xcf, 0x3c, 0xf3, 0xcd, 0xf0, 0xc2, 0xf8,

    /* U+007A "z" */
    0xfc, 0x31, 0x88, 0x63, 0xf0,

    /* U+007B "{" */
    0x3c, 0xc3, 0xc, 0x33, 0x3, 0xc, 0x30, 0xc3,
    0xc0,

    /* U+007C "|" */
    0xff, 0xff, 0xf0,

    /* U+007D "}" */
    0xe1, 0x8c, 0x63, 0xc, 0xc6, 0x31, 0xb8,

    /* U+007E "~" */
    0x66, 0x60,

    /* U+00B0 "°" */
    0x66, 0xf6,

    /* U+00B1 "±" */
    0x30, 0xcf, 0xcc, 0x3, 0xf0,

    /* U+00C4 "Ä" */
    0x78, 0x7, 0xb3, 0xcf, 0x3f, 0xf3, 0xcf, 0x30,

    /* U+00D6 "Ö" */
    0x78, 0x7, 0xb3, 0xcf, 0x3c, 0xf3, 0xcd, 0xe0,

    /* U+00DC "Ü" */
    0x78, 0xc, 0xf3, 0xcf, 0x3c, 0xf3, 0xcd, 0xf0,

    /* U+00DF "ß" */
    0x71, 0x4d, 0xb6, 0xf3, 0x6c, 0xfb, 0xf8,

    /* U+00E4 "ä" */
    0x78, 0x0, 0x1e, 0xd, 0xfc, 0xf3, 0x7c,

    /* U+00F6 "ö" */
    0x78, 0x0, 0x1e, 0xcf, 0x3c, 0xf3, 0x78,

    /* U+00FC "ü" */
    0x78, 0x0, 0x33, 0xcf, 0x3c, 0xf3, 0x7c,

    /* U+20AC "€" */
    0x18, 0xf6, 0x3c, 0xf1, 0x83, 0xc6
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 112, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 112, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 6, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 112, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 21, .adv_w = 112, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 27, .adv_w = 112, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 112, .box_w = 2, .box_h = 4, .ofs_x = 3, .ofs_y = 6},
    {.bitmap_index = 36, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 45, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 54, .adv_w = 112, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 60, .adv_w = 112, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 63, .adv_w = 112, .box_w = 3, .box_h = 2, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 64, .adv_w = 112, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 65, .adv_w = 112, .box_w = 2, .box_h = 1, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 112, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 75, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 112, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 112, .box_w = 2, .box_h = 6, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 112, .box_w = 3, .box_h = 7, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 151, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 112, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 161, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 112, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 208, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 112, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 279, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 302, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 112, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 332, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 339, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 346, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 112, .box_w = 5, .box_h = 10, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 374, .adv_w = 112, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 383, .adv_w = 112, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 390, .adv_w = 112, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 393, .adv_w = 112, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 394, .adv_w = 112, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 396, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 413, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 425, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 432, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 439, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 446, .adv_w = 112, .box_w = 4, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 112, .box_w = 4, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 457, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 464, .adv_w = 112, .box_w = 4, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 474, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 484, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 491, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 498, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 503, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 508, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 515, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 525, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 530, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 535, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 542, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 556, .adv_w = 112, .box_w = 2, .box_h = 10, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 559, .adv_w = 112, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 566, .adv_w = 112, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 568, .adv_w = 112, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 570, .adv_w = 112, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 112, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 583, .adv_w = 112, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 591, .adv_w = 112, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 599, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 606, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 620, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 627, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x1, 0x14, 0x26, 0x2c, 0x2f, 0x34, 0x46,
    0x4c, 0x1ffc
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 33, .range_length = 94, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 8189, .glyph_id_start = 95,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 10, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
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
const lv_font_t lv_font_spleen = {
#else
lv_font_t lv_font_spleen = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 13,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_14,
#endif
    .user_data = NULL,
};



#endif /*#if SPLEEN*/
