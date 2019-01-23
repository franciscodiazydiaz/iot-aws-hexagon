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
#include "mgos_prometheus_metrics.h"

static struct mgos_dht *s_dht = NULL;

static void root_handler(struct mg_connection *nc, int ev, void *p, void *user_data) {
  float t = mgos_dht_get_temp(s_dht);
  float h = mgos_dht_get_humidity(s_dht);

  (void) p;
  if (ev != MG_EV_HTTP_REQUEST) return;

  mg_printf(nc, "HTTP/1.1 200 OK\r\n");
  mg_send_response_line(nc, 200, "Content-Type: text/plain\r\n");
  mg_printf(nc, "Temperature: %f\r\n", t);
  mg_printf(nc, "Humidity: %f\r\n", h);

  nc->flags |= MG_F_SEND_AND_CLOSE;
  (void) user_data;
}

static void prometheus_metrics_dht22(struct mg_connection *nc, void *user_data) {

  mgos_prometheus_metrics_printf(nc, GAUGE, "temperature", "SysEng temperature in Celcius",
                                 "%.2f", mgos_dht_get_temp(s_dht));
  mgos_prometheus_metrics_printf(nc, GAUGE, "humidity", "SysEng humidity percentage",
                                 "%.2f", mgos_dht_get_humidity(s_dht));
  (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
  // Reading from PIN 23
  if ((s_dht = mgos_dht_create(23, DHT22)) == NULL) return MGOS_APP_INIT_ERROR;

  mgos_register_http_endpoint("/", root_handler, NULL);
  mgos_prometheus_metrics_add_handler(prometheus_metrics_dht22, NULL);

  return MGOS_APP_INIT_SUCCESS;
}

