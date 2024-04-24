#include <iostream>
// #include <SDL2/SDL.h>
// #include <box2d/box2d.h>
#include "include/box2d/box2d.h"
#include "include/SDL2/SDL.h"

const float kScale = 30;
const float kWindow_width = 800;
const float kWindow_height = 600;

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Event events;
    bool quit = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Initialization error : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    window = SDL_CreateWindow("Bouncing Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, kWindow_width, kWindow_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "Creation error : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cout << "Renderer creation error : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    b2Vec2 gravity(0.0f, 9.81f);
    b2World world(gravity);

    // ground
    int ground_box_height = kWindow_height * 0.1;
    int ground_box_width = kWindow_width;
    int ground_box_x = kWindow_width / 2;
    int ground_box_y = kWindow_height * 0.95;

    b2BodyDef ground_body_def;
    ground_body_def.position.Set(ground_box_x / kScale, ground_box_y / kScale);
    b2Body* ground_body = world.CreateBody(&ground_body_def);
    b2PolygonShape ground_box;
    ground_box.SetAsBox(ground_box_width / 2 / kScale, ground_box_height / 2 / kScale);
    ground_body->CreateFixture(&ground_box, 0.0f);

    // ball
    b2BodyDef ball_body_def;
    ball_body_def.type = b2_dynamicBody;
    ball_body_def.position.Set(kWindow_width / 2 / kScale, kWindow_height / 2 / kScale);
    b2Body* ball_body = world.CreateBody(&ball_body_def);
    b2CircleShape ball_shape;
    ball_shape.m_radius = 0.5f;
    b2FixtureDef ball_fixture_def;
    ball_fixture_def.shape = &ball_shape;
    ball_fixture_def.density = 1.0f;
    ball_fixture_def.friction = 0.3f;
    ball_fixture_def.restitution = 0.8f;
    ball_body->CreateFixture(&ball_fixture_def);

    while (!quit)
    {
        while (SDL_PollEvent(&events))
        {
            if (events.type == SDL_QUIT)
                quit = true;
        }

        world.Step(1.0f / 60.0f, 6, 2);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw ground
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int ground_x = ground_box_x - ground_box_width / 2;
        int ground_y = ground_box_y - ground_box_height / 2;
        SDL_Rect groundRect = { (int)ground_x, (int)ground_y, ground_box_width, ground_box_height };
        SDL_RenderFillRect(renderer, &groundRect);

        // draw ball
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        b2Vec2 ball_position = ball_body->GetPosition();
        int ball_x = (ball_position.x - ball_shape.m_radius + 0.01) * kScale;
        int ball_y = (ball_position.y - ball_shape.m_radius + 0.01) * kScale;
        int ball_diameter = (int)(ball_shape.m_radius * 2 * kScale);
        SDL_Rect ball_rect = { ball_x, ball_y, ball_diameter, ball_diameter };
        SDL_RenderFillRect(renderer, &ball_rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
