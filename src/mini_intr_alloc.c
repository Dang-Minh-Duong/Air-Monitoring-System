#include <stdint.h>

/* Base address of DPORT peripheral registers */
#define DPORT_BASE                 0x3FF00000
/* Base address for interrupt matrix */
#define INTERRUPT_MATRIX_BASE      (DPORT_BASE + 0x0484)
/* Register to map peripheral interrupts to CPU interrupt lines */
#define INTR_MATRIX_REG            0x3FF0013C 
/* Register to enable CPU interrupt lines */
#define CPU_INT_ENABLE_REG         0x3FF000C0
/* Base address of CPU interrupt vector table */
#define INTERRUPT_VECTOR_TABLE     0x40080000

/* Use CPU interrupt line number 3 for timer or any other peripheral */
#define MINI_CPU_INT_NUM           3

/**
 * Getter function to access the stored argument inside the ISR if needed
 */
void *MiniIntr_GetArg() {
    return s_intr_arg;
}

/**
 * Register an ISR handler for the given interrupt source without argument passing.
 * This is the simplest standard version.
 *
 * @param source  The interrupt source peripheral number (not used here but typically required)
 * @param handler Pointer to the interrupt service routine function
 */
void Mini_Intr_Alloc(int source, void (*handler)(void *)) {
    /* 1. Map the peripheral interrupt source to CPU interrupt line */
    volatile uint32_t *matrix = (volatile uint32_t *)INTR_MATRIX_REG;
    *matrix = (uint32_t)MINI_CPU_INT_NUM;

    /* 2. Enable the CPU interrupt line */
    volatile uint32_t *int_enable = (volatile uint32_t *)CPU_INT_ENABLE_REG;
    *int_enable |= (1 << MINI_CPU_INT_NUM);

    /* 3. Assign the ISR handler address into the CPU interrupt vector table (core 0) */
    volatile uint32_t *vector_table = (volatile uint32_t *)INTERRUPT_VECTOR_TABLE;
    vector_table[MINI_CPU_INT_NUM * 4] = (uint32_t)handler;
}
