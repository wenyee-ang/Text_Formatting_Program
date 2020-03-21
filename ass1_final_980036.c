/* comp10002 Assignment 1, 2019 semester 2. Text editor program.
WEN YEE ANG
Dated: 13/09/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEBUG 0
#define MAXCHAR 999 /*max number of characters per input line*/
#define MAXLINE 999 /*max number of lines from input*/
#define MAXTEXT MAXCHAR*MAXLINE
#define MAXSPACES 50
#define MAXNUM 3
#define MAXLEVEL 5
#define CHARLINE 50 /*max number of characters per output line*/
#define NUMSPACES 4
#define NEXTLINE 1
#define ENDOFFILE 0

int mygetchar();
int make_para(char *A, int j);
int make_space(char *A, int j, int numspaces);
int readline(char *A, int max);
int sortline(char *A, char *B, int *charperline, int *lspaces, int max);
int check_end_line(char *A, char *B, int *m, int *j, int maxchar, int spaces);
void check_newlines(char *A, char *B, int *m, int *j, int *c);
int command_b_p(char command, char *B, int j, int spaces);
int command_w_l(char command, char *A, int m, int *maxchar, int *startspaces);
void command_c(char *A, int m,  int *maxchar, int *startspaces);
int command_h(char *B, int j, int maxchar, int spaces, int h, int *H);


int
main(int argc, char *argv[]) {
	char input[MAXTEXT], lines[MAXTEXT];
	int continue_read=1, continue_sort=1, s=0;
	int charperline=CHARLINE, lspaces=NUMSPACES;

	/*read in text line by line*/
	while (continue_read){
		continue_read=(readline(input, MAXTEXT));
	}
	if (DEBUG){
		printf("%s\n",input);
	}
  /*set initial margin*/
	make_space(lines, s, NUMSPACES);
  /*process lines and execute commands*/
	while (continue_sort){
		continue_sort=(sortline(input, lines, &charperline, &lspaces, MAXTEXT));
	}
	if (DEBUG){
		printf("0------------------------------------------------50---\n");
		printf("sorted lines are:\n%s\n",lines);
		printf("0------------------------------------------------50---\n");
	}else{
		printf("%s",lines); /*print to output*/
	}
	return (0);
}


