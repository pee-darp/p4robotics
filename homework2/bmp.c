#include "bmp.h"
#include <string.h>

// calculate the number of bytes of memory needed to serialize the bitmap
// that is, to write a valid bmp file to memory
size_t bmp_calculate_size(bitmap_t *bmp) {
    size_t sz_fh = sizeof(BITMAPFILEHEADER);
    size_t sz_ih = sizeof(BITMAPINFOHEADER);
    size_t sz_img = bmp->width * bmp->height * sizeof(color_bgr_t);
    return sz_fh + sz_ih + sz_img;
}

// write the bmp file to memory at data, which must be at least
// bmp_calculate_size large.
void bmp_serialize(bitmap_t *bmp, uint8_t *data) {
    BITMAPFILEHEADER file_header = {0};
    file_header.F_bfType = 'B';
    file_header.L_bfType = 'M';
    file_header.bfSize = bmp_calculate_size(bmp); //need to give 921600
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER info_header = {0};
    info_header.biSize = sizeof(BITMAPINFOHEADER);
    info_header.biWidth = bmp->width;
    info_header.biHeight = bmp->height; //bottom up DIB
    info_header.biPlanes = 1;
    info_header.biBitCount = 24;
    info_header.biCompression = 0; //ask acshii
    info_header.biSizeImage = 0;
    info_header.biXPelsPerMeter = 2835;
    info_header.biYPelsPerMeter = 2835;

    uint8_t *data_out = data;
    memcpy(data_out, &file_header, sizeof(file_header));
    data_out += sizeof(file_header);
    memcpy(data_out, &info_header, sizeof(info_header));
    data_out += sizeof(info_header);

    for (int row = 0; row < bmp->height; row++) {
        memcpy(data_out, &bmp->data[row * bmp->width], bmp->width * sizeof(color_bgr_t));
        data_out += bmp->width * sizeof(color_bgr_t);
    }
}
