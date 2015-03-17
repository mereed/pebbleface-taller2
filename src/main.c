#include <pebble.h>
#include "packbits.h"

GColor background_color = GColorBlack;

static Window *window;
static Layer *window_layer;

static uint8_t batteryPercent;

static AppSync sync;
static uint8_t sync_buffer[256];

static int blink;
static int invert;
static int bluetoothvibe;
static int hourlyvibe;
static int battbar;
//static int germanlang;
//static int russianlang;
//static int frenchlang;
//static int italianlang;
//static int chlang;

static bool appStarted = false;

enum {
  BLINK_KEY = 0x0,
  INVERT_KEY = 0x1,
  BLUETOOTHVIBE_KEY = 0x2,
  HOURLYVIBE_KEY = 0x3,
  CHINESE_KEY = 0x4,
  BATTBAR_KEY = 0x5,
  GERMAN_KEY = 0x6,
  RUSSIAN_KEY = 0x7,
  FRENCH_KEY = 0x8,
  ITALIAN_KEY = 0x9
};

static GBitmap *separator_image;
static BitmapLayer *separator_layer;

static GBitmap *bluetooth_image;
static BitmapLayer *bluetooth_layer;

static GBitmap *x;
static BitmapLayer *y;
static BitmapLayer *z;

static GBitmap *time_format_image;
static BitmapLayer *time_format_layer;

static GBitmap *day_name_image;
static BitmapLayer *day_name_layer;

static uint8_t lang =0; //active language
static const uint8_t day_x_pos[]= {76,73,72,72,73,73}; //same order as below
static const uint8_t month_x_pos[]={5, 2, 2, 3, 3, 3}; //same order as below

  
const int DAY_NAME_IMAGE_RESOURCE_IDS[][7] = {
 {RESOURCE_ID_IMAGE_DAY_NAME_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_SAT},
  //GERMAN
 {RESOURCE_ID_IMAGE_DAY_NAME_SONNTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_MONTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_DIENSTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_MITTWOCH,
  RESOURCE_ID_IMAGE_DAY_NAME_DONNERSTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_FREITAG,
  RESOURCE_ID_IMAGE_DAY_NAME_SAMSTAG},
  //RUSSIAN
 {RESOURCE_ID_IMAGE_DAY_NAME_RUS_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_SAT},
  //FRENCH
 {RESOURCE_ID_IMAGE_DAY_NAME_FR_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_SAT},
  //ITALIAN
 {RESOURCE_ID_IMAGE_DAY_NAME_IT_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_SAT},
  //CHINESE
 {RESOURCE_ID_IMAGE_DAY_NAME_CH_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_SAT},
};

#define TOTAL_DATE_DIGITS 2	
static GBitmap *date_digits_images[TOTAL_DATE_DIGITS];
static BitmapLayer *date_digits_layers[TOTAL_DATE_DIGITS];

const int DATENUM_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DATENUM_0,
  RESOURCE_ID_IMAGE_DATENUM_1,
  RESOURCE_ID_IMAGE_DATENUM_2,
  RESOURCE_ID_IMAGE_DATENUM_3,
  RESOURCE_ID_IMAGE_DATENUM_4,
  RESOURCE_ID_IMAGE_DATENUM_5,
  RESOURCE_ID_IMAGE_DATENUM_6,
  RESOURCE_ID_IMAGE_DATENUM_7,
  RESOURCE_ID_IMAGE_DATENUM_8,
  RESOURCE_ID_IMAGE_DATENUM_9
};

#define TOTAL_TIME_DIGITS 4
static GBitmap *time_digits_images[TOTAL_TIME_DIGITS];
static BitmapLayer *time_digits_layers[TOTAL_TIME_DIGITS];

const int BIG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_NUM_0,
  RESOURCE_ID_IMAGE_NUM_1,
  RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3,
  RESOURCE_ID_IMAGE_NUM_4,
  RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6,
  RESOURCE_ID_IMAGE_NUM_7,
  RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9
};

static GBitmap *month_image;
static BitmapLayer *month_layer;

