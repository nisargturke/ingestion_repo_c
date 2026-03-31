#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peripheral.h"
#include "interface/keyinput.h"
#include "monitor.h"

// Structure to hold device data
typedef struct {
    int id;
    char name[50];
    int type;        // Device type
    int status;      // 0 = inactive, 1 = active
    void* config;    // Device-specific configuration
    int power_mode;  // 0 = off, 1 = low power, 2 = full power
    int error_code;  // Last error code
    char firmware_version[20];
    int buffer_size;
    void* buffer;
} Device;

// Service interface structure
typedef struct {
    // Basic device operations
    Device* (*get_device_by_id)(int id);
    int (*initialize_device)(Device* device);
    void (*configure_device)(Device* device);
    void (*shutdown_device)(int id);
    int (*get_device_status)(int id);
    
    // Additional device operations
    int (*reset_device)(int id);
    int (*update_firmware)(int id, const char* version);
    int (*set_power_mode)(int id, int mode);
    int (*get_error_code)(int id);
    void (*clear_error)(int id);
    
    // Buffer operations
    int (*allocate_buffer)(int id, int size);
    void (*free_buffer)(int id);
    int (*read_buffer)(int id, void* data, int size);
    int (*write_buffer)(int id, const void* data, int size);
    
    // Diagnostic operations
    char* (*get_firmware_version)(int id);
    int (*run_self_test)(int id);
    int (*get_power_consumption)(int id);
    
    // Configuration operations
    int (*save_config)(int id);
    int (*load_config)(int id);
    void (*reset_config)(int id);
} DeviceService;

// Mock device database
static Device* devices = NULL;
static int device_count = 0;
static int max_devices = 10;

// Service implementation functions (existing ones)
Device* get_device_by_id(int id) {
    for (int i = 0; i < device_count; i++) {
        if (devices[i].id == id) {
            return &devices[i];
        }
    }
    return NULL;
}

int service_initialize_device(Device* device) {
    if (device_count >= max_devices) return -1;
    devices[device_count] = *device;
    device_count++;
    return 0;
}

void service_configure_device(Device* device) {
    Device* existing = get_device_by_id(device->id);
    if (existing) *existing = *device;
}

void service_shutdown_device(int id) {
    Device* device = get_device_by_id(id);
    if (device) device->status = 0;
}

int service_get_device_status(int id) {
    Device* device = get_device_by_id(id);
    return device ? device->status : -1;
}

// New service implementation functions
int service_reset_device(int id) {
    Device* device = get_device_by_id(id);
    if (!device) return -1;
    device->status = 1;
    device->error_code = 0;
    return 0;
}

int service_update_firmware(int id, const char* version) {
    Device* device = get_device_by_id(id);
    if (!device) return -1;
    strncpy(device->firmware_version, version, 19);
    return 0;
}

int service_set_power_mode(int id, int mode) {
    Device* device = get_device_by_id(id);
    if (!device) return -1;
    device->power_mode = mode;
    return 0;
}

int service_get_error_code(int id) {
    Device* device = get_device_by_id(id);
    return device ? device->error_code : -1;
}

void service_clear_error(int id) {
    Device* device = get_device_by_id(id);
    if (device) device->error_code = 0;
}

int service_allocate_buffer(int id, int size) {
    Device* device = get_device_by_id(id);
    if (!device) return -1;
    device->buffer = malloc(size);
    device->buffer_size = size;
    return device->buffer ? 0 : -1;
}

void service_free_buffer(int id) {
    Device* device = get_device_by_id(id);
    if (device && device->buffer) {
        free(device->buffer);
        device->buffer = NULL;
        device->buffer_size = 0;
    }
}

int service_read_buffer(int id, void* data, int size) {
    Device* device = get_device_by_id(id);
    if (!device || !device->buffer) return -1;
    memcpy(data, device->buffer, size);
    return 0;
}

int service_write_buffer(int id, const void* data, int size) {
    Device* device = get_device_by_id(id);
    if (!device || !device->buffer) return -1;
    memcpy(device->buffer, data, size);
    return 0;
}

char* service_get_firmware_version(int id) {
    Device* device = get_device_by_id(id);
    return device ? device->firmware_version : NULL;
}

int service_run_self_test(int id) {
    Device* device = get_device_by_id(id);
    return device ? 0 : -1;
}

int service_get_power_consumption(int id) {
    Device* device = get_device_by_id(id);
    return device ? device->power_mode * 5 : -1;
}

