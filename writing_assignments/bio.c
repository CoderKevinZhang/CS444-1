struct bio {
   sector_t bi_sector; /* associated sector on disk */
   struct bio *bi_next; /* list of requests */
   struct block_device *bi_bdev; /* associated block device */
   unsigned long bi_flags; /* status and command flags */
   unsigned long bi_rw; /* read or write? */
   unsigned short bi_vcnt; /* number of bio_vecs off */
   unsigned short bi_idx; /* current index in bi_io_vec */
   unsigned short bi_phys_segments; /* number of segments */
   unsigned int bi_size; /* I/O count */
   unsigned int bi_seg_front_size; /* size of first segment */
   unsigned int bi_seg_back_size; /* size of last segment */
   unsigned int bi_max_vecs; /* maximum bio_vecs possible */
   unsigned int bi_comp_cpu; /* completion CPU */
   atomic_t bi_cnt; /* usage counter */
   struct bio_vec *bi_io_vec; /* bio_vec list */
   bio_end_io_t *bi_end_io; /* I/O completion method */
   void *bi_private; /* owner-private method */
   bio_destructor_t *bi_destructor; /* destructor method */
   struct bio_vec bi_inline_vecs[0]; /* inline bio vectors */
};
