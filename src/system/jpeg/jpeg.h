#ifndef __JPEG_H__
#define __JPEG_H__

bool 		jpeg_decode_mem(const uint8_t *src_data_ptr, size_t src_data_len, uint8_t **dst_data_ptr, unsigned * w, unsigned *h);
uint8_t * 	jpeg_encode_mem(unsigned char *src_data, unsigned image_width, unsigned image_height, int quality, long unsigned int * dst_data_len);

#endif



