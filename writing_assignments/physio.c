void physio (
   device dev,
   struct uio *uio,
   int ioflag);
{
   allocate a swap buffer;
   while (uio is not exhausted) {
      mark the buffer busy;
      set up a maximum-size transfer;
      use device maximum I/O size to bound
	 the transfer size;
      check user read/write access at uio location;
      lock the part of the user address space
	 involved in the transfer into RAM;
      map the user pages into the buffer;
      call dev->strategy() to start the transfer;
      wait as the transfer completes;
      unmap the user pages from the buffer;
      unlock the part of the address space
	 previously locked;
      deduct the transfer size from the total number
	 of data to transfer
   }
   free swap buffer;
}
