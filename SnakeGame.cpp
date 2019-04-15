#include <SFML/Graphics.hpp>   //using namespace sf
#include <time.h>
#include <iostream>

//dimensions for window size and background
int num_vertBox = 30, num_horzBox = 20;
int size = 16; //number of pixels
int w = size * num_horzBox;  //background number of pixels in width
int h = size * num_vertBox;  //background number of pixels in height

//Snake variables to dertermine length and direction
int P1direction, P2direction;    //direction the snake is moving
int P1snake_length = 4, P2snake_length = 4; //initial size is 4

//Controls speed of snakes
float delay = 0.1f;

//Actual Snake in game is an array of x,y coordinates of sprite2
struct Snake1
{
    int x, y;        //each sprite of snake needs an x,y coordinate
}s1[100];

struct Snake2
{
    int x, y;
}s2[100];

//***NEW*** this is the fruit or food that the snake will eat
struct Fruit
{
    int x, y;    // only need one sprite needed for one food item
}food;

//move snake head based on user input and body by incrementing
//forward by iterating through arrays previous position
void move() {
    //1st update body so everything updates in proper order
    //move the body only!  s[0] = head will not be changed here
    for (int i = P1snake_length; i > 0; --i)
    {
        s1[i].x = s1[i - 1].x;
        s1[i].y = s1[i - 1].y;
    }
    
    for (int j = P2snake_length; j > 0; --j)
    {
        s2[j].x = s2[j - 1].x;
        s2[j].y = s2[j - 1].y;
    }
    
    //2nd update the head
    //Head of snake, s[0] depends on direction user inputs
    
    //if player1 presses up
    if (P1direction == 0) s1[0].y -= 1;
    
    //if player1 presses down
    if (P1direction == 1) s1[0].y += 1;
    
    //if player1 presses left
    if (P1direction == 2) s1[0].x -= 1;
    
    //if player1 presses right
    if (P1direction == 3) s1[0].x += 1;
    
    //if player2 presses up
    if (P2direction == 0) s2[0].y -= 1;
    
    //if player2 presses down
    if (P2direction == 1) s2[0].y += 1;

    //if player2 presses left
    if (P2direction == 2) s2[0].x -= 1;
    
    //if player2 presses right
    if (P2direction == 3) s2[0].x += 1;
    
    //***NEW*** If Snake eats food it should grow
    //check if snake head = food location
    if (((s1[0].x) == food.x) && ((s1[0].y) == food.y))
    {
        //increment snake
        P1snake_length++;
        
        //Randomly place food somewhere else
        food.x = rand() % num_horzBox;
        food.y = rand() % num_vertBox;
        delay -= 0.005f;
    }
    
    if (((s2[0].x) == food.x) && ((s2[0].y) == food.y))
    {
        //increment snake
        P2snake_length++;
        
        //Randomly place food somewhere else
        food.x = rand() % num_horzBox;
        food.y = rand() % num_vertBox;
        delay -= 0.005f;
    }
    
    //***NEW*** Boundary Checking snake as is hits screen end
    //loop snake back on other side
    
    //LEFT and RIGHT
    if (s1[0].x > num_horzBox) s1[0].x = 0; //player 1
    if (s1[0].x < 0) s1[0].x = num_horzBox; //player 1
    
    if (s2[0].x > num_horzBox) s2[0].x = 0; //player 2
    if (s2[0].x < 0) s2[0].x = num_horzBox; //player 2
    
    //TOP and BOTTOM
    if (s1[0].y > num_vertBox) s1[0].y = 0; //player 1
    if (s1[0].y < 0) s1[0].y = num_vertBox; //player 1
    
    if (s2[0].y > num_vertBox) s2[0].y = 0; //player 2
    if (s2[0].y < 0) s2[0].y = num_vertBox; //player 2
    
    //***NEW*** Check if you eat body of snake(s)
    for (int i = 1; i < P1snake_length; i++)
    {
        //Cut Snake body from place eaten
        if ((s1[0].x == s1[i].x && s1[0].y == s1[i].y) ||
            (s1[0].x == s2[i].x && s1[0].y == s2[i].y)) {
            P1snake_length = i;
            delay = 0.1f;
        }
    }
    
    for (int j = 1; j < P2snake_length; j++)
    {
        //Cut Snake body from place eaten
        if ((s2[0].x == s2[j].x && s2[0].y == s2[j].y) ||
            (s2[0].x == s1[j].x && s2[0].y == s1[j].y)) {
            P2snake_length = j;
            delay = 0.1f;
        }
    }
}