/*main processing function which processes input and command lines*/
int
sortline(char *A, char *B, int *charperline, int *lspaces, int max){
	/*A is input lines, B is output lines*/
	/*static variables track pointers as the function is returned and iterated*/
	/*m is index of main array, j is index of sorted array*/
	static int m=0, j=0, num_h_times=0;
	int c=0, i=0, consecutive=0, valid=0, com_wl=0, h_num=0;
	char command=0;
	int H[MAXLEVEL]; /*array for heading numbers*/
	/*making copies of the variables*/
	int maxchar = *charperline, startspaces = *lspaces;

	if (DEBUG){
		printf("current startspace=%d, current maxchar=%d\n",startspaces,maxchar);
	}
	if (j<startspaces){ /*set starting index*/
		j=startspaces;
	}

	while (c<maxchar && j<max-1){
		/*if end of input array is reached*/
		if (A[m]=='\0'){
			if (j>0){
				j--;
				for (;B[j]!='\n';j--){
					/*go back looking for final newline*/
				}
			}
			B[j]='\0'; /*close array*/
			if (DEBUG){
				printf("Finished sorting.\n");
			}
			return ENDOFFILE;
		}

	  /*while line starts with a fullstop*/
		while ((A[m]=='\n' && A[m+1]=='.')||(m==0 && A[m]=='.')){
			if (DEBUG){
				printf("FUllSTOP found! at %c\n",A[m-1]);
			}
			if (m==0){
				m++;
			}else{
				m=m+2;
			}
			command=A[m];
			if (DEBUG){
				printf("command is %c\n", command);
			}

			if (command=='c'){
				if (c>0&&(isalnum(B[j-1])||ispunct(B[j-1]))){
					B[j++]='\n';
				}
				/*centering process*/
				command_c(A, m, &maxchar, &startspaces);
				j=(make_space(B, j, startspaces));
				if (DEBUG){
					printf("Found c at %c\n",A[m+2]);
				}
				m++;
				while(A[m]!='\n'){ /*B[j]includes newline*/
					B[j++]=A[m++];
				}
				B[j++]='\n';
				consecutive=0;
				com_wl=0;
				valid=1;

			}else if (command=='l'||command=='w'){
				m=command_w_l(command, A, m, &maxchar, &startspaces);
				if (DEBUG){
					printf("new startspace=%d, new maxchar=%d\n",startspaces,maxchar);
				}
				/*alter left margin and width values*/
				*charperline=maxchar;
				*lspaces=startspaces;
				consecutive=0;
				com_wl=1;
				valid=1;

			}else if ((command=='b'||command=='p')&&!consecutive){
				if (DEBUG){
					printf("found command %c\n", command);
				}
				j=command_b_p(command, B, j, *lspaces);
				consecutive=1;
				com_wl=0;
				valid=1;

			}else if (command=='h'){
				h_num=(A[m+2]-'0');
				if (DEBUG){
					printf("h_num=%d\n",h_num);
				}
				num_h_times++;
				/*initialize heading array to zero for first time*/
				if (num_h_times==1){
					for (i=0;i<MAXLEVEL;i++){
						H[i]=0;
					}
				}
				j=command_h(B, j, maxchar, *lspaces, h_num, H);
				m=m+3;
				while (A[m]!='\n'){
					B[j++]=A[m++];
				}
				j=make_para(B, j);
				com_wl=0;
				consecutive=0;
				valid=1;
			}

			for(;A[m]!='\n';m++){
			/*keep moving until reaches next newline*/
			}
		}
	/*fullstop while loop exits when A[m+1] != .*/
	if (valid){/*final processing of command lines*/
		if (com_wl&&c>0){
			j=make_para(B, j);
		}else if (com_wl&&c==0){
			B[j++]='\n';
		}
		if (B[j-1]=='\n'){
			j=(make_space(B, j, *lspaces));
		}
		return NEXTLINE;
	}

	/*check for extra newlines and insert rest of characters into input array*/
	check_newlines(A, B, &m, &j, &c);
 }
 /*while loop exits here when no. of characters>max. characaters per line*/

 if(j<max-1){
 /*find the correct end of the line*/
	if (check_end_line(A, B, &m, &j, maxchar, *lspaces)==1){
		return NEXTLINE;
	}
 }

 if (j==max-1){ /*if j exceeds max array size*/
	if (j>0){
		j--;
		for (;B[j]!='\n'&&B[j]!=' ';j--){
		}
	}
	B[j]='\0';
	return ENDOFFILE;
 }
	return NEXTLINE;
}


/*function readline reads each input line till /n,
discards white spaces, stores into array*/
int
readline(char *A, int max){
	if (DEBUG){
		printf("Enter strings!\n");
	}
	int c;
	static int i=0;
	/*static variable tracks pointer as the function iterates*/

	/*while the end of the line is not reached*/
	while ((c=mygetchar())!='\n'&&i<max-1){
		/*if the end of the input is reached*/
		if (c==EOF){
			A[i]='\0';
			if (DEBUG){
				printf("Final index is %d\n", i);
			}
			return ENDOFFILE;
		}

		/*if there are multiple space/tab characters after a character*/
		if (((c==' ')||(c=='\t')) && A[i-1]==' '){
			/*do nothing*/
		}else if (c=='\t'){
			/*replace first tab after the end of a word with a space*/
			A[i]=' ';
			if (DEBUG){
				printf("%c",A[i]);
			}
			i++;
		}else{
			A[i]=c;
			if (DEBUG){
				printf("%c",A[i]);
			}
			i++;
		}
	}
/*while loop exits when c reaches the end of the input line*/
	if (i==max-1){/*if max size of array is exceeded*/
		A[i]='\0';
		return ENDOFFILE;
	}else{
		A[i]='\n';
		i++;
		return NEXTLINE;
	}
}


