#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define BUFFER_SIZE 4096

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t real_size = size * nmemb;
	char *buffer = (char *)userp;
	strncpy(buffer + strlen(buffer), contents, real_size);
	return real_size;
}

void fetch_json(const char *url, char *buffer, size_t buffer_size) {
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}

int kami_check(const char *yan_api, int app_id, const char *kami) {
	char url[BUFFER_SIZE]; // 确保url数组足够大
	sprintf(url, "%s?app=%d&kami=%s", yan_api, app_id, kami);
	char buffer[BUFFER_SIZE] = {0}; // 初始化缓冲区

	fetch_json(url, buffer, sizeof(buffer));

	if (buffer[0] == '\0') {
		//printf("Failed to fetch data or empty response.\n");
		return 1;
	}

	cJSON *json = cJSON_Parse(buffer);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
	} else {
		cJSON *code = cJSON_GetObjectItem(json, "code");
		if (code != NULL && cJSON_IsNumber(code)) {
			if (code->valueint == 149) {
				cJSON_Delete(json);
				return 1;
			} else if (code->valueint == 200) {
				cJSON *msg = cJSON_GetObjectItem(json, "msg");
				if (msg != NULL && cJSON_IsObject(msg)) {
					cJSON *vip = cJSON_GetObjectItem(msg, "vip");
					if (vip != NULL && cJSON_IsString(vip)) {
						printf("VIP: %s\n", vip->valuestring);
						cJSON_Delete(json);
						return 0;
					} else {
						printf("VIP not found\n");
						cJSON_Delete(json);
						return 1;
					}
				} else {
					printf("msg not found\n");
				}
			} else {
				//printf("Unexpected code: %d\n", code->valueint);
				cJSON_Delete(json);
				return 1;
			}
		} else {
			printf("code not found\n");
		}
		cJSON_Delete(json);
	}

	return 0;
}

/* int main() {
	char kami[100];
	printf("Enter Kami: ");
	fgets(kami, sizeof(kami), stdin); // 使用fgets代替scanf
	kami[strcspn(kami, "\n")] = 0; // 去除fgets读取的换行符

	if(kami_check("https://w.yandata.cn/api/3ehXWJv4p7KZDSIt", 525, kami) == 1) {
		printf("False\n");
		return 1;
	} else {
		printf("True\n");
		return 0;
	}
} */
