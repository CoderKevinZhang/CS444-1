if (request_irq(irqn, my_interrupt, IRQF_SHARED, "my_device", my_dev)) {
   printk(KERN_ERR "my_device: cannot register IRQ %d\n", irqn);
   return -EIO;
}
