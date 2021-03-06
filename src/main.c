#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_second_layer;
static TextLayer *s_battery_layer;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  static char buffer2[] = "00";
  
  if(clock_is_24h_style() == true){
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  }
  else{
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  strftime(buffer2, sizeof("00"), "%S", tick_time);
  
  //if()
  
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_second_layer, buffer2);
}

static void main_window_load(Window *window){
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WATCH_BACK);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_STARCRAFT_38));
  
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  s_second_layer = text_layer_create(GRect(50, 106, 44, 21));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);

  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_font(s_second_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_second_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_second_layer));
}

static void main_window_unload(Window *window){
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_second_layer);
  fonts_unload_custom_font(s_time_font);
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
}
  
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init(){
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(){
  window_destroy(s_main_window);
}


int main(void){
  init();
  app_event_loop();
  deinit();
}