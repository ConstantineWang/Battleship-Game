#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************


bool getLineWithTwoIntegers(int& r, int& c, Board& b, Direction& dir, int k)
{

    cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
    bool result(cin >> r >> c);
    if (!result){
        cout << "You must enter two integers."<<endl;
        cin.clear();  // clear error state so can do more input operations
        cin.ignore(10000, '\n');
        return false;
    }
    if (!b.placeShip(Point(r,c), k, dir)){
        cout << "You cannot place a ship there." << endl;
        cin.ignore(10000, '\n');
        return false;
    }
    
    return true;
}

bool hOrV(char& c, const Game& g, int k)
{

    string shipName=g.shipName(k);
    int shipLength=g.shipLength(k);
    cout << "Enter h or v for direction of " << shipName << " (length " << shipLength << "): ";
    
    cin >> c;
    cin.ignore(10000, '\n');
    if(c=='h' || c=='v'){  
        return true;
    }
    else{
        cout << "Direction must be h or v." << endl;
        cin.clear();  // clear error state so can do more input operations
        return hOrV(c, g, k);
    }
}

bool getLineWithTwoIntegersForAttack(int& r, int& c)
{
    cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
    bool result(cin >> r >> c);
    if (!result){
        cout << "You must enter two integers."<<endl;
        cin.clear();  // clear error state so can do more input operations
        cin.ignore(10000, '\n');
        return false;
    }
    return true;
}


class HumanPlayer : public Player
{
    public:
        HumanPlayer(string nm, const Game& g): Player(nm, g){}
        virtual bool isHuman() const {return true;}
        virtual bool placeShips(Board& b){
            for (int k = 0; k < game().nShips(); k++)
            {
                char direction;
                Direction dir;              
                hOrV(direction, game(),k);
                if (direction == 'h'){
                    dir = HORIZONTAL;
                }
                else{
                    dir = VERTICAL;
                }
                int r, c;
                while(getLineWithTwoIntegers( r,  c, b, dir, k)==false){
                }
                if(k!=game().nShips()-1){
                    b.display(false);
                }               
            }  
            return true;
        }

        virtual Point recommendAttack(){
            int r, c;
            while(getLineWithTwoIntegersForAttack(r, c)==false){
            }
            return Point(r, c);
        }
        virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId){
                                                }
        virtual void recordAttackByOpponent(Point p){}


};




//*********************************************************************
//  MediocrePlayer
// class MediocrePlayer : public Player
// {
//     public:
//         MediocrePlayer(string nm, const Game& g);
//         virtual bool placeShips(Board& b);
// };




//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer MediocrePlayer;
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
typedef AwfulPlayer GoodPlayer;

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                    type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