/*function checks for multiple newlines,
and adds all other characters into output array*/
void
check_newlines(char *A, char *B, int *m, int *j, int *c){
/*if at start of the line*/
	if(*c==0&&(A[*m]==' '||A[*m]=='\n')){
		*m+=1;/*move on to next character in main array*/

/*if there are multiple newlines*/
	}else if((A[*m]=='\n')&&(A[*m+1]=='\n'||A[*m+1]==' ')){
		*m+=1;

/*special case where line immediately after a \n is a ' ' or \n */
	}else if((A[*m]=='\n'||A[*m]==' ') && (B[*j-1]==' '||B[*j-1]=='\n')){
		*m+=1;

/*replace all single newlines with spaces, move on to next empty space*/
	}else if(A[*m]=='\n'){
		B[*j]=' ';
		*j+=1;
		*m+=1;
		*c+=1;/*increase character count*/

/*for all other characters, insert into output array*/
	}else{
		B[*j]=A[*m];
		*j+=1;
		*m+=1;
		*c+=1;
	}
	return;
}


/*function checks if output line ends in the middle of a word,
goes back to start of the word, and adds newlines*/
int
check_end_line(char *A, char *B, int *m, int *j, int maxchar, int spaces){
	int n=0;
	/*if the loop ends in the middle of a word*/
	if (isalnum(A[*m])||ispunct(A[*m])){
		if (DEBUG){
			printf("A[*m]=%c\n",A[*m]);
			printf("starting n=%d\n",n);
		}
		/*go backwards looking for beginning of word*/
		while ( (B[*j-1]!=' ') && (n!=maxchar)){
			*j-=1;
			n++;
			if (DEBUG){
				printf("B[*j-1]=%c,n=%d\n",B[*j-1],n);
			}
		}
	/*loop exits when n==maxchar||B[j-1]==' '*/
		if (DEBUG){
			printf("end n=%d\n",n);
		}
	  /*dealing with token that is more than max characters per line*/
		if (n==maxchar){
			if (DEBUG){
				printf("max number of characters exceeded.\n");
			}
			*j=*j+n;
			while((A[*m]!=' ')&&(A[*m]!='\n')&&*j<MAXTEXT-1){
				B[*j]=A[*m];
				*j+=1;
				*m+=1;
			}
			/*loop exits when A[m]==' ' or \n*/
			B[*j]='\n';
			*j+=1;
			*m+=1;
			*j=(make_space(B, *j, spaces));
			return NEXTLINE;
  	/*all cases where length of token less than maxchar*/
		}else{
			if (DEBUG){
				printf("B[j-1]=%c\n",B[*j-1]);
			}
			B[*j-1]='\n';
			*j=(make_space(B, *j, spaces));
			*m-=n; /*return main array pointer to start of word after new /n*/
			return NEXTLINE;
		}
/*for cases where the maxchar loop ends after a word*/
	}else{
		if (DEBUG){
			printf("A[*m]=%c\n",A[*m]);
			printf("B[*j]=%c\n",B[*j]);
		}
		B[*j]='\n';
		*j+=1;
		*j=(make_space(B, *j, spaces));
		return NEXTLINE;
	}
	return NEXTLINE;
}


/*function to process .w and .l command lines*/
int
command_w_l(char command, char *A, int m, int *maxchar, int *startspaces){
	int N[MAXNUM], num, complete_num=0;
	/*initialize all elements of array to 0*/
	for (num=0;num<MAXNUM;num++){
		N[num]=0;
	}
	m=m+2;
	num=0;
	/*converting numbers from char to int*/
	while (isdigit(A[m])){
		if (DEBUG){
			printf("command numbers are: %c\n", A[m]);
		}
		N[num++]=(A[m++]-'0');
	}
	if (num>2){
		complete_num=(100*N[0])+(10*N[1])+(N[0]);
	}else if (num>1){
		complete_num=(10*N[0])+(N[1]);
	}else{
		complete_num=N[0];
	}

	if (DEBUG){
		printf("complete_num=%d\n",complete_num);
	}

	if (command == 'w'){
		/*change the line width(maxchar)*/
		*maxchar = complete_num;
		if (DEBUG){
			printf("*maxchar=%d\n",*maxchar);
		}
	}
	if (command == 'l'){
		/*change the margin width (startspaces)*/
		*startspaces = complete_num;
		if (DEBUG){
			printf("*startspaces=%d\n",*startspaces);
		}
	}
	return m;
}


