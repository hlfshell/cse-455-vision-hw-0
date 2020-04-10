#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
	if(x < 0){
		x = 0;
	} else if(x >= im.w) {
		x = im.w - 1;
	}

	if(y < 0){
		y = 0;
	} else if(y >= im.h){
		y = im.h - 1;
	}

	if(c < 0){
		c = 0;
	} else if(c > im.c){
		c = im.c - 1;
	}

	int index = (im.h * im.w * c) + (im.w*y) + x;
    return im.data[index];
	
}

void set_pixel(image im, int x, int y, int c, float v)
{
	if(x < 0 || x >= im.w){
		return;
	} else if(y < 0 || y >= im.h){
		return;
	} else if(c < 0 || c >= im.c){
		return;
	}

	int index = (im.h * im.w * c) + (im.w*y) + x;
	im.data[index] = v;
	return;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
	for(int i = 0; i < im.w*im.h*im.c; i++){
		copy.data[i] = im.data[i];
	}
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int i = 0; i < im.w * im.h; i++){
		int y = i / gray.w;
		int x = i % gray.w;
		float r = get_pixel(im, x, y, 0);
		float g = get_pixel(im, x, y, 1);
		float b = get_pixel(im, x, y, 2);

		set_pixel(gray, x, y, 0, (0.299 * r) + (0.587 * g) + (0.114 * b));
	}
	

   return gray;
}

void shift_image(image im, int c, float v)
{
	for(int y = 0; y < im.h; y++){
		for(int x = 0; x < im.w; x++){
			float p = get_pixel(im, x, y, c);
			set_pixel(im, x, y, c, p + v);
		}
	}
}

void clamp_image(image im)
{
    for(int i = 0; i < im.h*im.w*im.c; i++){
		if(im.data[i] < 0){
			im.data[i] = 0;
		} else if(im.data[i] > 1){
			im.data[i] = 1;
		}
	}
}


// These might be handy
float three_way_v(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_m(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    float r, g, b, h, s, v, c, hp, m;

	for(int y = 0; y < im.h; y++){
		for(int x = 0; x < im.w; x++){
			r = get_pixel(im, x, y, 0);
			g = get_pixel(im, x, y, 1);
			b = get_pixel(im, x, y, 2);

			v = three_way_v(r, g, b);
			
			m = three_way_m(r, g, b);
			c = v - m;
			if(v == 0){
				s = 0;
			} else {
				s = c / v;
			}


			if(c == 0){
				h = 0;
			} else {
				if(v == r){
					hp = (g - b) / c;
				} else if(v == g){
					hp = ((b - r) / c) + 2;
				} else {
					hp = ((r - g) / c) + 4;
				}

				if(hp < 0){
					h = (hp/6) + 1;
				} else {
					h = (hp/6);
				}
			}

			set_pixel(im, x, y, 0, h);
			set_pixel(im, x, y, 1, s);
			set_pixel(im, x, y, 2, v);
		}
	}
}

void hsv_to_rgb(image im)
{
	float h, s, v, r, g, b;

	for(int y = 0; y < im.h; y++){
		for(int x = 0; x < im.w; x++){
			h = get_pixel(im, x, y, 0);
			s = get_pixel(im, x, y, 1);
			v = get_pixel(im, x, y, 2);

			// if(h == 0) {
			// 	set_pixel(im, x, y, 0, 0);
			// 	set_pixel(im, x, y, 0, 0);
			// 	set_pixel(im, x, y, 0, 0);
			// 	continue;
			// }

			float c = s * v;
			float m = v - c;
		
			float h_temp = h * 6;

			if(c == 0){
				r = v;
				g = v;
				b = v;
			} else if (h_temp > 5 && h_temp < 6) {
                r = v;
                g = m;
                b = ((((h_temp /  6) - 1) * 6 * c) - g) * -1;
            } else if (h_temp == 5) {
                r = v;
                g = m;
                b = v;
            } else if (h_temp < 5 && h_temp > 4) {
                g = m;
                r = (h_temp - 4) * c + g;
                b = v;
            } else if (h_temp == 4) {
                r = m;
                g = m;
                b = v;
            } else if (h_temp < 4 && h_temp > 3) {
                r = m;
                g = (((h_temp - 4) * c) - r) * -1;
                b = v;
            } else if (h_temp == 3) {
                r = m;
                g = v;
                b = v;
            } else if (h_temp < 3 && h_temp > 2) {
                r = m;
                g = v;
                b = ((h_temp - 2) * c) + r;
            } else if (h_temp == 2) {
                r = m;
                g = v;
                b = m;
            } else if (h_temp < 2 && h_temp > 1) {
                g = v;
                b = m;
                r = (((h_temp - 2) * c) - b) * -1;
            } else if (h_temp == 1) {
                r = v;
                g = v;
                b = m;
            } else if (h_temp < 1 && h_temp > 0) {
                r = v;
                b = m;
                g = (h_temp * c) + b;
            } else {
                r = v;
                g = m;
                b = m;
            }

			set_pixel(im, x, y, 0, r);
			set_pixel(im, x, y, 1, g);
			set_pixel(im, x, y, 2, b);
		}
	}
}
