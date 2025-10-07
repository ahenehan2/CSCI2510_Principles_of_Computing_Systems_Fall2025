// rle.c - Run Length Encode/Decode using Linux syscalls
// Usage: rle <input> <output> <chunk size K> <mode>
// mode = 0 -> compress, 1 -> decompress

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <sys/stat.h>   // file perms
#include <errno.h>      // errno
#include <stdio.h>      // perror, fprintf
#include <stdlib.h>     // strtol, exit
#include <string.h>     // memcmp, memcpy
#include <stdint.h>     // uint8_t
#include <limits.h>

static void die_usage(void){
    fprintf(stderr,"Usage: rle <input file> <output file> <compression length> <mode>\n");
    exit(-1);
}
static void die_perror(const char *m){ perror(m); exit(-1); }

// write everything (handles partial writes)
static int write_all(int fd, const void *buf, size_t n){
    const uint8_t *p=(const uint8_t*)buf; size_t t=0;
    while(t<n){
        ssize_t w=write(fd,p+t,n-t);
        if(w<0){ if(errno==EINTR) continue; return -1; }
        t+=(size_t)w;
    }
    return 0;
}

// read once (ok if short at EOF)
static ssize_t read_up_to(int fd, void *buf, size_t n){
    for(;;){
        ssize_t r=read(fd,buf,n);
        if(r<0 && errno==EINTR) continue;
        return r;
    }
}

// -------- compression --------
static int compress_fd(int in_fd,int out_fd,size_t K){
    if(K==0){ fprintf(stderr,"Compression length must be >= 1\n"); return -1; }

    uint8_t *prev=malloc(K), *curr=malloc(K);
    if(!prev||!curr){ fprintf(stderr,"Out of memory\n"); free(prev); free(curr); return -1; }

    ssize_t prev_len=read_up_to(in_fd,prev,K); // first chunk
    if(prev_len<0){ free(prev); free(curr); return -1; }
    if(prev_len==0){ free(prev); free(curr); return 0; } // empty input

    int count=1;
    for(;;){
        ssize_t curr_len=read_up_to(in_fd,curr,K);
        if(curr_len<0){ free(prev); free(curr); return -1; }

        int same=(curr_len==prev_len && curr_len>0 && memcmp(prev,curr,(size_t)curr_len)==0);
        if(same && count<255){ count++; continue; }

        // flush run
        uint8_t cbyte=(uint8_t)count;
        if(write_all(out_fd,&cbyte,1)<0){ free(prev); free(curr); return -1; }
        if(write_all(out_fd,prev,(size_t)prev_len)<0){ free(prev); free(curr); return -1; }

        if(curr_len==0) break; // EOF

        memcpy(prev,curr,(size_t)curr_len);
        prev_len=curr_len; count=1;
    }
    free(prev); free(curr); return 0;
}

// -------- decompression --------
static int decompress_fd(int in_fd,int out_fd,size_t K){
    if(K==0){ fprintf(stderr,"Compression length must be >= 1\n"); return -1; }
    uint8_t *pattern=malloc(K); if(!pattern){ fprintf(stderr,"Out of memory\n"); return -1; }

    for(;;){
        uint8_t cbyte; ssize_t r=read_up_to(in_fd,&cbyte,1); // count
        if(r<0){ free(pattern); return -1; }
        if(r==0) break; // EOF cleanly

        int count=(int)cbyte;
        if(count<1){ fprintf(stderr,"Malformed input: zero count\n"); free(pattern); return -1; }

        ssize_t got=read_up_to(in_fd,pattern,K); // pattern (may be <K at end)
        if(got<=0){ fprintf(stderr,"Malformed input: missing pattern\n"); free(pattern); return -1; }

        for(int i=0;i<count;i++){
            if(write_all(out_fd,pattern,(size_t)got)<0){ free(pattern); return -1; }
        }
    }
    free(pattern); return 0;
}

int main(int argc,char **argv){
    if(argc!=5) die_usage();

    const char *in_path=argv[1], *out_path=argv[2];
    char *end=NULL; errno=0;
    long K_long=strtol(argv[3],&end,10);
    if(errno||end==argv[3]||K_long<=0||K_long>INT_MAX){
        fprintf(stderr,"Invalid compression length: %s\n",argv[3]);
        exit(-1);
    }
    size_t K=(size_t)K_long;

    errno=0; long mode_long=strtol(argv[4],&end,10);
    if(errno||end==argv[4]||(mode_long!=0 && mode_long!=1)){
        fprintf(stderr,"Invalid mode (0 or 1): %s\n",argv[4]);
        exit(-1);
    }
    int mode=(int)mode_long;

    int in_fd=open(in_path,O_RDONLY);
    if(in_fd<0) die_perror("open input");
    int out_fd=open(out_path,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if(out_fd<0){ int s=errno; close(in_fd); errno=s; die_perror("open output"); }

    int rc=(mode==0)?compress_fd(in_fd,out_fd,K):decompress_fd(in_fd,out_fd,K);

    if(close(in_fd)<0){ int s=errno; close(out_fd); errno=s; die_perror("close input"); }
    if(close(out_fd)<0){ die_perror("close output"); }

    if(rc<0) exit(-1);
    return 0;
}
