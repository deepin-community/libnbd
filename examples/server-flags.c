/* This example shows how to connect to an NBD
 * server and print the export flags.
 *
 * You can test it with nbdkit like this:
 *
 * nbdkit -U - memory 1M \
 *   --run './server-flags $unixsocket'
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libnbd.h>

int
main (int argc, char *argv[])
{
  struct nbd_handle *nbd;
  char *str;
  int flag;

  if (argc != 2) {
    fprintf (stderr, "%s socket\n", argv[0]);
    exit (EXIT_FAILURE);
  }

  /* Create the libnbd handle. */
  nbd = nbd_create ();
  if (nbd == NULL) {
    fprintf (stderr, "%s\n", nbd_get_error ());
    exit (EXIT_FAILURE);
  }

  /* Request full information. */
#if LIBNBD_HAVE_NBD_SET_FULL_INFO /* Added in 1.4 */
  if (nbd_set_full_info (nbd, true) == -1) {
    fprintf (stderr, "%s\n", nbd_get_error ());
    exit (EXIT_FAILURE);
  }
#endif

  /* Connect to the NBD server over a
   * Unix domain socket.
   */
  if (nbd_connect_unix (nbd, argv[1]) == -1) {
    fprintf (stderr, "%s\n", nbd_get_error ());
    exit (EXIT_FAILURE);
  }

  /* See if the server provided extra details,
   * using functions added in 1.4
   */
#if LIBNBD_HAVE_NBD_GET_EXPORT_DESCRIPTION
  str = nbd_get_canonical_export_name (nbd);
  if (str)
    printf ("canonical_name = %s\n", str);
  free (str);
  str = nbd_get_export_description (nbd);
  if (str)
    printf ("description = %s\n", str);
  free (str);
#endif

  /* Read and print the flags. */
#define PRINT_FLAG(flag_fn)                     \
  flag = flag_fn (nbd);                         \
  if (flag == -1) {                             \
    fprintf (stderr, "%s\n", nbd_get_error ()); \
    exit (EXIT_FAILURE);                        \
  }                                             \
  printf (#flag_fn " = %s\n",                   \
          flag ? "true" : "false");

  PRINT_FLAG (nbd_can_cache);
  PRINT_FLAG (nbd_can_df);
  PRINT_FLAG (nbd_can_flush);
  PRINT_FLAG (nbd_can_fua);
  PRINT_FLAG (nbd_can_multi_conn);
  PRINT_FLAG (nbd_can_trim);
  PRINT_FLAG (nbd_can_zero);
#if LIBNBD_HAVE_NBD_CAN_FAST_ZERO
  /* Added in 1.2 */
  PRINT_FLAG (nbd_can_fast_zero);
#endif
#if LIBNBD_HAVE_NBD_CAN_BLOCK_STATUS_PAYLOAD
  /* Added in 1.18 */
  PRINT_FLAG (nbd_can_block_status_payload);
#endif
  PRINT_FLAG (nbd_is_read_only);
  PRINT_FLAG (nbd_is_rotational);

  /* Close the libnbd handle. */
  nbd_close (nbd);

  exit (EXIT_SUCCESS);
}