const int MONTH_IMAGE_RESOURCE_IDS[][12] = {
 {RESOURCE_ID_IMAGE_JAN,
  RESOURCE_ID_IMAGE_FEB,
  RESOURCE_ID_IMAGE_MAR,
  RESOURCE_ID_IMAGE_APR,
  RESOURCE_ID_IMAGE_MAY,
  RESOURCE_ID_IMAGE_JUN,
  RESOURCE_ID_IMAGE_JUL,
  RESOURCE_ID_IMAGE_AUG,
  RESOURCE_ID_IMAGE_SEP,
  RESOURCE_ID_IMAGE_OCT,
  RESOURCE_ID_IMAGE_NOV,
  RESOURCE_ID_IMAGE_DEC},
  //German
 {RESOURCE_ID_IMAGE_GJAN,
  RESOURCE_ID_IMAGE_GFEB,
  RESOURCE_ID_IMAGE_GMAR,
  RESOURCE_ID_IMAGE_GAPR,
  RESOURCE_ID_IMAGE_GMAY,
  RESOURCE_ID_IMAGE_GJUN,
  RESOURCE_ID_IMAGE_GJUL,
  RESOURCE_ID_IMAGE_GAUG,
  RESOURCE_ID_IMAGE_GSEP,
  RESOURCE_ID_IMAGE_GOCT,
  RESOURCE_ID_IMAGE_GNOV,
  RESOURCE_ID_IMAGE_GDEC},
  //Russian
 {RESOURCE_ID_IMAGE_RJAN,
  RESOURCE_ID_IMAGE_RFEB,
  RESOURCE_ID_IMAGE_RMAR,
  RESOURCE_ID_IMAGE_RAPR,
  RESOURCE_ID_IMAGE_RMAY,
  RESOURCE_ID_IMAGE_RJUN,
  RESOURCE_ID_IMAGE_RJUL,
  RESOURCE_ID_IMAGE_RAUG,
  RESOURCE_ID_IMAGE_RSEP,
  RESOURCE_ID_IMAGE_ROCT,
  RESOURCE_ID_IMAGE_RNOV,
  RESOURCE_ID_IMAGE_RDEC},
  //French
 {RESOURCE_ID_IMAGE_FJAN,
  RESOURCE_ID_IMAGE_FFEB,
  RESOURCE_ID_IMAGE_FMAR,
  RESOURCE_ID_IMAGE_FAPR,
  RESOURCE_ID_IMAGE_FMAY,
  RESOURCE_ID_IMAGE_FJUN,
  RESOURCE_ID_IMAGE_FJUL,
  RESOURCE_ID_IMAGE_FAUG,
  RESOURCE_ID_IMAGE_FSEP,
  RESOURCE_ID_IMAGE_FOCT,
  RESOURCE_ID_IMAGE_FNOV,
  RESOURCE_ID_IMAGE_FDEC},
  //Italian
 {RESOURCE_ID_IMAGE_IJAN,
  RESOURCE_ID_IMAGE_IFEB,
  RESOURCE_ID_IMAGE_IMAR,
  RESOURCE_ID_IMAGE_IAPR,
  RESOURCE_ID_IMAGE_IMAY,
  RESOURCE_ID_IMAGE_IJUN,
  RESOURCE_ID_IMAGE_IJUL,
  RESOURCE_ID_IMAGE_IAUG,
  RESOURCE_ID_IMAGE_ISEP,
  RESOURCE_ID_IMAGE_IOCT,
  RESOURCE_ID_IMAGE_INOV,
  RESOURCE_ID_IMAGE_IDEC},
  //China
 {RESOURCE_ID_IMAGE_CJAN,
  RESOURCE_ID_IMAGE_CFEB,
  RESOURCE_ID_IMAGE_CMAR,
  RESOURCE_ID_IMAGE_CAPR,
  RESOURCE_ID_IMAGE_CMAY,
  RESOURCE_ID_IMAGE_CJUN,
  RESOURCE_ID_IMAGE_CJUL,
  RESOURCE_ID_IMAGE_CAUG,
  RESOURCE_ID_IMAGE_CSEP,
  RESOURCE_ID_IMAGE_COCT,
  RESOURCE_ID_IMAGE_CNOV,
  RESOURCE_ID_IMAGE_CDEC}
  
};


#define TOTAL_BATTERY_PERCENT_DIGITS 3
static GBitmap *battery_percent_image[TOTAL_BATTERY_PERCENT_DIGITS];
static BitmapLayer *battery_percent_layers[TOTAL_BATTERY_PERCENT_DIGITS];

