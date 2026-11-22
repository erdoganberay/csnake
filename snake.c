#include <stdio.h>
#include "term.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void print_pawn(int snake_x, int snake_y){

    printf("\x1b[32m");  // make the text color green
    printf("\x1b[%d;%dH██", snake_y, snake_x); // print the head
    printf("\x1b[0m"); // return to original text color
    fflush(stdout);

}

void print_hunt(int hunt_x, int hunt_y){

    printf("\x1b[31m");  // make the text color red
    printf("\x1b[%d;%dH██", hunt_y, hunt_x); // print the hunt
    printf("\x1b[0m"); // return to original text color
    fflush(stdout);

}


int main() {

    configure_terminal();
    signal(SIGINT, signal_handler);


    int body_x[4096] = {0};
    int body_y[4096] = {0};
    char buf[4096];

    int snake_vel_x = 1; 
    int snake_vel_y = 0;
    int snake_len = 1;
    int snake_x = 1;
    int snake_y = 1;

    int hunt_x = get_term_col() / 2;
    int hunt_y = get_term_row() / 2;
    
    //game loop
    while(!exit_loop){

        read(STDIN_FILENO, &buf,3);
        
        // snake_vel_x is 2 or -2 because its 2 chars wide
        if (buf[0] == 'q')
            break;
        else if (buf[0] == '\x1b' && buf[1] == '['){
            if (snake_vel_x == 0){
                switch(buf[2]){
                    case 'C': 
                        snake_vel_x = 2;
                        snake_vel_y = 0;
                        break;
                    case 'D': 
                        snake_vel_x = -2;
                        snake_vel_y = 0;
                        break;
                }
            }
            else if(snake_vel_y == 0){
                switch(buf[2]){
                    case 'A': 
                        snake_vel_x = 0;
                        snake_vel_y = -1;
                        break;
                    case 'B':
                        snake_vel_x = 0;
                        snake_vel_y = 1;
                        break;
                }
            }
        }
        
        
        snake_x = snake_x + snake_vel_x;
        snake_y = snake_y + snake_vel_y;

        
        // change hunts pos when hunted
        if (snake_y == hunt_y && snake_x >= hunt_x - 1 && snake_x <= hunt_x + 1){
            hunt_y = rand() % (get_term_row() - 2) + 1;
            hunt_x = rand() % (get_term_col() - 2) + 1;
            ++snake_len;
        }

        // update the snake body 
        for (int i = snake_len - 1; i > 0; --i) {
            body_x[i] = body_x[i - 1];
            body_y[i] = body_y[i - 1];
        }

        body_x[0] = snake_x;
        body_y[0] = snake_y;


        // adjust to game according to term size
        if(snake_x > get_term_col() - 1)
            snake_x = 1;
        if (snake_x < 1)
            snake_x = get_term_col();
        if (snake_y > get_term_row())
            snake_y = 1;
        if (snake_y < 1)
            snake_y = get_term_row();

        if(hunt_x > get_term_col() - 1)
            hunt_x = get_term_col() -1;
        if (hunt_x < 1)
            hunt_x = 1;
        if (hunt_y > get_term_row())
            hunt_y = get_term_row();
        if (hunt_y < 1)
            hunt_y = 1;

        // exit when snake eats itself
        for (int i = snake_len - 1; i > 0; --i) {
            if (snake_x == body_x[i] && snake_y == body_y[i])
                exit(1);
        }



        // draw the game
        printf("\x1b[2J"); // clear the screen
                           
        print_hunt(hunt_x,hunt_y);

        for(int i = 0; i < snake_len; ++i)
            print_pawn(body_x[i], body_y[i]);

        usleep(100000);  
    }

    
    return 0;
}


