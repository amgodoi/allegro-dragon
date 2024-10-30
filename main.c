#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <stdbool.h>

// 573 / 3 = 191 -> sprite width
// 644 / 4 = 161  -> sprite height

#define BLACK 0, 0, 0
#define WHITE 255, 255, 255

static const char *FONT_FILE_NAME = "./font.ttf";
static const char *BACKGROUND_FILE_NAME = "./bg.png";
static const char *DRAGON_SPRITE_FILE_NAME = "./dragon.png";
static const int DISPLAY_WIDTH  = 1280;
static const int DISPLAY_HEIGHT = 720;
static const int DISPLAY_X_INITIAL_POSITION = 200;
static const int DISPLAY_Y_INITIAL_POSITION = 200;
static const int FONT_SIZE = 25;
static const float FPS = 30.0f;
static const float DISPLAY_TEXT_X_POSITION = 5.0f;
static const float DISPLAY_TEXT_Y_POSITION = 5.0f;
static const float DISPLAY_TEXT_SHADOW_INCREMENT = 3.0f;
static const float DISPLAY_TEXT_X_POSITION_SHADOW = DISPLAY_TEXT_X_POSITION + DISPLAY_TEXT_SHADOW_INCREMENT;
static const float DISPLAY_TEXT_Y_POSITION_SHADOW = DISPLAY_TEXT_Y_POSITION + DISPLAY_TEXT_SHADOW_INCREMENT; 
static const int MAX_X_IMAGE_SIZE = 573;
static const int MAX_X_IMAGES = 3;
static const int MAX_Y_IMAGE_SIZE = 644;
static const int MAX_Y_IMAGES = 4;
static const float SPRITE_X_SIZE = (int) (MAX_X_IMAGE_SIZE / MAX_X_IMAGES); // 191
static const float SPRITE_Y_SIZE = (int) (MAX_Y_IMAGE_SIZE / MAX_Y_IMAGES); // 161
static const float POSITION_INCREMENT = 20.0f;

typedef struct
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_FONT *font;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *bg;
    ALLEGRO_EVENT_QUEUE *event_queue;
} allegro_data_t;

static void init(allegro_data_t *allegro_data);
static void config_window(allegro_data_t *allegro_data);
static void register_event(allegro_data_t *allegro_data);
static void start_timer(allegro_data_t *allegro_data);
static void event_loop(allegro_data_t *allegro_data);
static void destroy(allegro_data_t *allegro_data);

static void draw_text_with_shadow(allegro_data_t *allegro_data, const char *text);

int main (){
    allegro_data_t allegro_data = {0};
    init(&allegro_data);
    config_window(&allegro_data);
    register_event(&allegro_data);
    start_timer(&allegro_data);
    event_loop(&allegro_data);
    destroy(&allegro_data);
    return 0;
}

void init(allegro_data_t *allegro_data)
{
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();

    allegro_data->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    allegro_data->font = al_load_font(FONT_FILE_NAME, FONT_SIZE, 0);
    allegro_data->timer = al_create_timer(1.0 / FPS);
    allegro_data->sprite = al_load_bitmap(DRAGON_SPRITE_FILE_NAME);
    allegro_data->bg = al_load_bitmap(BACKGROUND_FILE_NAME);
    allegro_data->event_queue = al_create_event_queue();
}

void config_window(allegro_data_t *allegro_data)
{
    al_set_window_position(allegro_data->display, DISPLAY_X_INITIAL_POSITION, DISPLAY_Y_INITIAL_POSITION);
    al_set_window_title(allegro_data->display, "Here be Dragons!");
}

void register_event(allegro_data_t *allegro_data)
{
    al_register_event_source(allegro_data->event_queue, al_get_display_event_source(allegro_data->display));
    al_register_event_source(allegro_data->event_queue, al_get_timer_event_source(allegro_data->timer));
    al_register_event_source(allegro_data->event_queue, al_get_keyboard_event_source() );
}

void start_timer(allegro_data_t *allegro_data)
{
    al_start_timer(allegro_data->timer);
}

void event_loop(allegro_data_t *allegro_data)
{
    float frame = 0.0f;
    float pos_x = 0.0f;
    float pos_y = 0.0f;
    float current_frame_x = SPRITE_X_SIZE + frame;
    float current_frame_y = SPRITE_Y_SIZE;

    while (true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(allegro_data->event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        switch (event.keyboard.keycode)
        {
            case ALLEGRO_KEY_RIGHT:
                current_frame_y = SPRITE_Y_SIZE * 1;
                pos_x += POSITION_INCREMENT;
                break;
            case ALLEGRO_KEY_LEFT:
                current_frame_y = SPRITE_Y_SIZE * 3;
                pos_x -= POSITION_INCREMENT;
                break;
            case ALLEGRO_KEY_DOWN:
                current_frame_y = SPRITE_Y_SIZE * 2;
                pos_y += POSITION_INCREMENT;
                break;
            case ALLEGRO_KEY_UP:
                current_frame_y = SPRITE_Y_SIZE * 0;
                pos_y -= POSITION_INCREMENT;
                break;
        }

        frame += 0.3f;
        if (frame > 3)
        {
            frame -= 3;
        }
        current_frame_x = SPRITE_X_SIZE * (int) frame;

        al_clear_to_color(al_map_rgb(WHITE));
        al_draw_bitmap(allegro_data->bg, BLACK);
        al_draw_bitmap_region(allegro_data->sprite, current_frame_x, current_frame_y, SPRITE_X_SIZE, SPRITE_Y_SIZE, pos_x, pos_y, 0);
        draw_text_with_shadow(allegro_data, "SCORE: dragon");
        al_flip_display();
    }
}

void draw_text_with_shadow(allegro_data_t *allegro_data, const char *text)
{
    al_draw_text(allegro_data->font, al_map_rgb(BLACK), DISPLAY_TEXT_X_POSITION_SHADOW, DISPLAY_TEXT_Y_POSITION_SHADOW, 0, text);
    al_draw_text(allegro_data->font, al_map_rgb(WHITE), DISPLAY_TEXT_X_POSITION, DISPLAY_TEXT_Y_POSITION, 0, text);
}

void destroy(allegro_data_t *allegro_data)
{
    al_destroy_bitmap(allegro_data->bg);
    al_destroy_bitmap(allegro_data->sprite);
    al_destroy_font(allegro_data->font);
    al_destroy_display(allegro_data->display);
    al_destroy_event_queue(allegro_data->event_queue);
}
