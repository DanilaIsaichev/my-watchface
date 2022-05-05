#include <pebble.h>

// Объявляем главное окно
static Window *s_main_window;

// Объявляем слои текста (время)
static TextLayer *s_time_layer; // время
static TextLayer *s_day_layer; // день недели
static TextLayer *s_date_layer; // дата
static TextLayer *s_weather_layer; // погода

// Объявляем шрифты
static GFont s_dancingscript_m_48;
static GFont s_dancingscript_m_24;


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
  
  // Буфферы для хранения информации о погоде
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layout_buffer[32];

  // Чтение кортежей данных
  Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);

  //
  if (temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
    snprintf(weather_layout_buffer, sizeof(weather_layout_buffer), "%s, %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, weather_layout_buffer);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}


// Функция обновления времени
static void update_time() {
  // Получение структуры tm
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Запись времени в буффер
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Отображение времени на слое
  text_layer_set_text(s_time_layer, s_buffer);
}

// Функция обновления дней
static void update_day() {
  // Получение структуры tm
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Запись в буффер
  static char s_day_buffer[8];
  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", tick_time); // день недели
  static char s_date_buffer[8];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%d %b", tick_time); // дата

  // Отображение на слое
  text_layer_set_text(s_day_layer, s_day_buffer); // день недели
  text_layer_set_text(s_date_layer, s_date_buffer); // дата
  
}

// Функция-обработчик смены времени (минуты)
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // Получение данных о погоде каждые 30 минут
  if(tick_time->tm_hour % 12 == 0) {
    update_day();
  }

  // Получение данных о погоде каждые 30 минут
  if(tick_time->tm_min % 30 == 0) {
    // Создание словаря
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Заполнение словаря
    dict_write_uint8(iter, 0, 0);

    // Отправка сообщения
    app_message_outbox_send();
  }
}

// Функция для загрузки главного окна
static void main_window_load(Window *window) {
  // Получение информации об окне
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Создание текстовых слоёв (TextLayer) со своими границами
  s_time_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(29, 26), bounds.size.w, bounds.size.h) // время
  );
  s_day_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(77, 74), bounds.size.w, bounds.size.h) // день недели
  );
  s_date_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(103, 100), bounds.size.w, bounds.size.h) // дата
  );
  s_weather_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(129, 126), bounds.size.w, bounds.size.h) // погода
  );
  
  // Делаем циферблат

  // Установка цвета фона текста
  text_layer_set_background_color(s_time_layer, GColorClear); // время
  text_layer_set_background_color(s_day_layer, GColorClear); // день недели
  text_layer_set_background_color(s_date_layer, GColorClear); // дата
  text_layer_set_background_color(s_weather_layer, GColorClear); // дата

  // Установка цвета текста
  text_layer_set_text_color(s_time_layer, GColorFromRGB(211, 236, 167)); // время
  text_layer_set_text_color(s_day_layer, GColorFromRGB(211, 236, 167)); // день недели
  text_layer_set_text_color(s_date_layer, GColorFromRGB(211, 236, 167)); // дата
  text_layer_set_text_color(s_weather_layer, GColorFromRGB(211, 236, 167)); // дата

  // Установка шрифта
  // Добавление нестандартных шрифтов циферблата
  s_dancingscript_m_48 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DANCINGSCRIPT_MEDIUM_48));
  s_dancingscript_m_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DANCINGSCRIPT_MEDIUM_24));

  // Применение шрифтов к тексту
  text_layer_set_font(s_time_layer, s_dancingscript_m_48); // время
  text_layer_set_font(s_day_layer, s_dancingscript_m_24); // день недели
  text_layer_set_font(s_date_layer, s_dancingscript_m_24); // дата
  text_layer_set_font(s_weather_layer, s_dancingscript_m_24); // дата

  // Установка выравнивания текста
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter); // время
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter); // день недели
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter); // дата
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter); // дата

  // Добавление текстовых слоёв, как дочерних, к слою главного окна (Порядок слоёв важен)
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer)); // время
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer)); // день недели
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer)); // дата
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer)); // дата
}

// Функция для уничтожения главного окна
static void main_window_unload(Window *window) {
  // Уничтожение слоёв циферблата
  text_layer_destroy(s_time_layer); // время
  text_layer_destroy(s_day_layer); // день недели
  text_layer_destroy(s_date_layer); // дата
  text_layer_destroy(s_weather_layer); // дата

  // Уничтожение шрифтов
  fonts_unload_custom_font(s_dancingscript_m_48);
  fonts_unload_custom_font(s_dancingscript_m_24);
}

// Функция инициализации
static void init() {
  // Создание элемента главного окна и назначение указателя
  s_main_window = window_create();

  // Установка обработчиков элементов внутри окна
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Создание обработчика событий "Изменение времени"
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler); // время
  
  // Отображение окна на часах с animated=true
  window_stack_push(s_main_window, true);

  // Обновление
  update_time(); // времени
  update_day(); // дней

  // Задаём цвет фона
  window_set_background_color(s_main_window, GColorFromRGB(25, 40, 47));

  //
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
}

// Функция деинициализации
static void deinit() {
  // Уничтожение окна
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}