const int TINY_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_TINY_0,
  RESOURCE_ID_IMAGE_TINY_1,
  RESOURCE_ID_IMAGE_TINY_2,
  RESOURCE_ID_IMAGE_TINY_3,
  RESOURCE_ID_IMAGE_TINY_4,
  RESOURCE_ID_IMAGE_TINY_5,
  RESOURCE_ID_IMAGE_TINY_6,
  RESOURCE_ID_IMAGE_TINY_7,
  RESOURCE_ID_IMAGE_TINY_8,
  RESOURCE_ID_IMAGE_TINY_9,
  RESOURCE_ID_IMAGE_TINY_PERCENT
};

BitmapLayer *layer_batt_img;
GBitmap *batt_100;
GBitmap *batt_90;
GBitmap *batt_80;
GBitmap *batt_70;
GBitmap *batt_60;
GBitmap *batt_50;
GBitmap *batt_40;
GBitmap *batt_30;
GBitmap *batt_20;
GBitmap *batt_10;
GBitmap *batt_charge;

InverterLayer *inverter_layer = NULL;


void set_invert_color(bool invert) {
  if (invert && inverter_layer == NULL) {
    // Add inverter layer
    Layer *window_layer = window_get_root_layer(window);

    inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
    layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
  } else if (!invert && inverter_layer != NULL) {
    // Remove Inverter layer
    layer_remove_from_parent(inverter_layer_get_layer(inverter_layer));
    inverter_layer_destroy(inverter_layer);
    inverter_layer = NULL;
  }
  // No action required
}

void hidebatt (bool battbar) {
	
	  if (battbar) {
		  	  	   layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true);
	  } else {
		  	  	   layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), false);
	  }
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed);

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case BLINK_KEY:
      blink = new_tuple->value->uint8;
	  persist_write_bool(INVERT_KEY, blink);
      tick_timer_service_unsubscribe();
      if(blink) {
        tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
      }
      else {
        tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
	  }
	  
      break;
     case INVERT_KEY:
      invert = new_tuple->value->uint8 != 0;
	  persist_write_bool(INVERT_KEY, invert);
      set_invert_color(invert);
      break;
	  
	case BLUETOOTHVIBE_KEY:
      bluetoothvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
      break;      
	  
    case HOURLYVIBE_KEY:
      hourlyvibe = new_tuple->value->uint8 != 0;
	  persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);	  
      break;	
	  
    case BATTBAR_KEY:
      battbar = new_tuple->value->uint8 != 0;
	  persist_write_bool(BATTBAR_KEY, battbar);
	  hidebatt (battbar);
      break;
	  
	/*case ITALIAN_KEY:
      italianlang = new_tuple->value->uint8 != 0;
	  persist_write_bool(ITALIAN_KEY, italianlang);	  
	
		if (italianlang) {
	  	   layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), false);
	  	   layer_set_hidden(bitmap_layer_get_layer(imonth_layer), false);
        }  else {
	  		layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
        }
      break;
	  
	case FRENCH_KEY:
      frenchlang = new_tuple->value->uint8 != 0;
	  persist_write_bool(FRENCH_KEY, frenchlang);	  
	
		if (frenchlang) {
	  	   layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), false);
	  	   layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), false);
        }  else {
	  		layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
        }
      break;
	  
	case RUSSIAN_KEY:
      russianlang = new_tuple->value->uint8 != 0;
	  persist_write_bool(RUSSIAN_KEY, russianlang);	  
	
		if (russianlang) {
	  	   layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), false);
	  	   layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), false);
        }  else {
	  		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
        }
      break;
	  
	case GERMAN_KEY:
      germanlang = new_tuple->value->uint8 != 0;
	  persist_write_bool(GERMAN_KEY, germanlang);	  
	
		if (germanlang) {
		   layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), false);
		   layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), false);
        }  else {
            layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
        }
      break;
	  
	case CHINESE_KEY:
      chlang = new_tuple->value->uint8 != 0;
	  persist_write_bool(CHINESE_KEY, chlang);	  
	
		if (chlang) {
		   layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), false);
		   layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), false);
        }  else {
            layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
        }
      break;*/
  }
}

