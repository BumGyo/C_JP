#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

typedef int bool;
#define true 1
#define false 0

typedef struct {
    bool use_n;     
    bool use_b;     
    long h_start;   
    long t_end;     // -1 = 無制限
} Opts;

static void print_usage(const char *prog){
    fprintf(stderr,
      "Usage: %s [-n|-b] [-h<num>] [-t<num>] [file ...]\n"
      "  -n      すべての行に通し番号\n"
      "  -b      空行を除いて番号（範囲内では h から連番）\n"
      "  -h<num> 開始行, -t<num> 終了行\n", prog);
}

static int parse_pos_long(const char *s, long *out){
    if(!s || !*s) return 0;
    char *e=NULL; long v=strtol(s,&e,10);
    if(*e!='\0' || v<1 || v==LONG_MAX || v==LONG_MIN) return 0;
    *out=v; return 1;
}

// 1ストリーム処理: 物理行(phys)で範囲を判定、-b は論理番号(vln)を範囲内で h から開始
static int process_stream(FILE *fp, const Opts *opt, long *p_phys, long *p_vln, int *p_done){
    int ch;
    long phys = *p_phys;    // 物理行
    long vln  = *p_vln;     // 論理番号
    int bol = 1;            // 行頭
    int numbered_this_line = 0;

    while((ch=fgetc(fp)) != EOF){
        if(opt->t_end > 0 && phys > opt->t_end){ *p_done = 1; break; }

        int in_range = (phys >= opt->h_start) && (opt->t_end <= 0 || phys <= opt->t_end);

        if(bol){
            numbered_this_line = 0;
            // -n かつ -bなし → 行頭で物理行番号を出す
            if(in_range && opt->use_n && !opt->use_b){
                if(printf("%ld ", phys) < 0) return -1;
                numbered_this_line = 1;
            }
            bol = 0;
        }

        // -b: 範囲内の最初の非空行で vln を初期化(h_start)し、その行に vln を出力して vln++。
        if(in_range && opt->use_b && !numbered_this_line && ch!='\n'){
            if(vln == 0) vln = opt->h_start;    // 範囲に初めて入った時点
            if(printf("%ld ", vln) < 0) return -1;
            vln++;
            numbered_this_line = 1;
        }

        // 範囲内なら文字を出力、範囲外は読み捨て
        if(in_range){
            if(fputc(ch, stdout) == EOF) return -1;
        }

        if(ch == '\n'){
            phys++;
            bol = 1;
        }
    }

    if(ferror(fp)) return -1;
    *p_phys = phys;
    *p_vln  = vln;
    return 0;
}

static int is_option(const char *s){ return s && s[0]=='-' && s[1]!='\0'; }

int main(int argc, char *argv[]){
    Opts opt = { .use_n=false, .use_b=false, .h_start=1, .t_end=-1 };
    const char *files[argc]; int file_count=0;

    // 引数解析（-b は -n より優先）
    for(int i=1;i<argc;++i){
        const char *a = argv[i];
        if(is_option(a)){
            if(strcmp(a,"-n")==0){ opt.use_n=true; continue; }
            if(strcmp(a,"-b")==0){ opt.use_b=true; continue; }
            if(strncmp(a,"-h",2)==0){ 
            	long v; 
            	if(!parse_pos_long(a+2,&v)){ 
            		fprintf(stderr,"mycat: invalid -h: '%s'\n",a); 
            		print_usage(argv[0]); 
            		return 2; 
            	} 
            	opt.h_start=v; 
            	continue; 
            }
            
            if(strncmp(a,"-t",2)==0){ 
            	long v; 
            	if(!parse_pos_long(a+2,&v)){ 
            		fprintf(stderr,"mycat: invalid -t: '%s'\n",a); 
            		print_usage(argv[0]); 
            		return 2; 
            	} 
            	opt.t_end=v; 
            	continue; 
            }
            
            fprintf(stderr,"mycat: unknown option: '%s'\n",a); 
            print_usage(argv[0]); return 2;
        } else files[file_count++] = a;
    }
    if(opt.t_end>0 && opt.h_start>opt.t_end){
        fprintf(stderr,"mycat: invalid range: -h%ld > -t%ld\n", opt.h_start, opt.t_end);
        print_usage(argv[0]); return 2;
    }

    long phys=1;    // 物理行
    long vln =0;    // 論理番号
    int done=0, exit_code=0;

    if(file_count==0){
        if(process_stream(stdin,&opt,&phys,&vln,&done)!=0){ 
        	fprintf(stderr,"mycat: error reading from stdin\n"); 
        	return 1; 
        }
        return 0;
    }
    for(int i=0;i<file_count && !done;++i){
        FILE *fp=fopen(files[i],"rb");
        if(!fp){ 
        	fprintf(stderr,"mycat: cannot open '%s': %s\n", files[i], strerror(errno)); 
        	exit_code=1; 
        	continue; 
        }
        if(process_stream(fp,&opt,&phys,&vln,&done)!=0){ 
        	fprintf(stderr,"mycat: error while reading '%s'\n", files[i]); 
        	exit_code=1; 
        }
        fclose(fp);
    }
    return exit_code;
}

