#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <stdbool.h>
//  573 / 3 = 191 -> sprite width
// 644 / 4 = 161  -> sprite height

typedef struct
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_FONT *font;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *bg;
    ALLEGRO_EVENT_QUEUE *event_queue;
} allegro_data_t;

static void init_allegro(allegro_data_t *allegro_data);
static void config_window(allegro_data_t *allegro_data);
static void register_event(allegro_data_t *allegro_data);
static void start_timer(allegro_data_t *allegro_data);
static void event_loop(allegro_data_t *allegro_data);
static void destroy_allegro(allegro_data_t *allegro_data);

int main (){
    allegro_data_t allegro_data = {0};
    init_allegro(&allegro_data);
    config_window(&allegro_data);
    register_event(&allegro_data);
    start_timer(&allegro_data);
    event_loop(&allegro_data);
    destroy_allegro(&allegro_data);
    return 0;
}

void init_allegro(allegro_data_t *allegro_data)
{
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();

    allegro_data->display = al_create_display(1280,720);
    allegro_data->font = al_load_font("./font.ttf", 25, 0);
    allegro_data->timer = al_create_timer(1.0 / 30.0);
    allegro_data->sprite = al_load_bitmap("./dragon.png");
    allegro_data->bg = al_load_bitmap("./bg.png");
    allegro_data->event_queue = al_create_event_queue();
}

void config_window(allegro_data_t *allegro_data)
{
    al_set_window_position(allegro_data->display, 200, 200);
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
    float frame = 0.f;
    int pos_x = 0, pos_y = 0;
    int current_frame_y = 161;

    while(true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(allegro_data->event_queue, &event);

        if( event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){
            break;
        }

        switch (event.keyboard.keycode)
        {
            case ALLEGRO_KEY_RIGHT:
                current_frame_y = 161;
                pos_x += 20;
                break;
            case ALLEGRO_KEY_LEFT:
                current_frame_y = 161 * 3;
                pos_x -= 20;
                break;
            case ALLEGRO_KEY_DOWN:
                current_frame_y = 161 * 2;
                pos_y += 20;
                break;
            case ALLEGRO_KEY_UP:
                current_frame_y = 0;
                pos_y -= 20;
                break;
        }

        frame += 0.3f;
        if( frame > 3){
            frame -= 3;
        }

        al_clear_to_color(al_map_rgb(255,255,255));
        al_draw_bitmap(allegro_data->bg, 0, 0, 0);
        al_draw_text(allegro_data->font, al_map_rgb(0,0,0), 7, 7, 0, "SCORE: dragon");
        al_draw_text(allegro_data->font, al_map_rgb(255,255,255), 5, 5, 0, "SCORE: dragon");
        al_draw_bitmap_region(allegro_data->sprite, 191 * (int)frame, current_frame_y, 191, 161, pos_x, pos_y, 0);
        al_flip_display();
    }
}

void destroy_allegro(allegro_data_t *allegro_data)
{
    al_destroy_bitmap(allegro_data->bg);
    al_destroy_bitmap(allegro_data->sprite);
    al_destroy_font(allegro_data->font);
    al_destroy_display(allegro_data->display);
    al_destroy_event_queue(allegro_data->event_queue);
}