static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) {
  GBitmap *old_image = *bmp_image;
  *bmp_image = gbitmap_create_from_pbt(resource_id);
  GRect frame = (GRect) {
    .origin = origin,
    .size = (*bmp_image)->bounds.size
  };
  bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
  layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);
  gbitmap_destroy(old_image);
}


static void update_battery(BatteryChargeState charge_state) {

  batteryPercent = charge_state.charge_percent;

	if (charge_state.is_charging) {
        bitmap_layer_set_bitmap(layer_batt_img, batt_charge);		
    } else {
        if (charge_state.charge_percent <= 10) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_10);
        } else if (charge_state.charge_percent <= 20) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_20);
        } else if (charge_state.charge_percent <= 30) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_30);
		} else if (charge_state.charge_percent <= 40) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_40);
		} else if (charge_state.charge_percent <= 50) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_50);
    	} else if (charge_state.charge_percent <= 60) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_60);	
        } else if (charge_state.charge_percent <= 70) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_70);
		} else if (charge_state.charge_percent <= 80) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_80);
		} else if (charge_state.charge_percent <= 90) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_90);
		} else if (charge_state.charge_percent <= 100) {
            bitmap_layer_set_bitmap(layer_batt_img, batt_100);					
		}
	
	
	if(batteryPercent>=99) {	
	layer_set_hidden(bitmap_layer_get_layer(y), false);

    for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
      layer_set_hidden(bitmap_layer_get_layer(battery_percent_layers[i]), true);
    }
    return;
  }
  layer_set_hidden(bitmap_layer_get_layer(y), true);
	
 for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
    layer_set_hidden(bitmap_layer_get_layer(battery_percent_layers[i]), false);
  }  
  set_container_image(&battery_percent_image[0], battery_percent_layers[0], TINY_IMAGE_RESOURCE_IDS[charge_state.charge_percent/10], GPoint(5, 131));
  set_container_image(&battery_percent_image[1], battery_percent_layers[1], TINY_IMAGE_RESOURCE_IDS[charge_state.charge_percent%10], GPoint(5, 139));
  set_container_image(&battery_percent_image[2], battery_percent_layers[2], TINY_IMAGE_RESOURCE_IDS[10], GPoint(5, 147));
}
}


static void toggle_bluetooth_icon(bool connected) {
  if(appStarted && !connected && bluetoothvibe) {
    //vibe!
    vibes_long_pulse();
  }
  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), connected);
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}


unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
}

static void update_days(struct tm *tick_time) {
  set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[lang][tick_time->tm_wday], GPoint(day_x_pos[lang], 16));

  set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(75,139));
  set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(75, 147));
}

//DRAW MONTH
	
static void update_months(struct tm *tick_time) {	
  set_container_image(&month_image, month_layer, MONTH_IMAGE_RESOURCE_IDS[lang][tick_time->tm_mon], GPoint(month_x_pos[lang], 16));

}

static void update_hours(struct tm *tick_time) {

  if(appStarted && hourlyvibe) {
    //vibe!
    vibes_short_pulse();
  }
  
  unsigned short display_hour = get_display_hour(tick_time->tm_hour);

  set_container_image(&time_digits_images[0], time_digits_layers[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(22, 16));
  set_container_image(&time_digits_images[1], time_digits_layers[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(46, 16));

  if (!clock_is_24h_style()) {
    
    
    if (display_hour/10 == 0) {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), true);
    }
    else {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), false);
    }

  }
}
static void update_minutes(struct tm *tick_time) {
  set_container_image(&time_digits_images[2], time_digits_layers[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min/10], GPoint(96, 16));
  set_container_image(&time_digits_images[3], time_digits_layers[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min%10], GPoint(120, 16));
}
static void update_seconds(struct tm *tick_time) {
  if(blink) {
    layer_set_hidden(bitmap_layer_get_layer(separator_layer), tick_time->tm_sec%2);
  }
  else {
    if(layer_get_hidden(bitmap_layer_get_layer(separator_layer))) {
      layer_set_hidden(bitmap_layer_get_layer(separator_layer), false);
    }
  }
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MONTH_UNIT) {
    update_months(tick_time);
  }
	if (units_changed & DAY_UNIT) {
    update_days(tick_time);
  }
  if (units_changed & HOUR_UNIT) {
    update_hours(tick_time);
  }
  if (units_changed & MINUTE_UNIT) {
    update_minutes(tick_time);
      // testing languages changing every minute
      lang=(lang+1)%6;
      update_months(tick_time);
      update_days(tick_time);
  }	
  if (units_changed & SECOND_UNIT) {
    update_seconds(tick_time);
  }		
}

