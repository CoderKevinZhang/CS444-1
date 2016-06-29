/* request_irq: allocate a given interrupt line */
int request_irq(unsigned int irq,
      irq_handler_t handler,
      unsigned long flags,
      const char *name,
      void *dev)
