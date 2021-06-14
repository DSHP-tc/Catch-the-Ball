#include "SFMl/Graphics.hpp"
#include<sstream>
#include<string>
#include<iostream>
class Game
{
public:
    Game();
    void run();
private:
    void processEvent();
    void update(sf::Time);
    void render();
    void handlePlayerInput(sf::Keyboard::Key,bool);
    std::string toString(int);
    void reset();
    void endGame();

private:
    sf::RenderWindow myWin;
    sf::RectangleShape pedal;
    sf::CircleShape ball;
    float playerSpeed=800.0f;
    sf::Time TimePerFrame= sf::seconds(1.0f/60.0f);
    bool isMovingLeft=false;
    bool isMovingRight=false;
    bool ballshooted=false;
    bool gameOver=false;
    int playerScore=0;
    sf::Font font;
    sf::Text scoreText;
    sf::Text finalText;


    sf::Vector2f ballSpeed;


};

Game::Game(): myWin(sf::VideoMode(800,600),"Catch"),pedal(sf::Vector2f(80.0f,20.0f)),ball(15.0f),ballSpeed(200.0f,-200.0f)
{
    myWin.setVerticalSyncEnabled(true);
    pedal.setPosition(sf::Vector2f(360.0f,575.0f));
    ball.setPosition(sf::Vector2f(385.0f,545.0f));
    font.loadFromFile("arial.ttf");
    scoreText.setCharacterSize(20);
    scoreText.setFont(font);
    scoreText.setString("Score : "+toString(playerScore));
    scoreText.setPosition(sf::Vector2f(10.0f,5.0f));

    finalText.setCharacterSize(20);
    finalText.setFont(font);
    finalText.setString("Game Ended, do you want to restart? (Y/N)");
    finalText.setPosition(sf::Vector2f(10.0f,30.0f));
}
void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(myWin.isOpen())
    {
        processEvent();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate>TimePerFrame)
            {
                timeSinceLastUpdate-=TimePerFrame;
                update(TimePerFrame);
            }

        render();
    }
}

std::string Game::toString(int num)
{
    std::stringstream ss;
    ss<<num;
    return ss.str();
}

void Game::processEvent()
{
    sf::Event event;
    while(myWin.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            myWin.close();
            break;
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code,true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code,false);
            break;
        default:
            break;
        }
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key,bool isPressed)
{
    if(key==sf::Keyboard::Right)
        isMovingRight=isPressed;
    else if(key==sf::Keyboard::Left)
        isMovingLeft=isPressed;
    else if(key==sf::Keyboard::Space)
        ballshooted=true;
    else if(key==sf::Keyboard::Y)
        reset();
    else if(key==sf::Keyboard::N)
        myWin.close();
}
void Game::update(sf::Time dt)
{
    sf::Vector2f movePedal(0.0f,0.0f);
    sf::Vector2f moveball(0.0f,0.0f);

    if(isMovingLeft && pedal.getPosition().x>0 && ballshooted==true)
        movePedal.x-=playerSpeed;
    if(isMovingRight && pedal.getPosition().x+80<800 && ballshooted==true)
        movePedal.x+=playerSpeed;
    if(ball.getPosition().x+30>=800 || ball.getPosition().x<=0)
        ballSpeed.x*=-1;
    if(ball.getPosition().y<=0)
        ballSpeed.y*=-1;
    if(ball.getPosition().y+30>600)
        {
            endGame();
        }


    if(ball.getGlobalBounds().intersects(pedal.getGlobalBounds()) && gameOver==false)
        {
            if(ball.getPosition().y+15>pedal.getPosition().y)
                endGame();
            else
            {
                ballSpeed.y*=-1.2;
                playerScore+=1;
            }
        }


    if(ballshooted==true)
    {
        moveball.x+=ballSpeed.x;
        moveball.y+=ballSpeed.y;
        ball.move(moveball*dt.asSeconds());
    }

    scoreText.setString("Score : "+toString(playerScore));
    pedal.move(movePedal*dt.asSeconds());
}

void Game::endGame()
{
    gameOver=true;
    ballSpeed.x*=0;
    ballSpeed.y*=0;
}

void Game::render()
{
    myWin.clear();
    myWin.draw(scoreText);
    if(gameOver==true)
        myWin.draw(finalText);
    myWin.draw(pedal);
    myWin.draw(ball);
    myWin.display();
}

void Game::reset()
{
    isMovingLeft=false;
    isMovingRight=false;
    ballshooted=false;
    gameOver=false;
    playerScore=0;
    ballSpeed.x=200.0f;
    ballSpeed.y=200.0f;
    pedal.setPosition(sf::Vector2f(360.0f,575.0f));
    ball.setPosition(sf::Vector2f(385.0f,545.0f));
    scoreText.setString("Score : "+toString(playerScore));
}

int main()
{
    Game game;
    game.run();
    return 0;
}