/* function to process .b and .p command lines*/
int
command_b_p(char command, char *B, int j, int spaces){
	/*if first line*/
	if (j==spaces){
		B[j++]='\n';
		return j;
	}

	/*if command preceeded by ' '*/
	if (B[j-1]==' '){
		if (B[j-spaces-1]=='\n'&&B[j-spaces-2]=='\n'){
			/*do nothing*/
			return j;
		}else if (command=='p' && B[j-spaces-1]=='\n'){
				B[j++]='\n';
				return j;
		}else if (command=='p'){
			j=make_para(B,j);
			return j;
		}
 }

	/*if command preceeded by \n*/
	if ((B[j-1]=='\n')&&B[j-2]=='\n'){
		return j;
	}else if (B[j-1]=='\n'){
		if (DEBUG){
			printf("command p\n");
		}
		if (command=='p'){
			B[j++]='\n';
		}
	}else{
		if (command=='b'){
			B[j++]='\n';
		}else if(command=='p'){
			j=make_para(B,j);
		}
	}

	return j;
}


/*function for centering process*/
void
command_c(char *A, int m,  int *maxchar, int *startspaces){
	int nchar=-1;
	while (A[m]!='\n'&&A[m]!='\0'){
				nchar++; /*increment num of characters to be centered*/
				m++;
	}/*loop exits when A[m]==/n*/

	/*calculating number of left margin spaces*/
	*startspaces=((*maxchar-nchar)/2);

	if (DEBUG){
		printf("spaces for c is %d\n", *startspaces);
	}
	return;
}


/*function for creating headings*/
int
command_h(char *B, int j, int maxchar, int spaces, int h, int *H){
	static int last_h=0;
	/*use static variable to track last header number as function iterates*/
	int i=0;

	if (last_h<h){ /*if the header numbers increase*/
		last_h=h; /*set as new header number*/
	}else{
		for(i=h;i<MAXLEVEL;i++){
			H[i]=0; /*reset to 0 if h<last h*/
		}
	}

	if (h>0 && h<MAXLEVEL+1){
		H[(h-1)]++; /*increment relevant subheader number by 1*/
		if (DEBUG){
			printf("h_numin=%d\n",h);
			printf ("H[h-1]=%d\n",H[h-1]);
		}
	}else{
		return j;
	}

	if (j==spaces){ /*if is first line*/
		B[j++]='\n';
		j=(make_space(B, j, spaces));

	}else{
		if((B[j-1]=='\n')&&(B[j-2]=='\n')){
		/*don't do anything if there is already a paragraph*/
		}else if(B[j-1]=='\n'){
			B[j++]='\n';
		}else if(B[j-1]!='\n'&&B[j-1]!=' '){
			j=make_para(B,j);
		}
		if (B[j-1]==' '){
			if (B[j-spaces-1]=='\n'&&B[j-spaces-2]=='\n'){
				/*do nothing if there is already a paragraph*/
			}else{
				B[j++]='\n';
				j=(make_space(B, j, spaces));
			}
		}else{
			j=(make_space(B, j, spaces));
		}
	}

	if (h==1){
		/*add dashes for h 1*/
		for (i=0;i<maxchar;i++){
			B[j++]='-';
		}
		B[j++]='\n';
		j=(make_space(B, j, spaces));
	}

	for(i=0;i<h;i++){
		if (i>0&&i<MAXLEVEL){
			B[j++]='.';
		}
		/*header numbers added to output array as char*/
		B[j++]=(H[i]+'0');
	}

	return j;
}


/*function for creating blank spaces for margin*/
int
make_space(char *A, int j, int numspaces){
	int a;
	for (a=0;a<numspaces;a++){
		A[j++]=' ';
	}
	return j;
}


/*function for creating paragraphs*/
int
make_para(char *A, int j){
	A[j++]='\n';
	A[j++]='\n';
	return j;
}


/*mygetchar() function from FAQ page*/
int
mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}
