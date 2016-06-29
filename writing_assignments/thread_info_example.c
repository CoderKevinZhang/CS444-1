struct thread_info {
   struct task_struct *task;
   struct exec_domain *exec_domain;
   __u32 flags;
   __u32 status;
   __u32 cpu;
   int preempt_count;
   mm_segment_t addr_limit;
   struct restart_block restart_block;
   void *sysenter_return;
   int uaccess_err;
};