int service_save_config(int id) {
    Device* device = get_device_by_id(id);
    return device ? 0 : -1;
}

int service_load_config(int id) {
    Device* device = get_device_by_id(id);
    return device ? 0 : -1;
}

void service_reset_config(int id) {
    Device* device = get_device_by_id(id);
    if (device) {
        device->power_mode = 0;
        device->error_code = 0;
    }
}

// Initialize the device service
DeviceService* initialize_device_service() {
    devices = (Device*)malloc(max_devices * sizeof(Device));
    if (!devices) return NULL;

    DeviceService* service = (DeviceService*)malloc(sizeof(DeviceService));
    if (!service) {
        free(devices);
        return NULL;
    }

    // Assign all function pointers
    service->get_device_by_id = get_device_by_id;
    service->initialize_device = service_initialize_device;
    service->configure_device = service_configure_device;
    service->shutdown_device = service_shutdown_device;
    service->get_device_status = service_get_device_status;
    service->reset_device = service_reset_device;
    service->update_firmware = service_update_firmware;
    service->set_power_mode = service_set_power_mode;
    service->get_error_code = service_get_error_code;
    service->clear_error = service_clear_error;
    service->allocate_buffer = service_allocate_buffer;
    service->free_buffer = service_free_buffer;
    service->read_buffer = service_read_buffer;
    service->write_buffer = service_write_buffer;
    service->get_firmware_version = service_get_firmware_version;
    service->run_self_test = service_run_self_test;
    service->get_power_consumption = service_get_power_consumption;
    service->save_config = service_save_config;
    service->load_config = service_load_config;
    service->reset_config = service_reset_config;

    return service;
}

void cleanup_device_service(DeviceService* service) {
    if (devices) {
        for (int i = 0; i < device_count; i++) {
            if (devices[i].buffer) {
                free(devices[i].buffer);
            }
        }
        free(devices);
        devices = NULL;
    }
    if (service) {
        free(service);
    }
}

int main() {
    // Initialize the service
    DeviceService* deviceService = initialize_device_service();
    if (!deviceService) {
        printf("Failed to initialize device service\n");
        return 1;
    }

    // Create a new device
    Device keyboard = {
        .id = 1,
        .type = KEYBOARD_DEVICE,
        .status = 0,
        .power_mode = 0,
        .error_code = 0,
        .buffer_size = 0,
        .buffer = NULL
    };
    strcpy(keyboard.name, "PS/2 Keyboard");
    strcpy(keyboard.firmware_version, "1.0.0");

    // Using all 20 service calls
    printf("1. Initializing device...\n");
    deviceService->initialize_device(&keyboard);

    printf("2. Getting device...\n");
    Device* dev = deviceService->get_device_by_id(1);

    printf("3. Configuring device...\n");
    deviceService->configure_device(&keyboard);

    printf("4. Checking status...\n");
    deviceService->get_device_status(1);

    printf("5. Resetting device...\n");
    deviceService->reset_device(1);

    printf("6. Updating firmware...\n");
    deviceService->update_firmware(1, "1.0.1");

    printf("7. Setting power mode...\n");
    deviceService->set_power_mode(1, 2);

    printf("8. Getting error code...\n");
    deviceService->get_error_code(1);

    printf("9. Clearing error...\n");
    deviceService->clear_error(1);

    printf("10. Allocating buffer...\n");
    deviceService->allocate_buffer(1, 1024);

    printf("11. Writing to buffer...\n");
    char data[] = "Test data";
    deviceService->write_buffer(1, data, strlen(data));

    printf("12. Reading from buffer...\n");
    char readData[20];
    deviceService->read_buffer(1, readData, strlen(data));

    printf("13. Getting firmware version...\n");
    char* version = deviceService->get_firmware_version(1);

    printf("14. Running self test...\n");
    deviceService->run_self_test(1);

    printf("15. Getting power consumption...\n");
    deviceService->get_power_consumption(1);

    printf("16. Saving config...\n");
    deviceService->save_config(1);

    printf("17. Loading config...\n");
    deviceService->load_config(1);

    printf("18. Resetting config...\n");
    deviceService->reset_config(1);

    printf("19. Freeing buffer...\n");
    deviceService->free_buffer(1);

    printf("20. Shutting down device...\n");
    deviceService->shutdown_device(1);

    // Clean up
    cleanup_device_service(deviceService);
    printf("Service cleanup complete!\n");
    return 0;
} 