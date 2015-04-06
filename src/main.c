#include <pebble.h>
#include "main.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

//declare custom font
static GFont *s_time_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time(){
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char bufferTime[] = "00:00";
	static char bufferDate[] = "00 - 00 - 0000";
  
  if(clock_is_24h_style()){
    strftime(bufferTime, sizeof("00:00"), "%H:%M", tick_time);
  }else{
    strftime(bufferTime, sizeof("00:00"), "%I:%M",tick_time);
  }
	
	strftime(bufferDate, sizeof("00 - 00 - 0000"), "%d - %m - %y", tick_time);
  
  text_layer_set_text(s_time_layer, bufferTime);
	text_layer_set_text(s_date_layer, bufferDate);
  
}

static void main_window_load(Window *window){
	
	//create GBitmap and Bitmaplayer
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
	
  //create customfont
	s_time_font = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
	
  s_time_layer = text_layer_create(GRect(5, 70, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
	//setting text layer font
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
	
	s_date_layer = text_layer_create(GRect(5, 120, 139, 50));
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer,  s_time_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
	
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window){
  
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
	text_layer_destroy(s_date_layer);
	
	gbitmap_destroy(s_background_bitmap);
	bitmap_layer_destroy(s_background_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void init(){
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  window_stack_push(s_main_window, true);
  update_time();
  
}

static void deinit(){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}