#include "printf.h"
#include "utils.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "mini_uart.h"

void process(char *array)
{
	while (1){
		for (int i = 0; i < 5; i++){
			uart_send(array[i]);
			delay(1000000);
		}
	}
}

void kernel_main(void)
{
	uart_init();
	init_printf(0, putc);
	irq_vector_init();
	timer_init();
	enable_interrupt_controller();
	enable_irq();

	//se crean 3 procesos con diferentes prioridades para probar el scheduler
	// el tercer parametro (int) es la prioridad
	int res = copy_process((unsigned long)&process, (unsigned long)"12345", (int)2);
	if (res != 0) {
		printf("error while starting process 1");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"abcde", (int)12);
	if (res != 0) {
		printf("error while starting process 2");
		return;
	}
	res = copy_process((unsigned long)&process, (unsigned long)"ABCDE", (int)6);
	if (res != 0) {
		printf("error while starting process 3");
		return;
	}

	while (1){
		schedule();
	}
}
