#include <stdio.h>

#include "common/platform.h"
#include "common/cs_file.h"
#include "mgos_app.h"
#include "mgos_gpio.h"
#include "mgos_sys_config.h"
#include "mgos_hal.h"
#include "mgos_dlsym.h"
#include "mgos.h"
#include "mgos_dht.h"
#include "mgos_http_server.h"
#include "mgos_dns_sd.h"

static struct mgos_dht *s_dht = NULL;

static void root_handler(struct mg_connection *nc, int ev, void *p, void *user_data) {
  float t = mgos_dht_get_temp(s_dht);
  float h = mgos_dht_get_humidity(s_dht);

  (void) p;
  if (ev != MG_EV_HTTP_REQUEST) return;
  mg_send_response_line(nc, 200, "Content-Type: text/html\r\n");
  mg_printf(nc, "Temperature: %f\r\n", t);
  mg_printf(nc, "Humidity: %f\r\n", h);
  nc->flags |= MG_F_SEND_AND_CLOSE;
  (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  // Reading from PIN 23
  if ((s_dht = mgos_dht_create(23, DHT22)) == NULL) return MGOS_APP_INIT_ERROR;

  mgos_register_http_endpoint("/", root_handler, NULL);

  return MGOS_APP_INIT_SUCCESS;
}

