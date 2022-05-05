// TODO: Проверить как часто обновляются дата и время (дата - раз в день, время - раз в минуту)


#include <pebble.h>

// Объявляем главное окно
static Window *s_main_window;

// Объявляем слой текста (время)
static TextLayer *s_time_layer;

// Объявляем слой текста (день - месяц)
static TextLayer *s_date_layer;

// Объявляем слой текста (день - месяц)
static TextLayer *s_day_layer;

// Объявляем шрифт
static GFont s_time_font;

// Объявляем шрифт
static GFont s_date_font;

static int a = 0;

static int b = 0;
// Объявляем bitmap слой
//static BitmapLayer *s_background_layer;

//Объявляем bitmap
//static GBitmap *s_background_bitmap;

// Функция обновления времени
static void update_time() {
  // Получение структуры tm
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Запись времени (часы и минуты) в буффер
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

  // Запись дня - месяца в буффер
  static char s_date_buffer[8];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%d %b", tick_time);

  // Запись 
  static char s_day_buffer[8];
  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", tick_time);

  // Отображение дня - месяца на слое
  text_layer_set_text(s_date_layer, s_date_buffer);

  // Отображение 
  text_layer_set_text(s_day_layer, s_day_buffer);
}

// Функция-обработчик смены времени
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void day_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_day();
}

// Функция для загрузки главного окна
static void main_window_load(Window *window) {
  // Получение информации об окне
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Создание текстового слоя (TextLayer) со своими границами (время)
  s_time_layer = text_layer_create(
    GRect(0, PBL_IF_ROUND_ELSE(58, 26), bounds.size.w, bounds.size.h) // 42 - нижняя граница текста
  );

  // Создание текстового слоя (TextLayer) со своими границами (день - месяц)
  s_date_layer = text_layer_create(
    GRect(0, 100, bounds.size.w, bounds.size.h) // 42 - нижняя граница текста
  );

  // Создание текстового слоя (TextLayer) со своими границами ()
  s_day_layer = text_layer_create(
    GRect(0, 74, bounds.size.w, bounds.size.h) // 42 - нижняя граница текста
  );

  // Делаем из слоя циферблат

  // Установка цвета фона текста
  text_layer_set_background_color(s_time_layer, GColorClear);

  // Установка цвета фона текста
  text_layer_set_background_color(s_date_layer, GColorClear);

  // Установка цвета фона текста
  text_layer_set_background_color(s_day_layer, GColorClear);

  // Установка цвета текста
  text_layer_set_text_color(s_time_layer, GColorFromRGB(211, 236, 167));
  //text_layer_set_text(s_time_layer, "00:00"); // Установка текста

  // Установка цвета текста
  text_layer_set_text_color(s_date_layer, GColorFromRGB(211, 236, 167));

  // Установка цвета текста
  text_layer_set_text_color(s_day_layer, GColorFromRGB(211, 236, 167));

  // Установка шрифта
  //text_layer_set_font(s_time_layer, fonts_get_system_font(BITHAM_42_BOLD)); FONT_KEY_ROBOTO_BOLD_SUBSET_49

  // Добавление нестандартного шрифта циферблата
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DANCINGSCRIPT_MEDIUM_48));

  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DANCINGSCRIPT_MEDIUM_24));

  // Применение шрифта к тексту
  text_layer_set_font(s_time_layer, s_time_font);

  // Применение шрифта к тексту
  text_layer_set_font(s_date_layer, s_date_font);

  // Применение шрифта к тексту
  text_layer_set_font(s_day_layer, s_date_font);

  // Установка выравнивания текста
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Установка выравнивания текста
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Установка выравнивания текста
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

  // Создание Bitmap
  //s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  // Создание слоя для отображения Bitmap
  //s_background_layer = bitmap_layer_create(bounds);

  // Установка Bitmap для Bitmap слоя
  //bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);

  // Добавление Bitmap слоя, как дочернего, к слою главного окна (Порядок слоёв важен)
  //layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Добавление текстового слоя, как дочернего, к слою главного окна (Порядок слоёв важен)
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // Добавление текстового слоя, как дочернего, к слою главного окна (Порядок слоёв важен)
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  // Добавление текстового слоя, как дочернего, к слою главного окна (Порядок слоёв важен)
  layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
}

// Функция для уничтожения главного окна
static void main_window_unload(Window *window) {
  // Уничтожение слоя с циферблатом
  text_layer_destroy(s_time_layer);

  // Уничтожение слоя с циферблатом
  text_layer_destroy(s_date_layer);

  // Уничтожение слоя с циферблатом
  text_layer_destroy(s_day_layer);

  // Уничтожение шрифта
  fonts_unload_custom_font(s_time_font);

  // Уничтожение Bitmap
  //gbitmap_destroy(s_background_bitmap);

  // Уничтожение Bitmap слоя
  //bitmap_layer_destroy(s_background_layer);
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

  // Создание обработчика события "Изменение времени"
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  tick_timer_service_subscribe(DAY_UNIT, day_handler);
  
  // Отображение окна на часах с animated=true
  window_stack_push(s_main_window, true);

  // Отображение правильного времени
  update_time();

  //
  update_day();

  // Задаём чёрный фон ???
  window_set_background_color(s_main_window, GColorFromRGB(25, 40, 47));
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