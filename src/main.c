#include "pebble.h"
  
// PERSISTENT STORAGE
#define SETTINGS_KEY 0
  
typedef struct persist{
  uint8_t c_1;
  uint8_t c_2;
  uint8_t c_3;
  uint8_t c_4;
  uint8_t c_5;
  uint8_t c_6;
  bool set;
}__attribute__((__packed__)) persist;

persist settings = {
  .c_1 = 5,
  .c_2 = 5,
  .c_3 = 5,
  .c_4 = 5,
  .c_5 = 5,
  .c_6 = 6,
  .set = false,
};


// Just some things all scattered about in no particular order
int valueRead, valueWritten;

static Window *s_main_window, *s_set_window;
static TextLayer *s_date_layer, *s_time_layer, *s_action_title, *s_action_label;
static Layer *s_line_layer;
int unlock = 0;
int reset = 0;
int length = 6;
static char s_length_buffer[] = "X";

// Handle the actual watch face part of this whole deal
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  static char s_time_text[] = "00:00";
  static char s_date_text[] = "Xxxxxxxxx 00";

  strftime(s_date_text, sizeof(s_date_text), "%B %e", tick_time);
  text_layer_set_text(s_date_layer, s_date_text);

  char *time_format;
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  strftime(s_time_text, sizeof(s_time_text), time_format, tick_time);

  if (!clock_is_24h_style() && (s_time_text[0] == '0')) {
    memmove(s_time_text, &s_time_text[1], sizeof(s_time_text) - 1);
  }
  text_layer_set_text(s_time_layer, s_time_text);
}

// The added bonus of a battery indicator
static void update_battery(Layer *layer, GContext *ctx) {
  int bat = battery_state_service_peek().charge_percent;
  GRect line_frame = GRect(8, 97, (bat * 1.28), 2);
  s_line_layer = layer_create(line_frame);
  
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(s_line_layer), 0, GCornerNone);
}

