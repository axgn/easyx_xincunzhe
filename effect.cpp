#include "eff.h"

void overturn(DWORD* buffer,int width,int height, IMAGE* temp)
{
	Resize(temp, width, height);
	DWORD* buffer2 = GetImageBuffer(temp);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int idx1 = y * width + x;
			int idx2 = y * width + width - x - 1;
			buffer2[idx1] = buffer[idx2];
		}
	}
}
void turn_white(IMAGE* src,IMAGE* des)
{
	DWORD* color_buffer_raw = GetImageBuffer(src);
	DWORD* color_buffer_sketch = GetImageBuffer(des);
	int width = src->getwidth();
	int height = src->getheight();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int idx = y * width + x;
			if ((color_buffer_raw[idx] & 0xFF000000) >> 24)
				color_buffer_sketch[idx] = BGR(RGB(255, 255, 255)) | (((DWORD)(BYTE)(255)) << 24);
		}
	}
}