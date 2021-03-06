#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <list>

using namespace std;

class ship{
    public:
        ship(int len, string name, char symbol,int id):mlength(len),mName(name),mSymbol(symbol),mId(id){}
        int length() const{return mlength;}
        string name() const{return mName;}
        char symbol() const{return mSymbol;}

    private:
        int mlength;
        string mName;
        char mSymbol;
        int mId;
};

class GameImpl
{
    public:
        GameImpl(int nRows, int nCols);
        int rows() const;
        int cols() const;
        bool isValid(Point p) const;
        Point randomPoint() const;
        bool addShip(int length, char symbol, string name);
        int nShips() const;
        int shipLength(int shipId) const;
        char shipSymbol(int shipId) const;
        string shipName(int shipId) const;
        Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
    private:
        int mRows;
        int mCols;
        vector<ship> mShips;
};


void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    mRows=nRows;
    mCols=nCols;
}

int GameImpl::rows() const
{
    return mRows;
}

int GameImpl::cols() const
{
    return mCols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    mShips.push_back(ship(length,name,symbol,mShips.size()));
    return true;
}

int GameImpl::nShips() const
{
    return mShips.size();
}

int GameImpl::shipLength(int shipId) const
{
    return mShips[shipId].length();
}

char GameImpl::shipSymbol(int shipId) const
{
    return mShips[shipId].symbol();
}

