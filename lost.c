#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

WINDOW *in, *out;
int pushed;

void scrflash(){
delwin(in);
delwin(out);
  int i, y=(int)LINES*0.5, x=(int)(COLS-4)*0.5;
  struct timespec tsf;
  tsf.tv_sec=0;tsf.tv_nsec=300000000L;
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(1) | A_BOLD);refresh();
  for(i=0;i<100;i++){
    attroff(COLOR_PAIR(1+(i%2)) | A_BOLD);
    attron(COLOR_PAIR(1+((i+1)%2)) | A_BOLD);
    bkgd(COLOR_PAIR(1+((i+1)%2)));
    beep();
    clear();
    refresh();
    nanosleep(&tsf, NULL);
  }
clear();refresh();

  tsf.tv_sec=3;tsf.tv_nsec=0L;
attron(A_BOLD);
mvprintw(y, x, "LOST");
refresh();
nanosleep(&tsf, NULL);

       out=newwin(LINES-1, 0, 0, 0);
       in=newwin(1, 0, LINES-1, 0);
       nodelay(in, TRUE);
       pushed=1;
       
}

void sig_handler(int s) {
  if(s==SIGINT){
      scrflash();
//      endwin();
//      exit(EXIT_SUCCESS);
  }
}

void countdown(int y, int x){
  int i, j,k, n,my,mx, len=0, tlen=2;
  char tmp[15], code[15], nums[]="4 8 15 16 23 42";
  struct timespec ts;
  //ts.tv_sec=1;ts.tv_nsec=0L;
  ts.tv_sec=0;ts.tv_nsec=100000000L;
  pushed=0;
  memset(code, 0, 15);
  memset(tmp, 0, 15);
  wattron(out, A_BOLD);
  // Edit This line to Reduce Countdown
  for(j=107;j>=0 && !pushed;j--){                                // MINUTE LOOP
    if(j==3){wprintw(in,":>");wrefresh(in);}        // PRINT PROMPT IF AT 4 MINS
    for(i=59;i>=0 && !pushed;i--){                             // SECOND LOOP
      if(j<=3) beep();
      for(k=9;k>=0 && !pushed;k--){
      getmaxyx(in, my, mx);
      wresize(in, my, mx);
      wrefresh(in);
      getmaxyx(out, my, mx);
      y=(int)my*0.5;
      x=(int)(mx-6)*0.5;
      wresize(out, my, mx);
      werase(out);
      wattroff(out, A_BOLD);
      mvwprintw(out, 0, 0, "DHARMA INITIATIVE");
      wattron(out, A_BOLD);
      mvwprintw(out, y, x, "% 3d:%02d", j, i);
      if(j<=3){
      n=mvwgetch(in, 0, tlen);
      if(n!=ERR){
        if(n==10){
          if (!strncmp(code, nums, 15)){
            //sig_handler(SIGINT);
            pushed=1;
            continue;
          }else {
            memset(code, 0, 15);tlen=2;
            werase(in);
            wprintw(in, ":>");
            wrefresh(in);
          }
        }
      else if((n==KEY_BACKSPACE || n==127 || n==827) && tlen>=2){
        if(tlen==2);
        else{
        tlen--;
        code[tlen-2]=0;
        werase(in);
        wprintw(in, ":>");
        wprintw(in, "%s", code);
        wrefresh(in);
        }
      }
      else{
      mvwaddch(in, 0,tlen, (char)n);
      //mvwprintw(out, 0 , 0,"KEY_BACKSPACE is %d", KEY_BACKSPACE);
      //mvwprintw(out, 1 , 0,"%d", n);
      code[tlen-2]=(char)n;
      ++tlen;
      }
    }
    }
    wrefresh(out);
    nanosleep(&ts, NULL);
    }
}
}
if(!pushed){
  scrflash();
}else {
wclear(in);wrefresh(in);
}
}

int main(){
int x, y;
struct sigaction sa;

  sa.sa_handler = sig_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

 if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    exit(1);
  } else if (sigaction(SIGINT, &sa, NULL) == -1){
      exit(1);
  }

initscr();
cbreak();
echo();
curs_set(0);
start_color();

y=(int)LINES*0.5;
x=(int)(COLS-6)*0.5;

out=newwin(LINES-1, 0, 0, 0);
in=newwin(1, 0, LINES-1, 0);
nodelay(in, TRUE);

  while(1)
    countdown(y, x);

delwin(in);
delwin(out);
endwin();


return 0;
}
