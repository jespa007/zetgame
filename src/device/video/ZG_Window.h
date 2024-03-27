#ifndef __ZG_WINDOW_H__
#define __ZG_WINDOW_H__

typedef struct ZG_Window ZG_Window;

struct ZG_Window{
	void *data;
};

ZG_Window *ZG_Window_New(
		const char *title
		, int x
		, int y
		, uint16_t _width
		, uint16_t _height
		, uint32_t flags
);

void ZG_Window_Delete(ZG_Window *_this);

#endif