// Handle the clicks on the main window for entering the passcode
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (unlock == 0) {
    if (settings.c_1 == 1) {
      unlock = 1;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 1) {
    if (settings.c_2 == 1) {
      unlock = 2;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 2) {
    if (settings.c_3 == 1) {
      unlock = 3;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 3) {
    if (settings.c_4 == 1) {
      unlock = 4;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 4) {
    if (settings.c_3 == 1) {
      unlock = 5;
    }
  }
  else if (unlock == 5) {
    if (settings.c_3 == 1) {
      unlock = 0;
      
      window_stack_remove(s_main_window, true);
    }
    else {
      unlock = 0;
    }
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (settings.set == false) {
    window_stack_push(s_set_window, true);
  }
  if (unlock == 0) {
    if (settings.c_1 == 2) {
      unlock = 1;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 1) {
    if (settings.c_2 == 2) {
      unlock = 2;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 2) {
    if (settings.c_3 == 2) {
      unlock = 3;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 3) {
    if (settings.c_4 == 2) {
      unlock = 4;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 4) {
    if (settings.c_3 == 2) {
      unlock = 5;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 5) {
    if (settings.c_3 == 2) {
      unlock = 0;
      window_stack_remove(s_main_window, true);
    }
    else {
      unlock = 0;
    }
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (unlock == 0) {
    if (settings.c_1 == 3) {
      unlock = 1;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 1) {
    if (settings.c_2 == 3) {
      unlock = 2;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 2) {
    if (settings.c_3 == 3) {
      unlock = 3;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 3) {
    if (settings.c_4 == 3) {
      unlock = 4;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 4) {
    if (settings.c_3 == 3) {
      unlock = 5;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 5) {
    if (settings.c_3 == 3) {
      unlock = 0;
      window_stack_remove(s_main_window, true);
    }
    else {
      unlock = 0;
    }
  }
}

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (unlock == 0) {
    if (settings.c_1 == 4) {
      unlock = 1;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 1) {
    if (settings.c_2 == 4) {
      unlock = 2;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 2) {
    if (settings.c_3 == 4) {
      unlock = 3;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 3) {
    if (settings.c_4 == 4) {
      unlock = 4;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 4) {
    if (settings.c_3 == 4) {
      unlock = 5;
    }
    else {
      unlock = 0;
    }
  }
  else if (unlock == 5) {
    if (settings.c_3 == 4) {
      unlock = 0;
      window_stack_remove(s_main_window, true);
    }
    else {
      unlock = 0;
    }
  }
}
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  settings.c_1 = 5;
  settings.c_2 = 5;
  settings.c_3 = 5;
  settings.c_4 = 5;
  settings.c_5 = 5;
  settings.c_6 = 5;
  settings.set = false;
  unlock = 0;
  length = 6;
  window_stack_push(s_set_window, true);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 5000, select_long_click_handler, NULL);
}

// Handle the clicks on the window that sets the passcode
static void set_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (settings.c_1 == 5) {
    settings.c_1 = 1;
  }
  else if (settings.c_2 == 5) {
    settings.c_2 = 1;
  }
  else if (settings.c_3 == 5) {
    settings.c_3 = 1;
  }
  else if (settings.c_4 == 5) {
    settings.c_4 = 1;
  }
  else if (settings.c_5 == 5) {
    settings.c_5 = 1;
  }
  else if (settings.c_6 == 5) {
    settings.c_6 = 1;
    
    window_stack_remove(s_set_window, true);
    settings.set = true;
  }
  length -= 1;
  
  snprintf(s_length_buffer, sizeof(s_length_buffer),"%d",length);
  text_layer_set_text(s_action_label, s_length_buffer);
}

static void set_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (settings.c_1 == 5) {
    settings.c_1 = 2;
  }
  else if (settings.c_2 == 5) {
    settings.c_2 = 2;
  }
  else if (settings.c_3 == 5) {
    settings.c_3 = 2;
  }
  else if (settings.c_4 == 5) {
    settings.c_4 = 2;
  }
  else if (settings.c_5 == 5) {
    settings.c_5 = 2;
  }
  else if (settings.c_6 == 5) {
    settings.c_6 = 2;

    window_stack_remove(s_set_window, true);
    settings.set = true;
  }
  length -= 1;
  
  snprintf(s_length_buffer, sizeof(s_length_buffer),"%d",length);
  text_layer_set_text(s_action_label, s_length_buffer);
}

static void set_down_click_handler(ClickRecognizerRef recognizer, void *context) {
if (settings.c_1 == 5) {
    settings.c_1 = 3;
  }
  else if (settings.c_2 == 5) {
    settings.c_2 = 3;
  }
  else if (settings.c_3 == 5) {
    settings.c_3 = 3;
  }
  else if (settings.c_4 == 5) {
    settings.c_4 = 3;
  }
  else if (settings.c_5 == 5) {
    settings.c_5 = 3;
  }
  else if (settings.c_6 == 5) {
    settings.c_6 = 3;
    
    window_stack_remove(s_set_window, true);
    settings.set = true;
  }
  length -= 1;
  
  snprintf(s_length_buffer, sizeof(s_length_buffer),"%d",length);
  text_layer_set_text(s_action_label, s_length_buffer);
}

static void set_back_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (settings.c_1 == 5) {
    settings.c_1 = 4;
  }
  else if (settings.c_2 == 5) {
    settings.c_2 = 4;
  }
  else if (settings.c_3 == 5) {
    settings.c_3 = 4;
  }
  else if (settings.c_4 == 5) {
    settings.c_4 = 4;
  }
  else if (settings.c_5 == 5) {
    settings.c_5 = 4;
  }
  else if (settings.c_6 == 5) {
    settings.c_6 = 4;
    
    window_stack_remove(s_set_window, true);
    settings.set = true;
  }
  length -= 1;
  
  snprintf(s_length_buffer, sizeof(s_length_buffer),"%d",length);
  text_layer_set_text(s_action_label, s_length_buffer);
}

static void set_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, set_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, set_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, set_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, set_back_click_handler);
}

// (Un)Load the windows
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  s_date_layer = text_layer_create(GRect(8, 68, 136, 100));
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  s_time_layer = text_layer_create(GRect(7, 92, 137, 76));
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  GRect line_frame = GRect(8, 97, 139, 2);
  s_line_layer = layer_create(line_frame);
  layer_set_update_proc(s_line_layer, update_battery);
  layer_add_child(window_layer, s_line_layer);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_time_layer);

  layer_destroy(s_line_layer);
}

static void set_window_load(Window *window) {
  
  // Creates window_layer as root and sets its bounds
  Layer *window_layer = window_get_root_layer(window);
 
  s_action_label = text_layer_create(GRect(0,55,144,60));
  s_action_title = text_layer_create(GRect(0,15,144,60));
  
  text_layer_set_background_color(s_action_label, GColorClear);
  text_layer_set_text_color(s_action_label, GColorWhite);
  
  text_layer_set_background_color(s_action_title, GColorClear);
  text_layer_set_text_color(s_action_title, GColorWhite);
  
  text_layer_set_font(s_action_label, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_font(s_action_title, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  
  snprintf(s_length_buffer, sizeof(s_length_buffer),"%d",length);
  
  text_layer_set_text(s_action_label, s_length_buffer);
  text_layer_set_text(s_action_title, "Enter Pattern");
  
  text_layer_set_text_alignment(s_action_label, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_action_title, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(s_action_label));
  layer_add_child(window_layer, text_layer_get_layer(s_action_title));
    
}

static void set_window_unload(Window *window) {
  text_layer_destroy(s_action_label);
  text_layer_destroy(s_action_title);
}

// Initialization
static void init() {
  s_main_window = window_create();
  s_set_window = window_create();
  
// Remove the statusbar on SDK 2
#ifdef PBL_SDK_2
  window_set_fullscreen(s_main_window, true);
  window_set_fullscreen(s_set_window, true);
#endif
  
  // Read the persistent storage. Probably incorrectly
  valueRead = persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  // Read the persistent storage better, meaning after that read fails, just go ahead and fix it
  if (settings.c_1 == 0) {
    settings.c_1 = 5;
    settings.c_2 = 5;
    settings.c_3 = 5;
    settings.c_4 = 5;
    settings.c_5 = 5;
    settings.c_6 = 5;
    settings.set = false;
  }
  
  window_set_background_color(s_main_window, GColorBlack);
  window_set_background_color(s_set_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_window_handlers(s_set_window, (WindowHandlers) {
    .load = set_window_load,
    .unload = set_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_click_config_provider(s_set_window, set_click_config_provider);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  handle_minute_tick(t, MINUTE_UNIT);
}

static void deinit() {
  
  // Write to persistent storage, correctly.
  valueWritten = persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  
  window_destroy(s_main_window);
  window_destroy(s_set_window);

  tick_timer_service_unsubscribe();
}

int main() {
  init();
  app_event_loop();
  deinit();
}