#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CHUNK 1024

void print_help();
size_t file_size(FILE *f);
void open_files(const char *filename1, const char *filename2,
		FILE **f_smaller, FILE **f_bigger,
		size_t *f_s_s, size_t *f_b_s);
int main(int argc, char **argv){

	if(argc!=3){
		print_help(); 
		exit(EXIT_FAILURE);
	}
	size_t f_s_s,f_b_s;

	FILE *f_smaller;
	FILE *f_bigger;
	open_files(argv[1],argv[2],
		   &f_smaller,&f_bigger,
		   &f_s_s,&f_b_s);

	size_t fl=f_s_s<CHUNK;
	size_t len=fl?f_s_s:CHUNK;
	size_t i,j;
	unsigned char *b_s=malloc(len*sizeof(unsigned char));
	unsigned char *b_b=malloc(len*sizeof(unsigned char));
	unsigned char *r=malloc(len*sizeof(unsigned char));
	size_t s_d=0,b_d=0;
	size_t l_len=len;
	int s,b;


	FILE *f=fopen("x.zip","wb");
	
	while(b_d<f_b_s){
	  fseek(f_bigger, b_d, SEEK_SET); 
	  fseek(f_smaller, b_d, SEEK_SET); 
	  fread(b_s,sizeof(unsigned char),l_len,f_smaller);
	  fread(b_b,sizeof(unsigned char),l_len,f_bigger);

	  for(i=0;i<l_len;i++){
	    s=b_s[i];
	    b=b_b[i];
	    r[i]=s^b;
	  }

	  //fseek(f_smaller, 0, SEEK_SET); 

	  if(b_d+l_len>f_b_s){
	    l_len=f_b_s-b_d;
	  }
	  fwrite(r,sizeof(char),l_len,f);	

	  b_d+=len;
	  s_d+=len;
	}
  
	return(0);
}
void print_help(){
	printf("\n    Usage: xoror file1 file2");
}
size_t file_size(FILE *f){
	size_t s;
	fseek(f, 0, SEEK_END);
	s = ftell(f); 
	fseek(f, 0, SEEK_SET); 
	return s;
}
void open_files(const char *filename1, const char *filename2,
		FILE **f_smaller, FILE **f_bigger,
		size_t *f_s_s, size_t *f_b_s
		){
	*f_smaller=fopen(filename1,"rb");
	*f_bigger=fopen(filename2,"rb");
	if(!*f_smaller){
		fprintf(stderr,"Check input file: %s\n", filename1);
		exit(EXIT_FAILURE);
	}
	if(!*f_bigger){
		fprintf(stderr,"Check input file: %s\n", filename2);
		exit(EXIT_FAILURE);
	}
	*f_s_s=file_size(*f_smaller);
	*f_b_s=file_size(*f_bigger);


	if(!*f_s_s){
		fprintf(stderr,"Input file is empty: %s\n", filename1);
		exit(EXIT_FAILURE);
	}
	if(!*f_b_s){
		fprintf(stderr,"Input file is empty: %s\n", filename2);
		exit(EXIT_FAILURE);
	}

	
	if (*f_s_s>*f_b_s) {
		FILE *f; 
		size_t n;
		f=*f_smaller;
		*f_smaller=*f_bigger;
		*f_bigger=f;
		n=*f_s_s;
		*f_s_s=*f_b_s;
		*f_b_s=n;
	}
}