void set_style(void) {
    background_color  = GColorBlack;
    window_set_background_color(window, background_color);
}

static void init(void) {
  memset(&time_digits_layers, 0, sizeof(time_digits_layers));
  memset(&time_digits_images, 0, sizeof(time_digits_images));
  memset(&date_digits_layers, 0, sizeof(date_digits_layers));
  memset(&date_digits_images, 0, sizeof(date_digits_images));
  memset(&battery_percent_layers, 0, sizeof(battery_percent_layers));
  memset(&battery_percent_image, 0, sizeof(battery_percent_image));

  const int inbound_size = 256;
  const int outbound_size = 256;
  app_message_open(inbound_size, outbound_size);  

  window = window_create();
  if (window == NULL) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "OOM: couldn't allocate window");
      return;
  }
	
set_style();

  window_stack_push(window, true /* Animated */);
  window_layer = window_get_root_layer(window);
	
  batt_100   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_090_100);
  batt_90   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_080_090);
  batt_80   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_070_080);
  batt_70   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_060_070);
  batt_60   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_050_060);
  batt_50   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_040_050);
  batt_40   = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_030_040);
  batt_30    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_020_030);
  batt_20    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_010_020);
  batt_10    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_000_010);
  batt_charge = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_CHARGING);
	
  layer_batt_img  = bitmap_layer_create(GRect(7, 1, 130, 5));
  bitmap_layer_set_bitmap(layer_batt_img, batt_100);
  layer_add_child(window_layer, bitmap_layer_get_layer(layer_batt_img));	
	
  separator_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEPARATOR);
  GRect frame = (GRect) {
    .origin = { .x = 75, .y = 130 },
    .size = separator_image->bounds.size
  };
  separator_layer = bitmap_layer_create(frame);
  bitmap_layer_set_bitmap(separator_layer, separator_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(separator_layer));   

  x = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY);
  GRect frame4 = (GRect) {
    .origin = { .x = 5, .y = 127 },
    .size = x->bounds.size
  };
  z = bitmap_layer_create(frame4);
  y = bitmap_layer_create(frame4);
  bitmap_layer_set_bitmap(y, x);
	
  layer_add_child(window_layer, bitmap_layer_get_layer(y));
  layer_add_child(window_layer, bitmap_layer_get_layer(z));
  
  // Create time and date layers
  GRect dummy_frame = { {0, 0}, {0, 0} };
	
   day_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(day_name_layer));	
   month_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(month_layer));
	
   /*gday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(gday_name_layer));
   gmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(gmonth_layer));
	
   rday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(rday_name_layer));	
   rmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(rmonth_layer));

   fday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(fday_name_layer));	
   fmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(fmonth_layer));

   iday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(iday_name_layer));	
   imonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(imonth_layer));
	
   cday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(cday_name_layer));	
   cmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(cmonth_layer));*/
	
  for (int i = 0; i < TOTAL_TIME_DIGITS; ++i) {
    time_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(time_digits_layers[i]));
  }
	
  for (int i = 0; i < TOTAL_DATE_DIGITS; ++i) {
    date_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(date_digits_layers[i]));
  }
  for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; ++i) {
    battery_percent_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(battery_percent_layers[i]));
  }
  
  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
  GRect frame3 = (GRect) {
    .origin = { .x = 4, .y = 127 },
    .size = bluetooth_image->bounds.size
  };
  bluetooth_layer = bitmap_layer_create(frame3);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));
	
  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery(battery_state_service_peek());

  Tuplet initial_values[] = {
    TupletInteger(BLINK_KEY, persist_read_bool(BLINK_KEY)),
    TupletInteger(INVERT_KEY, persist_read_bool(INVERT_KEY)),
    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
    TupletInteger(BATTBAR_KEY, persist_read_bool(BATTBAR_KEY)),
    TupletInteger(GERMAN_KEY, persist_read_bool(GERMAN_KEY)),
    TupletInteger(RUSSIAN_KEY, persist_read_bool(RUSSIAN_KEY)),
    TupletInteger(FRENCH_KEY, persist_read_bool(FRENCH_KEY)),
    TupletInteger(ITALIAN_KEY, persist_read_bool(ITALIAN_KEY)),
	TupletInteger(CHINESE_KEY, persist_read_bool(CHINESE_KEY)),

  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, NULL, NULL);
   
  appStarted = true;
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  handle_tick(tick_time, MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT + SECOND_UNIT);
	
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&update_battery);

}

