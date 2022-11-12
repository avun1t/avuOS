#ifndef __AVUOS_IRQHANDLER_H_
#define __AVUOS_IRQHANDLER_H_

#include <kernel/kstddef.h>

class IRQHandler {
public:
	IRQHandler(int irq);
	virtual void handle_irq(registers *reg) = 0;
};

#endif // __AVUOS_IRQHANDLER_H_