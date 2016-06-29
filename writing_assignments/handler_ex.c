static irqreturn_t rtc_interrupt(int irq, void *dev)
{
   /*
    * * Can be an alarm interrupt, update complete interrupt,
    * * or a periodic interrupt. We store the status in the
    * * low byte and the number of interrupts received since
    * * the last read in the remainder of rtc_irq_data.
    * */
   spin_lock(&rtc_lock);
   rtc_irq_data += 0x100;
   rtc_irq_data &= ~0xff;
   rtc_irq_data |= (CMOS_READ(RTC_INTR_FLAGS) & 0xF0);
   if (rtc_status & RTC_TIMER_ON)
      mod_timer(&rtc_irq_timer, jiffies + HZ/rtc_freq + 2*HZ/100);
   spin_unlock(&rtc_lock);
   /*
    * * Now do the rest of the actions
    * */
   spin_lock(&rtc_task_lock);
   if (rtc_callback)
      rtc_callback->func(rtc_callback->private_data);
   spin_unlock(&rtc_task_lock);
   wake_up_interruptible(&rtc_wait);
   kill_fasync(&rtc_async_queue, SIGIO, POLL_IN);
   return IRQ_HANDLED;
}
