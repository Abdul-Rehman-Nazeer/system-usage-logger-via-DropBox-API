#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <curl/curl.h>

#define LOG_FILE "system_usage.log"
#define DROPBOX_TOKEN "INSERT YOUR API TOKEN HERE"

// Function to log detailed system usage (CPU, Memory, Disk, Processes, Network, Swap)
void log_system_usage() {
    FILE *log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    fprintf(log, "[%s]\n", ctime(&now));

    // Log CPU usage
    FILE *cpu_file = fopen("/proc/stat", "r");
    if (cpu_file != NULL) {
        char buffer[256];
        fgets(buffer, sizeof(buffer), cpu_file); // Read the first line with CPU info
        fprintf(log, "CPU Usage: %s", buffer);
        fclose(cpu_file);
    }

    // Log memory usage
    FILE *mem_file = fopen("/proc/meminfo", "r");
    if (mem_file != NULL) {
        char buffer[256];
        for (int i = 0; i < 2 && fgets(buffer, sizeof(buffer), mem_file); i++) {
            fprintf(log, "%s", buffer); // Log first two lines: MemTotal and MemFree
        }
        fclose(mem_file);
    }

    // Log Disk usage
    struct statvfs disk_info;
    if (statvfs("/", &disk_info) == 0) {
        unsigned long total_disk = disk_info.f_blocks * disk_info.f_frsize;
        unsigned long free_disk = disk_info.f_bfree * disk_info.f_frsize;
        unsigned long used_disk = total_disk - free_disk;
        fprintf(log, "Disk Usage: Total: %lu MB, Used: %lu MB, Free: %lu MB\n", 
                total_disk / (1024 * 1024), used_disk / (1024 * 1024), free_disk / (1024 * 1024));
    }

    // Log number of running processes
    FILE *proc_file = fopen("/proc/loadavg", "r");
    if (proc_file != NULL) {
        char buffer[256];
        fgets(buffer, sizeof(buffer), proc_file);
        fprintf(log, "Processes: %s", buffer);
        fclose(proc_file);
    }

    // Log network usage
    FILE *net_file = fopen("/proc/net/dev", "r");
    if (net_file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), net_file)) {
            if (strstr(buffer, "eth0") != NULL || strstr(buffer, "wlan0") != NULL) {  // Filter for network interfaces
                fprintf(log, "Network Usage (eth0/wlan0): %s", buffer);
            }
        }
        fclose(net_file);
    }

    // Log Swap usage
    FILE *swap_file = fopen("/proc/meminfo", "r");
    if (swap_file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), swap_file)) {
            if (strstr(buffer, "SwapTotal") != NULL || strstr(buffer, "SwapFree") != NULL) {
                fprintf(log, "%s", buffer);
            }
        }
        fclose(swap_file);
    }

    fprintf(log, "\n");
    fclose(log);
}

// Function to upload file to Dropbox
void upload_to_dropbox(const char *file_path) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        FILE *file = fopen(file_path, "rb");
        if (!file) {
            perror("Failed to open log file for uploading");
            return;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *buffer = malloc(file_size);
        fread(buffer, 1, file_size, file);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", DROPBOX_TOKEN);
        headers = curl_slist_append(headers, auth_header);

        headers = curl_slist_append(headers, "Dropbox-API-Arg: {\"path\": \"/system_usage.log\",\"mode\": \"overwrite\"}");

        curl_easy_setopt(curl, CURLOPT_URL, "https://content.dropboxapi.com/2/files/upload");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, file_size);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Log file successfully uploaded to Dropbox.\n");
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(buffer);
        fclose(file);
    }

    curl_global_cleanup();
}

int main() {
    while (1) {
        log_system_usage();
        upload_to_dropbox(LOG_FILE);
        sleep(60); // Log and sync every minute
    }
    return 0;
}
