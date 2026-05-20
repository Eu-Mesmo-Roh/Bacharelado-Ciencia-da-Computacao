#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


int main()
{
    int x;
    int y;
    al_init();
    al_install_keyboard();
    ALLEGRO_COLOR cor = al_map_rgb(255, 0, 255);
    ALLEGRO_DISPLAY *display = al_create_display(1024, 768);
    al_clear_to_color(cor);
    al_flip_display();
    al_rest(1.0);
    al_clear_to_color(al_map_rgb(0, 0, 255));
    al_flip_display();
    al_rest(10.0);
    al_destroy_display(display);

    return 0;
}