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
bool tackleWithPlace(bool placeOrNot, Board& b, int r, int c,int id){
    if(placeOrNot){
        if (b.placeShip(Point(r,c),id, VERTICAL))return true;
        if (b.placeShip(Point(r,c),id, HORIZONTAL)) return true;
        else return false;
    }
    else{
        if(b.unplaceShip(Point(r,c), id, VERTICAL))return true;
    if(b.unplaceShip(Point(r,c),id , HORIZONTAL)) return true;
    else{
        return false;
    }
    }
}



class MediocrePlayer : public Player
{
    public:
        MediocrePlayer(string nm, const Game& g);
        virtual bool placeShips(Board& b);
        bool mPlaceShip(Board& b, int id);
        virtual Point recommendAttack();
        virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
        virtual void recordAttackByOpponent(Point p);
        bool Duplicate(Point p);
        void randomShuffleAndAdd(Point p, int n);
    private:
        Point m_lastCellAttacked;
        Point m_lastCellAttackedByOpponent;
        vector <int> mId;
        vector <Point> mPoint;
        vector <Point> forAttackVector;
        int mstate;
        Point forAttack[100];
        vector <Point> forAttackUp;
        vector <Point> forAttackDown;
        vector <Point> forAttackLeft;
        vector <Point> forAttackRight;
        
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0), m_lastCellAttackedByOpponent(0, 0),mstate(1)
{}

bool MediocrePlayer::placeShips(Board& b){
    if (game().nShips()==0){
        return true;
    }
    int trying = 0;
    while (trying!= 50) {
        b.block();
        bool result=mPlaceShip(b,0);
        b.unblock();
        if (result){
            return true;
        }
        //cout << "trying " << trying << endl;
        b.clear();
        trying++;
    }
    return false;
}


bool MediocrePlayer::mPlaceShip(Board& b, int id){
    int temp;
    if (id==game().nShips()) {
        return true;
    }
    for (int r= 0; r< game().rows();r++) {
        for (int c= 0; c< game().cols();c++) {
            if (tackleWithPlace(1, b,r, c,id)) {
                temp = id+1;
                if (!mPlaceShip(b,temp)) {
                    tackleWithPlace(0,b,r, c,id);
                }else return true;
            }
        }
    }



    return false;

}

