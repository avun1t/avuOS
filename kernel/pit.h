#ifndef __PIT_H_
#define __PIT_H_

#define PIT_COUNTER0 0x40
#define PIT_COUNTER1 0x41
#define PIT_COUNTER2 0x42
#define PIT_CMD 0x43

void pit_handler();
void pit_init(uint32_t frequency);

#endif // __PIT_H_