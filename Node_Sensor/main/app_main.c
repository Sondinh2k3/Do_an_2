
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "dht11.h"

#include <inttypes.h>
#include "ra02.h"

// =============================================================================================

void task_tx(void *pvParameters)
{
    ESP_LOGI(pcTaskGetName(NULL), "Start");
    uint8_t buf[256]; // Maximum Payload size of SX1276/77/78/79 is 255
    while (1)
    {
        TickType_t nowTick = xTaskGetTickCount();
        int send_len = sprintf((char *)buf, "Hello World!! %" PRIu32, nowTick);
        lora_send_packet(buf, send_len);
        ESP_LOGI(pcTaskGetName(NULL), "%d byte packet sent...", send_len);
        int lost = lora_packet_lost();
        if (lost != 0)
        {
            ESP_LOGW(pcTaskGetName(NULL), "%d packets lost", lost);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    } // end while
}

void app_main()
{
    if (lora_init() == 0)
    {
        ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
        while (1)
        {
            vTaskDelay(1);
        }
    }

    ESP_LOGI(pcTaskGetName(NULL), "Frequency is 433MHz");
    lora_set_frequency(433e6); // 433MHz

    lora_enable_crc();

    int cr = 1;
    int bw = 7;
    int sf = 7;
#if CONFIF_ADVANCED
    cr = CONFIG_CODING_RATE
        bw = CONFIG_BANDWIDTH;
    sf = CONFIG_SF_RATE;
#endif

    lora_set_coding_rate(cr);
    // lora_set_coding_rate(CONFIG_CODING_RATE);
    // cr = lora_get_coding_rate();
    ESP_LOGI(pcTaskGetName(NULL), "coding_rate=%d", cr);

    lora_set_bandwidth(bw);
    // lora_set_bandwidth(CONFIG_BANDWIDTH);
    // int bw = lora_get_bandwidth();
    ESP_LOGI(pcTaskGetName(NULL), "bandwidth=%d", bw);

    lora_set_spreading_factor(sf);
    // lora_set_spreading_factor(CONFIG_SF_RATE);
    // int sf = lora_get_spreading_factor();
    ESP_LOGI(pcTaskGetName(NULL), "spreading_factor=%d", sf);

    xTaskCreate(&task_tx, "TX", 1024 * 3, NULL, 5, NULL);
}