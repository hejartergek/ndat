#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>

const int screenWidth = 800;
const int screenHeight = 600;

class SnakeSegment {
public:
    int x, y;

    SnakeSegment(int xPos, int yPos)
        : x(xPos), y(yPos) {}
};

class Snake {
public:
    std::vector<SnakeSegment> segments;
    int direction;

    Snake() {
        int startX = screenWidth / 2;
        int startY = screenHeight / 2;
        int segmentSize = 10;

        segments.push_back(SnakeSegment(startX, startY));
        segments.push_back(SnakeSegment(startX - segmentSize, startY));
        segments.push_back(SnakeSegment(startX - 2 * segmentSize, startY));

        direction = GLUT_KEY_RIGHT;
    }

    void draw() {
        glColor3f(1.0f, 1.0f, 1.0f);
        for (const auto& segment : segments) {
            glRectf(segment.x, segment.y, segment.x + 10, segment.y + 10);
        }
    }

    void move() {
        int segmentSize = 10;
        int headX = segments[0].x;
        int headY = segments[0].y;

        switch (direction) {
        case GLUT_KEY_UP:
            segments.insert(segments.begin(), SnakeSegment(headX, headY + segmentSize));
            break;
        case GLUT_KEY_DOWN:
            segments.insert(segments.begin(), SnakeSegment(headX, headY - segmentSize));
            break;
        case GLUT_KEY_LEFT:
            segments.insert(segments.begin(), SnakeSegment(headX - segmentSize, headY));
            break;
        case GLUT_KEY_RIGHT:
            segments.insert(segments.begin(), SnakeSegment(headX + segmentSize, headY));
            break;
        }

        segments.pop_back();
    }
};

Snake snake;

int foodX, foodY;
bool foodEaten = true;

void generateFood() {
    int segmentSize = 10;
    int maxX = screenWidth / segmentSize - 1;
    int maxY = screenHeight / segmentSize - 1;

    foodX = rand() % maxX * segmentSize;
    foodY = rand() % maxY * segmentSize;

    foodEaten = false;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    snake.draw();

    if (foodEaten) {
        generateFood();
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(foodX, foodY, foodX + 10, foodY + 10);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        snake.direction = key;
        break;
    }
}

void update(int value) {
    snake.move();

    if (snake.segments[0].x == foodX && snake.segments[0].y == foodY) {
        snake.segments.push_back(SnakeSegment(-10, -10));
        foodEaten = true;
    }

    for (int i = 1; i < snake.segments.size(); i++) {
        if (snake.segments[0].x == snake.segments[i].x && snake.segments[0].y == snake.segments[i].y) {
            std::cout << "Oyun bitti!" << std::endl;
            exit(0);
        }
    }

    if (snake.segments[0].x < 0 || snake.segments[0].x >= screenWidth
        || snake.segments[0].y < 0 || snake.segments[0].y >= screenHeight) {
        std::cout << "Oyun bitti!" << std::endl;
        exit(0);
    }

    glutPostRedisplay();
    glutTimerFunc(100, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Yýlan Oyunu");

    glewInit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutMainLoop();

    return 0;
}