int main()
{
    //Setting pseudorandom time, TODO:discuss true random vs pseudorandom
    srand(time(0));

    //Window that we can play the game in
    sf::RenderWindow window(sf::VideoMode(w, h), "Snake Game");

    //Textures load an image into the GPU Memory
    sf::Texture t1, t2, t3, t4;
    t1.loadFromFile("/Users/gilescarlos32/Desktop/images/white.png");
    t2.loadFromFile("/Users/gilescarlos32/Desktop/images/red.png");
    t3.loadFromFile("/Users/gilescarlos32/Desktop/images/goldenApple.png");
    t4.loadFromFile("/Users/gilescarlos32/Desktop/images/green.png");
    
    //Sprite has physical dimmensions that can be set in
    //coordinate system, setPosition(x,y), and drawn on screen
    sf::Sprite sprite1(t1);
    sf::Sprite sprite2(t2);
    sf::Sprite sprite3(t3);
    sf::Sprite sprite4(t4);
    
    sf::Clock clock;
    float timer = 0.0f;
    
    //***NEW*** initially place food somewhere on screen
    food.x = 10;
    food.y = 15;
    
    while (window.isOpen())
    {
        //Error in class I placed this before the while loop
        //Should be placed in while loop as shown here to update timer
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        //Allow us to check when a user does something
        sf::Event e;
        
        //Check when the window is closed
        while (window.pollEvent(e))
        {
            //If user presses x in the top right, Windows, top left, Mac,  close the window
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
        //Control for Snake by User1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) P1direction = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) P1direction = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) P1direction = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) P1direction = 3;
        
        //Control for Snake by User1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) P2direction = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) P2direction = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) P2direction = 2;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) P2direction = 3;
        
        if (timer > delay)
        {
            timer = 0; //reset timer
            move();    //move Snake one sprite forward
        }

        /*****************
         //Draw in window
         *****************/
        window.clear();    //clear the window so new frame can be drawn in
        //NOTE: Order matters as we will draw over items listed first.
        //Hence the background should be the first thing you will always do
        //1st: Draw Background first
        for (int i = 0; i < num_horzBox; i++)
        {
            for (int j = 0; j < num_vertBox; j++)
            {
                //Set position of sprite1 one at a time
                sprite1.setPosition(i*size, j*size);
                
                //Draw sprite1 but, do not show it on screen.
                window.draw(sprite1);
            }
        }
        
        //2nd: Then Draw snakes otherwise background will be drawn over snakes if order was reversed with background
        for (int i = 0; i < P1snake_length; i++)
        {
            //position sprite2 one at a time
            sprite2.setPosition(s1[i].x*size, s1[i].y*size);
            
            //Draw sprite2 one at a time by drawing over background
            window.draw(sprite2);
        }
    
        for (int i = 0; i < P2snake_length; i++)
        {
            //position sprite2 one at a time
            sprite4.setPosition(s2[i].x*size, s2[i].y*size);
            
            //Draw sprite2 one at a time by drawing over background
            window.draw(sprite4);
        }
        
        //***NEW*** 3rd: Draw Fruit
        sprite3.setPosition(food.x*size, food.y*size);
        window.draw(sprite3);
        
        //Show everything we have drawn on the screen
        window.display();
    }
    return 0;
}