string GameImpl::shipName(int shipId) const
{
    return mShips[shipId].name();
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(p2->isHuman()==true){
        cout << p2->name() << " must place " << this->nShips() << " ships." << endl;
    }
    bool p1ship = p1->placeShips(b1);
    bool p2ship = p2->placeShips(b2);
    if(p1ship==false || p2ship==false){
        cout << "That's not one of the choices." << endl;
        return nullptr;
    }
    // cin.ignore(10000, '\n');
    while(b1.allShipsDestroyed()==false && b2.allShipsDestroyed()==false){
        //??????????????????????????????
        if(p1->isHuman()==false && p2->isHuman()==false){
            //player1's turn
            cout << p1->name() << "'s turn. Board for " <<  p2->name() << ':'<<endl;
            bool hit;
            bool destroy;
            int shipId;
            b2.display(false);
            //Make the first player's attack
            Point p = p1->recommendAttack();
            if(b2.attack(p, hit, destroy, shipId)){
                if(destroy==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and destroyed " << this->shipName(shipId) << ", resulting in:" << endl;
                }
                else if(hit==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and missed, resulting in:" << endl;
                }
                        //record the attack
                p1->recordAttackResult(p,true, hit, destroy, shipId);
                p2->recordAttackByOpponent(p);
            }
            else{
                p1->recordAttackResult(p,false, hit, destroy, shipId);
                cout << p1->name() << " wasted a shot at (" << p.r << ',' << p.c << ")." << endl;
            }
            b2.display(false);

            if(b2.allShipsDestroyed()){
                cout << p1->name() << " wins!" << endl;
                return p1;
            }

            if(shouldPause){
                waitForEnter();
            }

            //player2's turn
            cout << p2->name() << "'s turn. Board for " <<  p1->name() << ':'<<endl;
            bool hit2;
            bool destroy2;
            int shipId2;
            b1.display(false);
            //Make the first player's attack
            Point q = p2->recommendAttack();
            if(b1.attack(q, hit2, destroy2, shipId2)){
                if(destroy2==true){
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and destroyed " << this->shipName(shipId2) << ", resulting in:" << endl;
                }
                else if(hit2 == true){
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and missed, resulting in:" << endl;
                }
                        //record the attack
                p2->recordAttackResult(q, true, hit2, destroy2, shipId2);
                p1->recordAttackByOpponent(q);
            }
            else{
                p2->recordAttackResult(q, false, hit2, destroy2, shipId2);
                cout << p2->name() << " wasted a shot at (" << q.r << ',' << q.c << ")." << endl;
            }
            b1.display(false);
            //if should pause, wait for enter

            if(b1.allShipsDestroyed()){
                cout << p2->name() << " wins!" << endl;
                return p2;
            }

            if(shouldPause){
                waitForEnter();
            }
        }


        //??????????????? ?????????????????? ???????????????
        if(p1->isHuman()==false && p2->isHuman()==true){
            cout << p1->name() << "'s turn. Board for " <<  p2->name() << ':'<<endl;
            bool hit;
            bool destroy;
            int shipId;
            b2.display(false);
            //Make the first player's attack
            Point p = p1->recommendAttack();


            if(b2.attack(p, hit, destroy, shipId)){
                if(destroy==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and destroyed " << this->shipName(shipId) << ", resulting in:" << endl;
                }
                else if(hit==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and missed, resulting in:" << endl;
                }
                        //record the attack
                p1->recordAttackResult(p,true, hit, destroy, shipId);
                p2->recordAttackByOpponent(p);
            }
            else{
                p1->recordAttackResult(p,false, hit, destroy, shipId);
                cout << p1->name() << " wasted a shot at (" << p.r << ',' << p.c << ")." << endl;
            }
            b2.display(false);

            if(b2.allShipsDestroyed()){
                cout << p1->name() << " wins!" << endl;
                return p1;
            }

            waitForEnter();
            //player2's turn
            cout << p2->name() << "'s turn. Board for " <<  p1->name() << ':'<<endl;
            bool hit2;
            bool destroy2;
            int shipId2;
            b1.display(true);

            //Make the first player's attack
            Point q = p2->recommendAttack();
            if(b1.attack(q, hit2, destroy2, shipId2)){
                if(destroy2==true){
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and destroyed " << this->shipName(shipId2) << ", resulting in:" << endl;
                }
                else if(hit2 == true){
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and missed, resulting in:" << endl;
                }
                b1.display(true);
                        //record the attack
                p2->recordAttackResult(q, true, hit2, destroy2, shipId2);
                p1->recordAttackByOpponent(q);
            }
            else{
                p2->recordAttackResult(q, false, hit2, destroy2, shipId2);
                cout << p2->name() << " wasted a shot at (" << q.r << ',' << q.c << ")." << endl;
            }

            if(b1.allShipsDestroyed()){
                cout << p2->name() << " wins!" << endl;
                return p2;
            }         
            cin.ignore(10000, '\n');
            waitForEnter();
        }

        //??????????????? ?????????????????? ??????????????????
        if(p1->isHuman()==true && p2->isHuman()==false){
            waitForEnter();
            cout << p1->name() << "'s turn. Board for " <<  p2->name() << ':'<<endl;
            bool hit;
            bool destroy;
            int shipId;
            b2.display(true);
            //Make the first player's attack
            Point p = p1->recommendAttack();
            if(b2.attack(p, hit, destroy, shipId)){
                if(destroy==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and destroyed " << this->shipName(shipId) << ", resulting in:" << endl;
                }
                else if(hit==true){
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p1->name() << " attacked (" << p.r << ',' << p.c << ") and missed, resulting in:" << endl;
                }
                b2.display(true);
                        //record the attack
                p1->recordAttackResult(p,true, hit, destroy, shipId);
                p2->recordAttackByOpponent(p);
            }
            else{
                p1->recordAttackResult(p,false, hit, destroy, shipId);
                cout << p1->name() << " wasted a shot at (" << p.r << ',' << p.c << ")." << endl;
            }
            

            if(b2.allShipsDestroyed()){
                cout << p1->name() << " wins!" << endl;
                return p1;
            }
            cin.ignore(10000, '\n');
            waitForEnter();
            
            //player2's turn
            cout << p2->name() << "'s turn. Board for " <<  p1->name() << ':'<<endl;
            bool hit2;
            bool destroy2;
            int shipId2;
            b1.display(false);

            //Make the first player's attack
            Point q = p2->recommendAttack();
            if(b1.attack(q, hit2, destroy2, shipId2)){
                if(destroy2==true) {
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and destroyed " << this->shipName(shipId2) << ", resulting in:" << endl;
                }
                else if(hit2 == true){
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and hit something, resulting in:" << endl;
                }
                else{
                    cout << p2->name() << " attacked (" << q.r << ',' << q.c << ") and missed, resulting in:" << endl;
                }
                
                        //record the attack
                p2->recordAttackResult(q, true, hit2, destroy2, shipId2);
                p1->recordAttackByOpponent(q);
            }
            else{
                p2->recordAttackResult(q, false, hit2, destroy2, shipId2);
                cout << p2->name() << " wasted a shot at (" << q.r << ',' << q.c << ")." << endl;
            }
            b1.display(false);

            if(b1.allShipsDestroyed()){
                cout << p2->name() << " wins!" << endl;
                return p2;
            }         

        }


    }

}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

