#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

/***********
KNOWN BUGS
    when bonfire kills barrel, player would get points
    level 1 stair on second floor, mario stays in the air   FIXED
    doesn't change direction when RIGHT/LEFT key pressed    FIXED
    Q: does burps get inherited into the next level?        NOPE
    player cannot land on a ladder in the air               FIXED
************/


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
    string curLevel = "level";
    m_level = getLevel();
    if (m_level < 10)
        curLevel += "0" + to_string(m_level) + ".txt";
    else curLevel += to_string(m_level) + ".txt";
    m_lev = new Level(assetPath());
    Level::LoadResult result = m_lev->loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
    {
        cout << "Something bad happened\n";
        return GWSTATUS_LEVEL_ERROR;
    }

    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = m_lev->getContentsOf(x, y);
            // m_maze[y][x] = Level::empty;
            switch (item)
            {
            case Level::player:
                m_player = new Player(this, x, y);
                break;
            case Level::floor:
                m_actorList.push_back(new Floor(this, x, y));
                // m_maze[y][x] = item;
                break;
            case Level::ladder:
                m_actorList.push_back(new Ladder(this, x, y));
                // m_maze[y][x] = item;
                break;
            case Level::extra_life :
                m_actorList.push_back(new ExtraLifeGoodie(this, x, y));
                break;
            case Level::garlic:
                m_actorList.push_back(new GarlicGoodie(this, x, y));
                break;
            case Level::bonfire:
                m_actorList.push_back(new Bonfire(this, x, y));
                break;
            case Level::fireball:
                m_actorList.push_back(new Fireball(this, x, y));
                break;
            case Level::koopa:
                m_actorList.push_back(new Koopa(this, x, y));
                break;
            case Level::left_kong:
                m_actorList.push_back(new Kong(this, x, y, 180));
                break;
            case Level::right_kong:
                m_actorList.push_back(new Kong(this, x, y, 0));
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
    setGameStatText("Score: " + to_string(getScore()) + "    Level: " + to_string(getLevel())
        + "    Lives: " + to_string(getLives()) + "    Burps: "+to_string(m_player->getBurps()));
    
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

    if (!(m_player->is_alive()))
        return GWSTATUS_PLAYER_DIED;
        //return GWSTATUS_FINISHED_LEVEL;
    
    if (m_finishedLevel)
        return GWSTATUS_FINISHED_LEVEL;

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (auto actor : m_actorList)
        delete actor;
    m_actorList.clear();
    delete m_player;
    m_player = nullptr;
    m_finishedLevel = false;
}

void StudentWorld::atSameGrid(Actor* attacker)
{
    for (auto actor : m_actorList)
    {
        if (actor->getX() == attacker->getX() && actor->getY() == attacker->getY()
            && attacker->canAttack(actor))
        {
            actor->set_dead();
        }
    }
}