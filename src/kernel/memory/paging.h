#ifndef __PAGING_H_
#define __PAGING_H_

extern "C" long kstart;
extern "C" long kend;
extern "C" void load_page_dir(unsigned int *);
void setup_paging();
void pagefault_handler(struct registers *r);
void exec(uint8_t *prog);

#endif // __PAGING_H_