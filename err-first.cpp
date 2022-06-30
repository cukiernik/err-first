#include <unistd.h>
#include <poll.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#define ERR( t) write( STDERR_FILENO, t,sizeof( t)- 1)
#define OUT( t) write( STDOUT_FILENO, t,sizeof( t)- 1)

typedef unsigned char error;

static int Out[ 2], Err[ 2];


static int copy(pollfd& in,int out) {
    char b[ 0x1000];
    int r=read( in.fd, b,sizeof( b));
    return write( out, b, r);       }


static int reader2(void*)                           {
    close( STDIN_FILENO);
    close( Out[ 1]);
    close( Err[ 1]);
    fcntl( Out[ 0], F_SETFL, O_NONBLOCK);
    fcntl( Err[ 0], F_SETFL, O_NONBLOCK);
    pollfd Fds[ 2]={{ Out[ 0], POLLIN , POLLIN},{ Err[ 0], POLLIN, POLLIN}};
    unsigned count= 2;
    for(;;)                                       {
        error E= poll( Fds, count, -1);
        if( E<= 0)return -E;
        if( POLLIN& Fds[ 1].revents) {
            copy( Fds[ 1], STDOUT_FILENO);
            return 0;                }
        else if( POLLHUP& Fds[ 1].revents) {
            close( Fds[ 1].fd); count= 1;  }
        if( POLLIN& Fds[ 0].revents)       {
            copy( Fds[ 0], STDERR_FILENO); }
        else if( POLLHUP& Fds[ 0].revents)      {
            close( Fds[ 0].fd); Fds[ 0].fd= -1; } } }


int main(int,char** Param)               {
    pipe( Out); pipe( Err);
    static char Stack[ 0x8000];
    clone( reader2, Stack+ sizeof( Stack), 0, 0);
    dup2( Out[ 1], STDOUT_FILENO);
    dup2( Err[ 1], STDERR_FILENO);
    close( Out[ 0]);
    close( Out[ 1]);
    close( Err[ 0]);
    close( Err[ 1]);
    return execvp( Param[ 1], Param+ 1); }
