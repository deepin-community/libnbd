# NBD client library in userspace
# WARNING: THIS FILE IS GENERATED FROM
# generator/generator
# ANY CHANGES YOU MAKE TO THIS FILE WILL BE LOST.
#
# Copyright Red Hat
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

api_built += \
	nbd_set_debug \
	nbd_get_debug \
	nbd_set_debug_callback \
	nbd_clear_debug_callback \
	nbd_stats_bytes_sent \
	nbd_stats_chunks_sent \
	nbd_stats_bytes_received \
	nbd_stats_chunks_received \
	nbd_set_handle_name \
	nbd_get_handle_name \
	nbd_set_private_data \
	nbd_get_private_data \
	nbd_set_export_name \
	nbd_get_export_name \
	nbd_set_request_block_size \
	nbd_get_request_block_size \
	nbd_set_full_info \
	nbd_get_full_info \
	nbd_get_canonical_export_name \
	nbd_get_export_description \
	nbd_set_tls \
	nbd_get_tls \
	nbd_get_tls_negotiated \
	nbd_set_tls_certificates \
	nbd_set_tls_verify_peer \
	nbd_get_tls_verify_peer \
	nbd_set_tls_username \
	nbd_get_tls_username \
	nbd_set_tls_hostname \
	nbd_get_tls_hostname \
	nbd_set_tls_psk_file \
	nbd_set_request_extended_headers \
	nbd_get_request_extended_headers \
	nbd_get_extended_headers_negotiated \
	nbd_set_request_structured_replies \
	nbd_get_request_structured_replies \
	nbd_get_structured_replies_negotiated \
	nbd_set_request_meta_context \
	nbd_get_request_meta_context \
	nbd_set_handshake_flags \
	nbd_get_handshake_flags \
	nbd_set_pread_initialize \
	nbd_get_pread_initialize \
	nbd_set_strict_mode \
	nbd_get_strict_mode \
	nbd_set_opt_mode \
	nbd_get_opt_mode \
	nbd_opt_go \
	nbd_opt_abort \
	nbd_opt_starttls \
	nbd_opt_extended_headers \
	nbd_opt_structured_reply \
	nbd_opt_list \
	nbd_opt_info \
	nbd_opt_list_meta_context \
	nbd_opt_list_meta_context_queries \
	nbd_opt_set_meta_context \
	nbd_opt_set_meta_context_queries \
	nbd_add_meta_context \
	nbd_get_nr_meta_contexts \
	nbd_get_meta_context \
	nbd_clear_meta_contexts \
	nbd_set_uri_allow_transports \
	nbd_set_uri_allow_tls \
	nbd_set_uri_allow_local_file \
	nbd_connect_uri \
	nbd_connect_unix \
	nbd_connect_vsock \
	nbd_connect_tcp \
	nbd_connect_socket \
	nbd_connect_command \
	nbd_connect_systemd_socket_activation \
	nbd_set_socket_activation_name \
	nbd_get_socket_activation_name \
	nbd_is_read_only \
	nbd_can_flush \
	nbd_can_fua \
	nbd_is_rotational \
	nbd_can_trim \
	nbd_can_zero \
	nbd_can_fast_zero \
	nbd_can_block_status_payload \
	nbd_can_df \
	nbd_can_multi_conn \
	nbd_can_cache \
	nbd_can_meta_context \
	nbd_get_protocol \
	nbd_get_size \
	nbd_get_block_size \
	nbd_pread \
	nbd_pread_structured \
	nbd_pwrite \
	nbd_shutdown \
	nbd_flush \
	nbd_trim \
	nbd_cache \
	nbd_zero \
	nbd_block_status \
	nbd_block_status_64 \
	nbd_block_status_filter \
	nbd_poll \
	nbd_poll2 \
	nbd_aio_connect \
	nbd_aio_connect_uri \
	nbd_aio_connect_unix \
	nbd_aio_connect_vsock \
	nbd_aio_connect_tcp \
	nbd_aio_connect_socket \
	nbd_aio_connect_command \
	nbd_aio_connect_systemd_socket_activation \
	nbd_aio_opt_go \
	nbd_aio_opt_abort \
	nbd_aio_opt_starttls \
	nbd_aio_opt_extended_headers \
	nbd_aio_opt_structured_reply \
	nbd_aio_opt_list \
	nbd_aio_opt_info \
	nbd_aio_opt_list_meta_context \
	nbd_aio_opt_list_meta_context_queries \
	nbd_aio_opt_set_meta_context \
	nbd_aio_opt_set_meta_context_queries \
	nbd_aio_pread \
	nbd_aio_pread_structured \
	nbd_aio_pwrite \
	nbd_aio_disconnect \
	nbd_aio_flush \
	nbd_aio_trim \
	nbd_aio_cache \
	nbd_aio_zero \
	nbd_aio_block_status \
	nbd_aio_block_status_64 \
	nbd_aio_block_status_filter \
	nbd_aio_get_fd \
	nbd_aio_get_direction \
	nbd_aio_notify_read \
	nbd_aio_notify_write \
	nbd_aio_is_created \
	nbd_aio_is_connecting \
	nbd_aio_is_negotiating \
	nbd_aio_is_ready \
	nbd_aio_is_processing \
	nbd_aio_is_dead \
	nbd_aio_is_closed \
	nbd_aio_command_completed \
	nbd_aio_peek_command_completed \
	nbd_aio_in_flight \
	nbd_connection_state \
	nbd_get_package_name \
	nbd_get_version \
	nbd_kill_subprocess \
	nbd_get_subprocess_pid \
	nbd_supports_tls \
	nbd_supports_vsock \
	nbd_supports_uri \
	nbd_get_uri \
	nbd_is_uri \
	$(NULL)
