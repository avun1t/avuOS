#ifndef __HEAP_H_
#define __HEAP_H_

typedef struct Frame {
	uint32_t num; // the number of the frame
	uint32_t set; // the set the frame is in (8 frames per set)
	uint32_t pos; // which number in the set it is
};

typedef struct FrameSet {
	Frame start;
	uint32_t len;
} FrameSet;

void init_heap();
Frame first_available_frame();
Frame falloc();
void ffree(Frame f);
FrameSet first_available_frameset(uint32_t len);
Frame get_frame(uint32_t i);
FrameSet fsalloc();
void *kmalloc(uint32_t len);
void kfree(void *ptr, uint32_t len);
char *String(char *str);
void strfree(char *str);

#endif // __HEAP_H_