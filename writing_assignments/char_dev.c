/* Function prototypes */
static d_open_t      echo_open;
static d_close_t     echo_close;
static d_read_t      echo_read;
static d_write_t     echo_write;i

/* Character device entry points */
static struct cdevsw echo_cdevsw = {
   .d_version = D_VERSION,
   .d_open = echo_open,
   .d_close = echo_close,
   .d_read = echo_read,
   .d_write = echo_write,
   .d_name = "echo",
};
