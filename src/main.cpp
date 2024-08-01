#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../resources/glfw/include/GLFW/glfw3.h"


#define WIDTH 500
#define HEIGHT 500
#define TITLE "Raycasting 2D | Hasan Efe Aksoy"

#define PI 3.1415926
#define BIG 1000000

int rayCount = 120;
int wallCount = 7;

double xpos = 0.0, ypos = 0.0;







float randFloat(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * (max - min);
}




class Wall {
public:
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 0.0f;
    float y2 = 0.0f;


    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;




    Wall(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void tick() {
        glColor4f(this->r, this->g, this->b, this->a);
        glBegin(GL_LINE_STRIP);
        glVertex2f(this->x1, this->y1);
        glVertex2f(this->x2, this->y2);
        glEnd();
    }
};

class Ray {
public:
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 0.0f;
    float y2 = 0.0f;
    float length = BIG;
    float angle = 0.0f;

    float x2_fix = 0.0;
    float y2_fix = 0.0;


    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;




    Ray(float angle, float r, float g, float b, float a) {
        this->angle = angle;

        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void tick() {
        this->x1 = xpos;
        this->y1 = ypos;

        this->x2 = this->x1 + this->length * cos(this->angle);
        this->y2 = this->y1 + this->length * sin(this->angle);

        this->x2_fix = this->x1 + BIG * cos(this->angle);
        this->y2_fix = this->y1 + BIG * sin(this->angle);
        

        glColor4f(this->r, this->g, this->b, this->a);
        glBegin(GL_LINE_STRIP);
        glVertex2f(this->x1, this->y1);
        glVertex2f(this->x2, this->y2);
        glEnd();
    }


    float* intersectionPosition(Wall* wall) {
        float* position = new float[2];

        float wall_x1 = wall->x1;
        float wall_y1 = wall->y1;
        float wall_x2 = wall->x2;
        float wall_y2 = wall->y2;


        float denominator = (this->x1 - this->x2_fix) * (wall_y1 - wall_y2) - (this->y1 - this->y2_fix) * (wall_x1 - wall_x2);
    
        if (denominator == 0) {
            return nullptr;
        }

        float t = ((this->x1 - wall_x1) * (wall_y1 - wall_y2) - (this->y1 - wall_y1) * (wall_x1 - wall_x2)) / denominator;
        float u = -(((this->x1 - this->x2_fix) * (this->y1 - wall_y1) - (this->y1 - this->y2_fix) * (this->x1 - wall_x1)) / denominator);
        

        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            position[0] = this->x1 + t * (this->x2_fix - this->x1);
            position[1] = this->y1 + t * (this->y2_fix - this->y1);
            return position;
        }
        else {
            return nullptr;
        }
    }
};




int main()
{
    srand(static_cast<unsigned>(time(0)));

    GLFWwindow* window = nullptr;
    
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, 0);
    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    std::vector<Ray*> rays;
    std::vector<Wall*> walls;

    for (float i = 0; i < 2*PI; i += (2*PI/rayCount)) {
        rays.push_back(new Ray(i, 1.0f, 1.0f, 0.0f, 1.0f));
    }
    for (int i = 0; i < wallCount; i++) {
        float x1_ = randFloat(-WIDTH/2, WIDTH/2);
        float y1_ = randFloat(-HEIGHT/2, HEIGHT/2);
        float x2_ = randFloat(-WIDTH/2, WIDTH/2);
        float y2_ = randFloat(-HEIGHT/2, HEIGHT/2);

        walls.push_back(new Wall(x1_, y1_, x2_, y2_, 1.0f, 1.0f, 1.0f, 1.0f));
    }
    walls.push_back(new Wall(-WIDTH/2, -HEIGHT/2, -WIDTH/2, HEIGHT/2, 1.0f, 1.0f, 1.0f, 0.0f));
    walls.push_back(new Wall(WIDTH/2, -HEIGHT/2, WIDTH/2, HEIGHT/2, 1.0f, 1.0f, 1.0f, 0.0f));
    walls.push_back(new Wall(-WIDTH/2, HEIGHT/2, WIDTH/2, HEIGHT/2, 1.0f, 1.0f, 1.0f, 0.0f));
    walls.push_back(new Wall(-WIDTH/2, -HEIGHT/2, WIDTH/2, -HEIGHT/2, 1.0f, 1.0f, 1.0f, 0.0f));


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwGetCursorPos(window, &xpos, &ypos);
        xpos = xpos - WIDTH / 2;
        ypos = HEIGHT / 2 - ypos;

        
        for (int i = 0; i < walls.size(); i++) {
            walls[i]->tick();
        }
        for (int i = 0; i < rays.size(); i++) {
            float closestDistance = BIG;

            for (int j = 0; j < walls.size(); j++) {
                float* result = rays[i]->intersectionPosition(walls[j]); 
                if (result != nullptr) {
                    float distance = std::sqrt(std::pow(result[0] - rays[i]->x1, 2) + std::pow(result[1] - rays[i]->y1, 2));
                    if (distance < closestDistance) {
                        closestDistance = distance;
                    }
                    delete[] result;
                }
            }

            if (closestDistance < BIG) {
                rays[i]->length = closestDistance;
            } else {
                rays[i]->length = BIG;
            }

            rays[i]->tick();
        }
        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }




    for (auto wall : walls) {
        delete wall;
    }
    for (auto ray : rays) {
        delete ray;
    }



    glfwTerminate();
    return 0;
}
