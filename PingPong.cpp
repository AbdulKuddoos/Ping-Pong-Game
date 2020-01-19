#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

int         width    = 55;
int         height   = 35;
const float maxSpeed = 1.8f;
int   delay    = 40 ;
int endTime = 0;

HANDLE hOut;

struct paddle {
  float x, y;
  int size;
  float ox, oy;
  float vx;

  paddle(int _x, int _y, int _size) {
    x = float( _x );
    y = float( _y );
    size = _size;
    ox = float( _x );
    oy = float( _y );
  }

  void draw() {
    COORD p;
    p.X = int( ox ) - size ;
    p.Y = int( oy );
    SetConsoleCursorPosition( hOut, p );
    for (int i = -size; i < size; ++i)
      cout << ' ';
    p.X = int( x ) - size;
    p.Y = int( y );
    SetConsoleCursorPosition( hOut, p );
    SetConsoleTextAttribute( hOut, 224 );
    for (int i = -size; i < size; ++i){
      SetConsoleTextAttribute( hOut, 1 );
      cout << '|';
    }
    SetConsoleTextAttribute( hOut, 224 );
    ox = x;
    oy = y;
  }
};

struct ball {
  float x, y, ox, oy;
  float vx, vy;

  ball(int _x, int _y) {
    x = float( _x );
    y = float( _y );
    ox = float( x );
    oy = float( y );
    vx = vy = 0;
  }

  void draw() {
    COORD p;
    p.X = int(ox);
    p.Y = int(oy);
    SetConsoleCursorPosition( hOut, p );
    cout << ' ';
    p.X = int(x);
    p.Y = int(y);
    SetConsoleCursorPosition( hOut, p );
    SetConsoleTextAttribute( hOut, 224 );
    cout << 'O';
    SetConsoleTextAttribute( hOut, 225 );
    ox = x;
    oy = y;
  }
};

void reset(paddle &you, paddle &comp, ball &b, bool &started, int &you_score, int &comp_score) {
  b.x = float( width / 2 - 1 );
  b.y = float( height / 3 );
  you.x = comp.x = float( width / 2 );
  comp.vx = 0.0f;
  b.vx = float(rand() % 3) - 1.5f;
  b.vy = 1;
  started = true;

  COORD p;
  p.X = 0;
  p.Y = height;
  SetConsoleCursorPosition( hOut, p );
  cout << "\n    Your score: " << you_score << "    Comp score: " << comp_score << "    Timer: " << endTime/1000 << "       \n";
  p.X = int ( width / 2 - 20 );
  p.Y = int ( height / 5.5f );
  SetConsoleCursorPosition( hOut, p );

  cout << "Game will be continue after two seconds";
  Sleep(2000);
  p.X = int ( width / 2 - 20 );
  p.Y = int ( height / 5.5f );
  SetConsoleCursorPosition( hOut, p );
  for(int i = 1; i<=40; i++)
  cout << " ";
}

///New Function to add timer on screen
void drawTimer(int endtime, int &you_score, int &comp_score)
{
  COORD p;
  p.X = 0;
  p.Y = height;
  SetConsoleCursorPosition( hOut, p );
  cout << "\n    Your score: " << you_score << "    Comp score: " << comp_score << "    Timer: " << endTime/1000 << "       \n";
}

///return size of pad
int paddleSize(HANDLE hOut, int x)
{
      COORD p;
      p.X = int ( width / 2 );
      p.Y = int ( 0 );
      int choice;
      SetConsoleCursorPosition( hOut, p );
      SMALL_RECT DisplayArea = {0, 0, width+20, height + 3};
      SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

    ///Menu
    while(1){
    system("cls");
    cout<<"\n\t\t\t\tPing Pong game\n";
    cout<<"\n\t\tInstruction:\n";
    cout<<"\n\t\t\tPress Left key to move paddle left";
    cout<<"\n\t\t\tPress Right key to move paddle right";
    cout<<"\n\t\t\tEvaluation of game will be after 1 minute.";

    cout<<"\n\nChoose difficulty Level\n\n\n\tPress 1 to play easy mode";
    cout<<"\n\n\tPress 2 to play normal mode";
    cout<<"\n\n\tPress 3 to play hard mode\n\n\tPress 4 to exit";
    cin>>choice;
    while(cin.fail()) {
        cout << "Please enter an integer : " << endl;
        cin.clear();
        cin.ignore(256,'\n');
        cin >> choice;
    }
    switch(choice)
    {
    case 1:
        x = 5;
        break;
    case 2:
        x = 3;
        break;
    case 3:
        x = 2;
        break;
    case 4:
        exit(0);
        break;
    default:
        cout<<"\n\nWrong Input\n\n";
        Sleep(500);
    }
    if(x == 5 || x == 2 || x == 3)
            break;
    }
    return x;

}


