/* ESP32-CAM + Edge Impulse + OLED Waste Detection FINAL - FIXED VERSION */

#include <Smart_sep_inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "esp_camera.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* OLED */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* CAMERA MODEL */
#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#endif

#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 320
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3

static bool debug_nn = false;
static bool is_initialised = false;
uint8_t *snapshot_buf;

/* CAMERA CONFIG */
static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,
    .fb_count = 1,
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

/* FUNCTIONS */
bool ei_camera_init(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr);

/* OLED CENTER PRINT */
void displayCenter(String line1, String line2) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    
    int16_t x1, y1;
    uint16_t w, h;
    
    // Center first line
    display.getTextBounds(line1, 0, 0, &x1, &y1, &w, &h);
    int x = (SCREEN_WIDTH - w) / 2;
    display.setCursor(x, 10);
    display.println(line1);
    
    // Center second line
    display.getTextBounds(line2, 0, 0, &x1, &y1, &w, &h);
    x = (SCREEN_WIDTH - w) / 2;
    display.setCursor(x, 40);
    display.println(line2);
    
    display.display();
}

/* SETUP */
void setup() {
    Serial.begin(115200);
    while(!Serial);
    
    /* OLED */
    Wire.begin(14, 15);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED FAIL");
        while(true);
    }
    
    displayCenter("SYSTEM", "STARTING");
    
    /* CAMERA */
    if(ei_camera_init() == false) {
        Serial.println("Camera failed");
        displayCenter("CAMERA", "FAILED");
        while(true);
    } else {
        Serial.println("Camera ready");
    }
    
    delay(2000);
    displayCenter("READY", "PLACE WASTE");
}

/* LOOP */
void loop() {
    // Allocate buffer for snapshot
    snapshot_buf = (uint8_t*)malloc(EI_CAMERA_RAW_FRAME_BUFFER_COLS * 
                                    EI_CAMERA_RAW_FRAME_BUFFER_ROWS * 
                                    EI_CAMERA_FRAME_BYTE_SIZE);
    
    if(snapshot_buf == nullptr) {
        Serial.println("Buffer error");
        delay(1000);
        return;
    }
    
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_get_data;
    
    // Capture image
    if(ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH, 
                         EI_CLASSIFIER_INPUT_HEIGHT, 
                         snapshot_buf) == false) {
        Serial.println("Capture failed");
        free(snapshot_buf);
        delay(1000);
        return;
    }
    
    // Run classifier
    ei_impulse_result_t result = {0};
    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
    
    if(err != EI_IMPULSE_OK) {
        Serial.println("Classifier error");
        free(snapshot_buf);
        delay(1000);
        return;
    }
    
    bool object_found = false;
    float highest_accuracy = 0;
    String best_label = "";
    String best_display_label = "";
    
    #if EI_CLASSIFIER_OBJECT_DETECTION == 1
    // Find the highest confidence detection
    for(uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        
        if(bb.value == 0) continue;
        
        object_found = true;
        float accuracy = bb.value * 100;
        
        // Print all detections to Serial
        Serial.print("Detected: ");
        Serial.print(bb.label);
        Serial.print(" - Confidence: ");
        Serial.print(accuracy);
        Serial.println("%");
        
        // Track highest accuracy
        if(accuracy > highest_accuracy) {
            highest_accuracy = accuracy;
            best_label = String(bb.label);
            
            // Convert to display format
            if(best_label == "biodegradable" || best_label == "Biodegradable") {
                best_display_label = "NON-BIO";// BIO
            } else if(best_label == "nonbiodegradable" || best_label == "Nonbiodegradable" || best_label == "nonbio") {
                best_display_label = "BIO";// NONBIO
            } else {
                best_display_label = best_label.substring(0, 5);
            }
        }
    }
    
    // Update OLED based on highest confidence detection
    if(object_found) {
        if(highest_accuracy >= 70) {  // Using 70% threshold for better detection
            String accText = String(highest_accuracy, 1) + "%";
            displayCenter(best_display_label, accText);
            
            // Also print to Serial
            Serial.print(">>> DISPLAYING: ");
            Serial.print(best_display_label);
            Serial.print(" with ");
            Serial.print(highest_accuracy);
            Serial.println("% confidence");
        } else {
            displayCenter("LOW", "CONFIDENCE");
            Serial.println(">>> DISPLAYING: LOW CONFIDENCE");
        }
    } else {
        displayCenter("NO OBJECT", "DETECTED");
        Serial.println(">>> DISPLAYING: NO OBJECT DETECTED");
    }
    
    #endif
    
    free(snapshot_buf);
    delay(1500);  // Delay between captures for stability
}

/* CAMERA INIT */
bool ei_camera_init(void) {
    if(is_initialised) return true;
    
    esp_err_t err = esp_camera_init(&camera_config);
    if(err != ESP_OK) {
        Serial.println("Camera init failed");
        return false;
    }
    
    is_initialised = true;
    return true;
}

/* CAMERA CAPTURE */
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
    camera_fb_t *fb = esp_camera_fb_get();
    
    if(!fb) {
        Serial.println("Capture failed");
        return false;
    }
    
    bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, snapshot_buf);
    esp_camera_fb_return(fb);
    
    if(!converted) {
        Serial.println("Conversion failed");
        return false;
    }
    
    return true;
}

/* GET IMAGE DATA */
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {
    size_t pixel_ix = offset * 3;
    size_t pixels_left = length;
    size_t out_ptr_ix = 0;
    
    while(pixels_left != 0) {
        out_ptr[out_ptr_ix] = (snapshot_buf[pixel_ix + 2] << 16) +
                               (snapshot_buf[pixel_ix + 1] << 8) +
                               snapshot_buf[pixel_ix];
        
        out_ptr_ix++;
        pixel_ix += 3;
        pixels_left--;
    }
    
    return 0;
}