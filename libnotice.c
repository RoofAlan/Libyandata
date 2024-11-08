#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON.h"

#define BUFFER_SIZE 4096

size_t write_callback_notice(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t real_size = size * nmemb;
	char *buffer = (char *)userp;
	strncpy(buffer + strlen(buffer), contents, real_size);
	return real_size;
}

void fetch_json_notice(const char *url, char *buffer, size_t buffer_size) {
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_notice);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}

int notice(const char *url) {
	//const char *url = "https://w.yandata.cn/api/fN59g96iubWs17Xi";
	char buffer[BUFFER_SIZE] = {0}; // 初始化缓冲区

	fetch_json_notice(url, buffer, sizeof(buffer));

	if (buffer[0] == '\0') {
		printf("Failed to fetch data or empty response.\n");
		return 1;
	}

	cJSON *json = cJSON_Parse(buffer);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
	} else {
		cJSON *msg = cJSON_GetObjectItem(json, "msg");
		if (msg != NULL && cJSON_IsObject(msg)) {
			cJSON *app_gg = cJSON_GetObjectItem(msg, "app_gg");
			if (app_gg != NULL && cJSON_IsString(app_gg)) {
				printf("公告: %s\n", app_gg->valuestring);
			} else {
				printf("app_gg not found\n");
			}
		} else {
			printf("msg not found\n");
		}
		cJSON_Delete(json);
	}

	return 0;
}