static void deinit(void) {
  app_sync_deinit(&sync);
  
  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();

  layer_remove_from_parent(bitmap_layer_get_layer(separator_layer));
  bitmap_layer_destroy(separator_layer);
  gbitmap_destroy(separator_image);
  
  layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
  bitmap_layer_destroy(bluetooth_layer);
  gbitmap_destroy(bluetooth_image);
  
  layer_remove_from_parent(bitmap_layer_get_layer(z));
  bitmap_layer_destroy(z);
  gbitmap_destroy(x);
  
  layer_remove_from_parent(bitmap_layer_get_layer(y));
  bitmap_layer_destroy(y);

  layer_remove_from_parent(bitmap_layer_get_layer(time_format_layer));
  bitmap_layer_destroy(time_format_layer);
  gbitmap_destroy(time_format_image);

  layer_remove_from_parent(bitmap_layer_get_layer(day_name_layer));
  bitmap_layer_destroy(day_name_layer);
  gbitmap_destroy(day_name_image);
	
  /*layer_remove_from_parent(bitmap_layer_get_layer(gday_name_layer));
  bitmap_layer_destroy(gday_name_layer);
  gbitmap_destroy(gday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(rday_name_layer));
  bitmap_layer_destroy(rday_name_layer);
  gbitmap_destroy(rday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(fday_name_layer));
  bitmap_layer_destroy(fday_name_layer);
  gbitmap_destroy(fday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(iday_name_layer));
  bitmap_layer_destroy(iday_name_layer);
  gbitmap_destroy(iday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(cday_name_layer));
  bitmap_layer_destroy(cday_name_layer);
  gbitmap_destroy(cday_name_image);*/
	
  layer_remove_from_parent(bitmap_layer_get_layer(month_layer));
  bitmap_layer_destroy(month_layer);
  gbitmap_destroy(month_image);  
	
  /*layer_remove_from_parent(bitmap_layer_get_layer(gmonth_layer));
  bitmap_layer_destroy(gmonth_layer);
  gbitmap_destroy(gmonth_image);  
	
  layer_remove_from_parent(bitmap_layer_get_layer(rmonth_layer));
  bitmap_layer_destroy(rmonth_layer);
  gbitmap_destroy(rmonth_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(fmonth_layer));
  bitmap_layer_destroy(fmonth_layer);
  gbitmap_destroy(fmonth_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(imonth_layer));
  bitmap_layer_destroy(imonth_layer);
  gbitmap_destroy(imonth_image);
		
  layer_remove_from_parent(bitmap_layer_get_layer(cmonth_layer));
  bitmap_layer_destroy(cmonth_layer);
  gbitmap_destroy(cmonth_image);*/
	
  	
  layer_remove_from_parent(bitmap_layer_get_layer(layer_batt_img));
  bitmap_layer_destroy(layer_batt_img);	
  gbitmap_destroy(batt_100);
  gbitmap_destroy(batt_90);
  gbitmap_destroy(batt_80);
  gbitmap_destroy(batt_70);
  gbitmap_destroy(batt_60);
  gbitmap_destroy(batt_50);
  gbitmap_destroy(batt_40);
  gbitmap_destroy(batt_30);
  gbitmap_destroy(batt_20);
  gbitmap_destroy(batt_10);
  gbitmap_destroy(batt_charge);	
	

	
  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(date_digits_layers[i]));
    gbitmap_destroy(date_digits_images[i]);
    bitmap_layer_destroy(date_digits_layers[i]);
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(time_digits_layers[i]));
    gbitmap_destroy(time_digits_images[i]);
    bitmap_layer_destroy(time_digits_layers[i]);
  }

  for (int i = 0; i < TOTAL_BATTERY_PERCENT_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(battery_percent_layers[i]));
    gbitmap_destroy(battery_percent_image[i]);
    bitmap_layer_destroy(battery_percent_layers[i]); 
  } 

  layer_remove_from_parent(window_layer);
  layer_destroy(window_layer);
		
//	window_destroy(window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
