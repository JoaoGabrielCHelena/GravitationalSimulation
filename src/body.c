#include <SDL2/SDL.h>

#include "definitions.h"
#include "functions.h"
#include "view.h"
#include "body.h"

// with this, you will have to zoom out a lot to see anything
// also, this works best with at least 300 fps and set scroll speed to 1
// mass, size, and distance should be to scale. speed is just what was needed to keep a "stable" orbit
Body body_list[] = {
    {
        {0, 0}, {0, 0},
        {(350 + 0), 350}, 2853, 6054795, {255,255,0,255}, 0
    },
    {
        {0, 0}, {0, 1.45},
        {(350 + 2963), 350}, 10, 1, {255,165,0,255}, 0
    },
    {
        {0, 0}, {0, 1.43},
        {(350 + 3065), 350}, 25, 15, {144,238,144,255}, 0
    },
    {
        {0, 0}, {0, 1.43},
        {(350 + 3137), 350}, 26, 18, {0,0,255,255}, 0
    },
    {
        {0, 0}, {0, 1.42},
        {(350 + 3261), 350}, 14, 2, {255,0,0,255}, 0
    },
    {
        {0, 0}, {0, 1.22},
        {(350 + 4303), 350}, 105, 5778, {139,69,19,255}, 0
    },
    {
        {0, 0}, {0, 1.06},
        {(350 + 5573), 350}, 247, 1730, {255,215,0,255}, 0
    },
    {
        {0, 0}, {0, 0.9},
        {(350 + 7910), 350}, 105, 264, {0,255,255,255}, 0
    },
    {
        {0, 0}, {0, 0.77},
        {(350 + 10752), 350}, 101, 312, {0,0,139,255}, 0
    },
};

int body_count = sizeof(body_list) / sizeof(Body);

Vector2 gravity(Body body1, Body body2) {
    // Phytagorean theorem to get the distance from body1 to body2
    double x_distance = body2.position.x - body1.position.x;
    double y_distance = body2.position.y - body1.position.y;

    double distance = sqrt((x_distance * x_distance) + (y_distance * y_distance));

    // Newton's laws to get the acceleration
    double force = BIG_G * (body1.mass * body2.mass) / (distance*distance);
    double accel = force / body1.mass;

    // Triangle similarity to convert accel into accel_x and accel_y
    double accel_x = x_distance * accel / distance;
    double accel_y = y_distance * accel / distance;

    return (Vector2){accel_x, accel_y};
}

void update_bodies() {
    for (int primary=0; primary < body_count; primary++) {
        // If this body is marked as immovable, skip it
        if (body_list[primary].flags & IMMOVABLE) { continue; }

        // Stores the acceleration of primary body after it's calculation
        Vector2 calculated_accel = {0, 0};

        // Calculating the gravitaional attraction every other
        // body (Secondary bodies) exerts on primary body
        for (int secondary=0; secondary < body_count; secondary++) {
            // Do not let a body exert a force on itself
            if (primary == secondary) { continue; } 

            Vector2 temp_struct = gravity(body_list[primary], body_list[secondary]);

            calculated_accel.x += temp_struct.x;
            calculated_accel.y += temp_struct.y;
        }

        // Updating acceleration, velocity and position
        body_list[primary].accel = calculated_accel;

        body_list[primary].velocity.x += body_list[primary].accel.x;
        body_list[primary].velocity.y += body_list[primary].accel.y;

        body_list[primary].position.x += body_list[primary].velocity.x;
        body_list[primary].position.y += body_list[primary].velocity.y;
    }
}

void draw_bodies(SDL_Renderer* renderer, View* view) {
    for (int i=0; i < body_count; i++) {
        // If this body is marked as invisble, don't draw it
        if (body_list[i].flags & INVISIBLE) { continue; }

        SDL_SetRenderDrawColor(renderer, 
            body_list[i].color.r, body_list[i].color.g,
            body_list[i].color.b, body_list[i].color.a
        );

        draw_polygon(renderer, 
            (SDL_FPoint){
                (body_list[i].position.x + view->draw_offset.x) * view->scale,
                (body_list[i].position.y + view->draw_offset.y) * view->scale
            },

            body_list[i].size * view->scale,
            body_list[i].size * view->scale + 6
        );
    }
}