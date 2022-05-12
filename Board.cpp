#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful
    const Game& m_game;
    int mRows;
    int mCols;
    int destroyedShips;
    vector <char> mIds;
    vector <int> mSymbols;
    char realBoard[MAXROWS][MAXCOLS];

};

BoardImpl::BoardImpl(const Game& g)
    : m_game(g)
{
    mRows = m_game.rows();
    mCols = m_game.cols();

    for (int i = 0; i < mCols; i++)
    {
        for (int j = 0; j < mRows; j++)
        {
            realBoard[i][j] = '.';
        }
    }
}

void BoardImpl::clear()
{
    for (int i = 0; i < mCols; i++)
    {
        for (int j = 0; j < mRows; j++)
        {
            realBoard[i][j] = '.';
        }
    }
}

void BoardImpl::block()
{
    int num = 0;
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++){
            if (randInt(2) == 0)
            {
                for (int r = 0; r < mRows; r++)
                {
                    for (int c = 0; c < mCols; c++)
                    {
                        if(realBoard[r][c] == 'X')
                        {
                            num++;
                        }
                    }
                }
                if(num >= mRows*mCols/2)
                {
                    return;
                } 

                realBoard[r][c] = 'X';
            }
        }
        if(num <= mRows*mCols/2)
        {
            block();
        }
        return;
}


void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (realBoard[r][c] == 'X')
            {
                realBoard[r][c] = '.';
            }
        }
}


bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId<0 || shipId>(m_game.nShips()-1)){
        return false;
    }
    

    //for horizontal
    if(dir==0 && (topOrLeft.c>=mCols || m_game.shipLength(shipId)>(mCols-topOrLeft.c))){
        return false;
    }
    //for vertial
    if(dir==1 && (topOrLeft.r>=mRows || m_game.shipLength(shipId)>(mRows-topOrLeft.r))){
        return false;
    }
    //if there is already a ship there
    if(dir==0){
        for(int i=0;i<m_game.shipLength(shipId);i++){
            if(realBoard[topOrLeft.r][topOrLeft.c+i]!='.'){
                return false;
            }
        }   
    }
    if(dir==1){
        for(int i=0;i<m_game.shipLength(shipId);i++){
            if(realBoard[topOrLeft.r+i][topOrLeft.c]!='.'){
                return false;
            }
        }   
    }
    //horizontal addship    
    if(dir==0){
        for(int i=0; i<m_game.shipLength(shipId);i++){
            realBoard[topOrLeft.r][topOrLeft.c+i]=m_game.shipSymbol(shipId);
        }
        mIds.push_back(shipId);
        mSymbols.push_back(m_game.shipSymbol(shipId));
        return true;
    }

    else{
        for(int i=0; i<m_game.shipLength(shipId);i++){
            realBoard[topOrLeft.r+i][topOrLeft.c]=m_game.shipSymbol(shipId);
        }
        mIds.push_back(shipId);
        mSymbols.push_back(m_game.shipSymbol(shipId));
        return true;
    }
        
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if(shipId<0 || shipId>(m_game.nShips()-1)){
        return false;
    }

    //for horizontal
    if(dir==0 && (topOrLeft.c>=mCols || m_game.shipLength(shipId)>(mCols-topOrLeft.c))){
        return false;
    }
    //for vertial
    if(dir==1 && (topOrLeft.r>=mRows || m_game.shipLength(shipId)>(mRows-topOrLeft.r))){
        return false;
    }
    //if there is not a ship there
    if(dir==0){
        for(int i=0;i<m_game.shipLength(shipId);i++){
            if(realBoard[topOrLeft.r][topOrLeft.c+i]=='.'){
                return false;
            }
        }   
    }
    if(dir==1){
        for(int i=0;i<m_game.shipLength(shipId);i++){
            if(realBoard[topOrLeft.r+i][topOrLeft.c]=='.'){
                return false;
            }
        }   
    }
    for (int i = 0; i < m_game.nShips(); i++){
        if (i != shipId){
            return false;
        }
    }

    //horizontal addship    
    if(dir==0){
        //if the clear is not enough, return false
        if(realBoard[topOrLeft.r][topOrLeft.c-1]==m_game.shipSymbol(shipId)){
            return false;
        }
        if (realBoard[topOrLeft.r][topOrLeft.c+m_game.shipLength(shipId)]==m_game.shipSymbol(shipId)){
            return false;
        }
        for(int i=0; i<m_game.shipLength(shipId);i++){
            realBoard[topOrLeft.r][topOrLeft.c+i]='.';
        }

        return true;
    }

    else{
        //if the clear is not enough, return false
        if(realBoard[topOrLeft.r-1][topOrLeft.c]==m_game.shipSymbol(shipId)){
            return false;
        }
        if (realBoard[topOrLeft.r+m_game.shipLength(shipId)][topOrLeft.c]==m_game.shipSymbol(shipId)){
            return false;
        }
        for(int i=0; i<m_game.shipLength(shipId);i++){
            realBoard[topOrLeft.r+i][topOrLeft.c]='.';
        }
        return true;
    }
}

void BoardImpl::display(bool shotsOnly) const
{
    //first line
    cout << "  ";
    for (int i= 0; i < mCols; i++){
        cout << i;
    }
    cout << endl;

    //if display all
    if(shotsOnly == false){
        for (int j = 0; j < mRows; j++)
        {
            cout << j << " ";
            for (int c = 0; c < mCols; c++)
            {
                cout << realBoard[j][c];
            }
            cout << endl;
        }
    }
    //if only display shots
    if(shotsOnly == true){
        for (int j = 0; j < mRows; j++)
        {
            cout << j << " ";
            for (int c = 0; c < mCols; c++)
            {  
                if(realBoard[j][c]!='o' && realBoard[j][c]!='x' &&realBoard[j][c]!='.'){
                    cout << '.';
                }else{
                    cout << realBoard[j][c];
                }
                
            }
            cout << endl;
        }
    }



}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    //return false if the attack is invalid (the attack point is outside of the board area, or an attack is made on a previously attacked location)
    if(p.r<0 || p.r>=mRows || p.c<0 || p.c>=mCols || realBoard[p.r][p.c]=='o' || realBoard[p.r][p.c]=='X'){
        shotHit=false;
        shipDestroyed=false;
        return false;
    }
    //if it hit nothing
    if(realBoard[p.r][p.c]=='.'){
        realBoard[p.r][p.c]='o';
        shotHit=false;
        shipDestroyed=false;
        return true;
    }
    //if any undamaged segment of a ship is at position p on the board,then the shotHit parameter must be set to true
    if (realBoard[p.r][p.c]!='.'){
        shotHit = true;
        char shipSymbol = realBoard[p.r][p.c];
        realBoard[p.r][p.c]='X';
        //if a whole ship is destroyed, the shipId parameter must be set to the ship's id
        //loop the whole board to check whether the symbol of the ship is destroyed
        for(int i=0;i<mRows;i++){
            for(int j=0;j<mCols;j++){
                if(realBoard[i][j]==shipSymbol){
                    shipDestroyed=false;
                    return true;
                }
            }
        }

        //use mId and mSymbol to find the ship's id
        char tempId=' ';
        for(int i=0;i<mIds.size();i++){
            if(shipSymbol==mSymbols[i]){
                tempId=mIds[i];
                mIds.erase(mIds.begin()+i);
            }
        }
        shipId=tempId;
        shipDestroyed=true;
        
        return true;
    }

    
}

bool BoardImpl::allShipsDestroyed() const
{

    if(mIds.size()==0){
        return true;
    }
    return false;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
