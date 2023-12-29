#include <iostream>
#include <raylib.h>

using namespace std;
const int screen_width = 1200;
const int screen_height = 800;
int player_score = 0;
int ai_score = 0;
Color c1 = Color{38, 125, 154, 255};
Color c2 = Color{20, 160, 133, 255};
Color c3 = Color{129, 204, 184, 255};
Color cBall = Color{157, 52, 75, 255};

class Ball{
    public:
    float x, y;
    int spd_x; int spd_y;
    int radius;

    Ball();
    Ball(int x, int y, int radius, int spd_x, int spd_y){
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->spd_x = spd_x;
        this->spd_y = spd_y;
    };

    int getY(){
        return this->y;
    }

    void draw(){
        DrawCircle(x, y, radius, cBall);
    }

    void update(){
        x += spd_x;
        y += spd_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            spd_y *= -1;
        }
        if(x + radius >= GetScreenWidth()){
            player_score++;
            ResetBall();
        } 
        if(x - radius <= 0){
            ai_score++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = screen_width/2;
        y = screen_height/2; 
        radius = 20;
        int spd_choices[2] = {-1, 1};
        spd_x *= spd_choices[GetRandomValue(0, 1)];
        spd_y *= spd_choices[GetRandomValue(0, 1)];
    }


};

class Paddle{
protected:
    void LimitMvt(){
         if(this->y <= 0){
            y = 0;
        }
        if(this->y + this->h >= GetScreenHeight()){
            y = GetScreenHeight() - this->h;
        }
    }
public:
    float x, y;
    float w, h;
    int spd;

    Paddle(float x, float y, float w, float h, int spd){
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->spd = spd;
    }

    void draw(){
        DrawRectangleRounded(Rectangle{x, y, w, h}, 0.8, 0, WHITE);
    }

    void update(){
        if(IsKeyDown(KEY_UP)){
            this->y -= spd;
        }
        if(IsKeyDown(KEY_DOWN)){
            this->y += spd;
        }
        LimitMvt();
    }
};

class AiPaddle: public Paddle{
public:
    AiPaddle(float x, float y, float w, float h, int spd) :  Paddle(x, y, w, h, spd){}

    void update(int ball_y){
        if(this->y + h/2 > ball_y){
            y -= this->spd;
        } else if(this->y + h/2 < ball_y){
            y += this->spd;
        }
        LimitMvt();
    }
};

int main () {
    InitWindow(screen_width, screen_height, "The Pong");    
    SetTargetFPS(60);

    // Initialize the ball
    Ball ball = Ball(screen_width/2, screen_height/2, 20, 7, 7);
    Paddle player = Paddle(10, screen_height/2 - 60, 25, 120, 7);
    AiPaddle ai = AiPaddle(screen_width - 35 , screen_height/2 - 60, 25, 120, 5);

    while(!WindowShouldClose()){
        BeginDrawing();

        // Update Game State
        ball.update();
        player.update();
        ai.update(ball.getY());

        // Check for collusion
        if(CheckCollisionCircleRec(
            Vector2{ball.x, ball.y}, ball.radius, 
            Rectangle{player.x, player.y, player.w, player.h}))
        {
            ball.spd_x *= -1;
        } 

        if(CheckCollisionCircleRec(
             Vector2{ball.x, ball.y}, ball.radius, 
            Rectangle{ai.x, ai.y, ai.w, ai.h}))
        {
            ball.spd_x *= -1;   
        }
 


        // Remove all previous game states
        ClearBackground(c2);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, c1);
        DrawCircle(screen_width/2, screen_height/2, 150 ,c3);
        DrawLine(screen_width/2, 0, screen_width/2, screen_width, WHITE);

        // Draw Assets 
        ball.draw();
        player.draw();
        ai.draw();
        DrawText(TextFormat("%i", ai_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);
        EndDrawing();
    }









    CloseWindow();
    return 0;
    
}