bool MediocrePlayer::Duplicate(Point p){
    for(int i = 0; i < mPoint.size(); i++){
        if(mPoint[i].c==p.c && mPoint[i].r==p.r){
            return true;
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack(){
    int row;    
    int col;
    if(mstate==1){
        row = randInt(game().rows());
        col = randInt(game().cols());
        while(Duplicate(Point(row,col))){
            row = randInt(game().rows());
            col = randInt(game().cols());
        }
        mPoint.push_back(Point(row,col));
        return Point(row,col); 
    }
    else if(mstate==2){
        //print out the vector for attack

        Point temp = forAttackVector[forAttackVector.size()-1];
        forAttackVector.pop_back();
        mPoint.push_back(temp);
        if(forAttackVector.size()==0){
            mstate=1;
        }
        return temp;
    }

}

void MediocrePlayer::randomShuffleAndAdd(Point p, int n=0){
    for(int i = 1; i <= 4; i++){
        if(p.c>=0 && p.r-i>=0){
            if(Duplicate(Point(p.r-i,p.c))==false){
                            forAttack[n]=Point(p.r-i,p.c);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c<game().cols() && p.r+i<game().rows()){
            if (Duplicate(Point(p.r+i,p.c))==false){
                            forAttack[n]=Point(p.r+i,p.c);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c+i<game().cols() && p.r<game().rows()){
            if (Duplicate(Point(p.r,p.c+i))==false){
                            forAttack[n]=Point(p.r,p.c+i);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c-i>=0 && p.r<game().rows()){
            if (Duplicate(Point(p.r,p.c-i))==false){
                            forAttack[n]=Point(p.r,p.c-i);
                            n++;
            }
        }
    }

    //random shuffle the array, and add every element to the vector forAttackVector
    for(int i = 0; i < n; i++){
        int j = randInt(n);
        Point temp = forAttack[i];
        forAttack[i] = forAttack[j];
        forAttack[j] = temp;
    }
    forAttackVector.clear();
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId){
    if(validShot){
        if(shotHit&&(!shipDestroyed)&&mstate==1){
            randomShuffleAndAdd(p);
            m_lastCellAttacked = p;
            mstate=2;
        }

        if(shotHit&&shipDestroyed){
            mstate=1;
        }
    }
    
}



void MediocrePlayer::recordAttackByOpponent(Point p){
    //do nothing
}

    

//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
    public:
        GoodPlayer(string nm, const Game& g);
        virtual bool placeShips(Board& b);
        bool mPlaceShip(Board& b, int id);
        virtual Point recommendAttack();
        virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
        virtual void recordAttackByOpponent(Point p);
        bool Duplicate(Point p);
        void randomShuffleAndAdd(Point p, int n);
        void randomShuffleAndAddUp(Point p);
        void randomShuffleAndAddDown(Point p);
        void randomShuffleAndAddLeft(Point p);
        void randomShuffleAndAddRight(Point p);
    private:
        Point m_lastCellAttacked;
        Point m_lastCellAttackedByOpponent;
        vector <int> mId;
        vector <Point> mPoint;
        vector <Point> forAttackVector;
        int mstate;
        Point forAttack[100];
        int mBoard[MAXROWS][MAXCOLS];
        int num = 0;
        
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0), m_lastCellAttackedByOpponent(0, 0),mstate(1)
{
    for(int i = 0; i < game().rows(); i++){
        for(int j = 0; j < game().cols(); j++){
            mBoard[i][j] = 0;
        }
    }

    //make the mboard as a checkboard
    for(int i = 0; i < game().rows(); i++){
        for(int j = 0; j < game().cols(); j++){
            if(i%2==0){
                if(j%2==0){
                    mBoard[i][j] = 1;
                }
            }
            else{
                if(j%2!=0){
                    mBoard[i][j] = 1;
                }
            }
        }
    }
}

bool GoodPlayer::placeShips(Board& b){
    if (game().nShips()==0){
        return true;
    }
    int trying = 0;
    while (trying!= 50) {
        b.block();

        bool result=mPlaceShip(b,0);
        b.unblock();
        if (result){
            return true;
        }
        //cout << "trying " << trying << endl;
        b.clear();
        trying++;
    }
    return false;
}


bool GoodPlayer::mPlaceShip(Board& b, int id){
    int temp;
    if (id==game().nShips()) {
        return true;
    }
    //do not let the ship go the center of the board
    for (int r= 0; r< game().rows();r++) {
        for (int c= 0; c< game().cols();c++) {
            if (tackleWithPlace(1, b,r, c,id)) {
                temp = id+1;
                if (!mPlaceShip(b,temp)) {
                    tackleWithPlace(0,b,r, c,id);
                }else return true;
            }
        }

        }
        return false;
    }



    



bool GoodPlayer::Duplicate(Point p){
    for(int i = 0; i < mPoint.size(); i++){
        if(mPoint[i].c==p.c && mPoint[i].r==p.r){
            return true;
        }
    }
    return false;
}

Point GoodPlayer::recommendAttack(){
    int row;    
    int col;
    if(mstate==1 || forAttackVector.size()==0){
        forAttackVector.clear();
        row = randInt(game().rows());
        col = randInt(game().cols());
        int trying =0;
        while(trying != 50){
            row = randInt(game().rows());
            col = randInt(game().cols());
            if( Duplicate(Point(row,col))==false && mBoard[row][col]==0){
                mPoint.push_back(Point(row,col));
                return Point (row,col);
            }
            trying++;
        }    
        while (trying!= 100) {
            row = randInt(game().rows());
            col = randInt(game().cols());
            if( Duplicate(Point(row,col))==false){
                mPoint.push_back(Point(row,col));
                return Point (row,col);
            }
            trying++;
        }
        mPoint.push_back(Point(row,col));
        return Point(row,col);
    }
    else{
        //print out the vector for attack

        for(int i = 0; i < forAttackVector.size(); i++){
        }
        Point temp = forAttackVector[forAttackVector.size()-1];
        forAttackVector.pop_back();

        
        mPoint.push_back(temp);
        if(forAttackVector.size()==0){
            mstate=1;
        }
        return temp;
    }

}



void GoodPlayer::randomShuffleAndAddUp(Point p){
    int n = 0;
    for(int i = 1; i <= 4; i++){
        if(p.c>=0 && p.r-i>=0){
            if(Duplicate(Point(p.r-i,p.c))==false){
                            forAttack[n]=Point(p.r-i,p.c);
                            n++;
            }
        }
    }
    forAttackVector.clear();
    //add to the vector forAttackVector
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}

void GoodPlayer::randomShuffleAndAddDown(Point p){
    int n=0;
    for(int i = 1; i <= 4; i++){
        if(p.c>=0 && p.r+i<game().rows()){
            if(Duplicate(Point(p.r+i,p.c))==false){
                            forAttack[n]=Point(p.r+i,p.c);
                            n++;
            }
        }
    }
    forAttackVector.clear();
    //add to the vector forAttackVector
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}

void GoodPlayer::randomShuffleAndAddLeft(Point p){
    int n=0;
    for(int i = 1; i <= 4; i++){
        if(p.c-i>=0){
            if(Duplicate(Point(p.r,p.c-i))==false){
                            forAttack[n]=Point(p.r,p.c-i);
                            n++;
            }
        }
    }
    forAttackVector.clear();
    //add to the vector forAttackVector
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}

void GoodPlayer::randomShuffleAndAddRight(Point p){
    int n=0;
    for(int i = 1; i <= 4; i++){
        if(p.c+i<game().cols()){
            if(Duplicate(Point(p.r,p.c+i))==false){
                            forAttack[n]=Point(p.r,p.c+i);
                            n++;
            }
        }
    }
    forAttackVector.clear();
    //add to the vector forAttackVector
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}


void GoodPlayer::randomShuffleAndAdd(Point p, int n=0){
    for(int i = 1; i <= 4; i++){
        if(p.c>=0 && p.r-i>=0){
            if(Duplicate(Point(p.r-i,p.c))==false){
                            forAttack[n]=Point(p.r-i,p.c);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c<game().cols() && p.r+i<game().rows()){
            if (Duplicate(Point(p.r+i,p.c))==false){
                            forAttack[n]=Point(p.r+i,p.c);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c+i<game().cols() && p.r<game().rows()){
            if (Duplicate(Point(p.r,p.c+i))==false){
                            forAttack[n]=Point(p.r,p.c+i);
                            n++;
            }
        }
    }
    for(int i = 1; i <= 4; i++){
        if(p.c-i>=0 && p.r<game().rows()){
            if (Duplicate(Point(p.r,p.c-i))==false){
                            forAttack[n]=Point(p.r,p.c-i);
                            n++;
            }
        }
    }

    //random shuffle the array, and add every element to the vector forAttackVector
    for(int i = 0; i < n; i++){
        int j = randInt(n);
        Point temp = forAttack[i];
        forAttack[i] = forAttack[j];
        forAttack[j] = temp;
    }
    forAttackVector.clear();
    for(int i = 0; i < n; i++){
        forAttackVector.push_back(forAttack[i]);
    }
}
void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,bool shipDestroyed, int shipId){
    if(validShot){
        if(shotHit&&(!shipDestroyed)&&mstate==1){
            randomShuffleAndAdd(p);
            m_lastCellAttacked = p;
            mstate=2;
        }

        // if(shotHit&&(!shipDestroyed)&&mstate==2 && p.r> m_lastCellAttacked.r){
        //     randomShuffleAndAddUp(p);
        //     m_lastCellAttacked = p;
        //     mstate=3;
        // }
        // if(shotHit&&(!shipDestroyed)&&mstate==2 && p.r< m_lastCellAttacked.r){
        //     randomShuffleAndAddDown(p);
        //     m_lastCellAttacked = p;
        //     mstate=4;
            
        // }
        // if(shotHit&&(!shipDestroyed)&&mstate==2 && p.c> m_lastCellAttacked.c){
        //     randomShuffleAndAddRight(p);
        //     m_lastCellAttacked = p;
        //     mstate=5;
        // }
        // if(shotHit&&(!shipDestroyed)&&mstate==2 && p.c< m_lastCellAttacked.c){
        //     randomShuffleAndAddLeft(p);
        //     m_lastCellAttacked = p;
        //     mstate=6;
        // }

        if(shotHit&&shipDestroyed){
            mstate=1;
        }
    }
    
}


void GoodPlayer::recordAttackByOpponent(Point p){
    //do nothing
}


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
