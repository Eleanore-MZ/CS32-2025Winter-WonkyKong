#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    string curLevel = "level00.txt";
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
    {
        cout << "Something bad happened\n";
        return GWSTATUS_LEVEL_ERROR;
    }

    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            switch (item)
            {
            case Level::player:
                std::cerr << "Player at (" << y << "," << x << ")" << std::endl;
                m_player = new Player(this, x, y);
                break;
            case Level::floor:
                std::cerr << "Floor at (" << y << "," << x << ")" << std::endl;
                m_actorList.push_back(new Floor(this, x, y));
                m_maze[y][x] = item;
                break;
            case Level::ladder:
                m_actorList.push_back(new Ladder(this, x, y));
                m_maze[y][x] = item;
                break;
            case Level::extra_life :
                m_actorList.push_back(new ExtraLifeGoodie(this, x, y));
                break;
            case Level::garlic:
                m_actorList.push_back(new GarlicGoodie(this, x, y));
                break;
            default:
                break;
            }
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    // update display text
    setGameStatText("Score: " + to_string(m_score) + "    Level: " + to_string(m_level)
        + "    Lives: " + to_string(m_lives) + "    Burps: "+to_string(m_player->getBurps()));
    
    // ask each actor to do something
    for (auto actor : m_actorList)
    {
        if (actor->is_alive())
            actor->doSomething();
    }
    m_player->doSomething();
    
    // delete dead actors
    auto actor = m_actorList.begin();
    while (actor != m_actorList.end())
    {
        if (!((*actor)->is_alive()))
        {
            delete* actor;
            actor = m_actorList.erase(actor);
        }
        else actor++;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (auto actor : m_actorList)
        delete actor;
    delete m_player;
}