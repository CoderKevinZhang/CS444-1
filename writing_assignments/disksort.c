void disksort(
      drive queue *dq,
      buffer *bp);
{
   if(active list is empty) {
      place the buffer at the front of the active list;
      return;
   }
   if(request lies before the first active request) {
      locate the beginning of the next-pass list;
      sort bp into the next-pass list;
   } else
      sort bp into the active list;
}