///main Function
int main()  {


  endTime = 0;
  int you_score = 0, comp_score = 0;
  int youPaddleSize = 0;

    hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    youPaddleSize = paddleSize(hOut, youPaddleSize);

    system("cls");

  srand( int(time(NULL)) );


  SMALL_RECT DisplayArea = {0, 0, width, height + 3};
  SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

  // Hide Console Cursor
  CONSOLE_CURSOR_INFO ConCurInf;
  ConCurInf.dwSize = 10;
  ConCurInf.bVisible = false;
  SetConsoleCursorInfo(hOut, &ConCurInf);

  // Draw border
  SetConsoleTextAttribute( hOut, 224 );
  for (int i = 0; i < width - 1; ++i)
    cout << '*';

  for (int i = 0; i < height - 1; ++i) {
    cout << "\n|";
    for (int i = 0; i < width - 3; ++i)
      cout << ' ';
    cout << "|";
  }

  cout << '\n';

  for (int i = 0; i < width - 1; ++i){
    cout << '*';
  }

  drawTimer(endTime, you_score, comp_score);

  for (int i = 0; i < width - 1; ++i)
    cout << '*';
  // End drawing border
  bool started = false;
  paddle you(width / 2, height - 2, youPaddleSize);
  paddle comp(width / 2, 2, 4);
  ball b(width / 2 - 1, height / 3);
  //int you_score = 0, comp_score = 0;
  // Ball's start velocity, always starts towards you
  b.vx = float(rand() % 3) - 1.5f;
  b.vy = 1;
  comp.vx = 0;



  while ( true ) {

    if ( started ) {
      // Arrow keys
      if ( GetAsyncKeyState(VK_LEFT) ) {
        you.x -= 2;
      } else if ( GetAsyncKeyState(VK_RIGHT) ) {
        you.x += 2;
      }

      // Move comp.x towards the ball
      comp.vx -= float(comp.x - b.x) / 10.0f;
      comp.vx *= 0.85f;

      // Apply comp's velocity to current location
      comp.x += comp.vx;

      // Add player boundaries
      if ( you.x < you.size + 1 ) {
        you.x = float ( you.size + 1 );
      } else if ( you.x > width - you.size - 2 ) {
        you.x = float ( width - you.size - 2 );
      }
      // Add comp boundaries
      if ( comp.x < comp.size + 1 ) {
        comp.x = float ( comp.size + 1 );
      } else if ( comp.x > width - comp.size - 2) {
        comp.x = float ( width - comp.size - 2 );
      }
      // Make sure ball doesn't exceed it's maximum speed
      if ( b.vx < -maxSpeed ) {
        b.vx = -maxSpeed;
      } else if ( b.vx > maxSpeed ) {
        b.vx = maxSpeed;
      }
      // Apply velocity to balls current location
      b.x += b.vx;
      b.y += b.vy;
      // Check if ball is hitting your pad
      if ( b.y >= you.y - 1 && b.y <= you.y + 1 ) {
        if (b.x > you.x - you.size && b.x < you.x + you.size) {
          b.vy *= -1;
          b.vx += float(b.x - you.x) / 3;
          b.y = you.y - 1;
        }
      }
      // Check if ball is hitting the computer pad
      if ( b.y <= comp.y + 1 && b.y >= comp.y - 1 ) {
        if (b.x > comp.x - comp.size && b.x < comp.x + comp.size) {
          b.vy *= -1;
          b.vx += float(b.x - comp.x) / 3;
          b.y = comp.y + 1;
        }
      }
      // Check if ball has gone out of boundaries (a player won/lost)
      if ( b.y >= height ) {
        ++comp_score;
        reset( you, comp, b, started, you_score, comp_score );
      } else if ( b.y <= 0 ) {
        ++you_score;
        reset( you, comp, b, started, you_score, comp_score );
      }

      if ( b.x <= 1 )
      {
        b.vx *= -1;
        b.x = 1.0f;
      }
      else if ( b.x >= width - 3 )
      {
        b.vx *= -1;
        b.x = float( width - 3 );
      }

    endTime += 2*(delay);

    }
    else {
      // Wait for user to press the space bar
      COORD p;
      p.X = int ( width / 2 -16 );
      p.Y = int ( height / 5.5f );
      SetConsoleCursorPosition( hOut, p );
      cout << "Press the space bar to start . .";
      if ( GetAsyncKeyState(VK_SPACE) ) {
        COORD p;
        p.X = int ( width / 2 - 16 );
        p.Y = int ( height / 5.5f );
        SetConsoleCursorPosition( hOut, p );
        cout << "                                 ";
        started = true;
      }
    }

    // Draw all objects
    you.draw();
    comp.draw();
    b.draw();
    drawTimer(endTime, you_score, comp_score);

    // Delay
    Sleep( delay );

    if(endTime > 120000)
    {
        system("cls");
        cout << "\n    Your score: " << you_score << "\tComp score: " << comp_score << '\n\n\n';
        int choice;
        cout<<"\n\nPress 1 to play Again or something else to exit";
        cin>>choice;

        while(cin.fail()) {
                exit(0);
        }

        switch(choice)
        {
        case 1:
            main();
            break;
        default:
            cout<<"Thanks For Playing\nAI enjoyed very much to counter with you\n\nAI wants to meet you again\n\n\n";
            exit(0);
        }
    }
  }

}
