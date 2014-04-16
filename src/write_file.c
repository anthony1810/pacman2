#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "write_file.h"
#include "constant.h"

/*! @file write_file.c 
	@brief this file provide functions to handle writing to text file.
	@author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/
	
void write_to_file(char file_name[],char author_name[], char author_email[], WINDOW *my_win, int width, int height){
	
	FILE *fp;
	char full_path[100]="" ;

	strcat(full_path,PATH);
 	strcat(full_path,file_name);
	strcat(full_path,EXTENSION);
	fp=fopen(full_path, "w");


	fputs(author_name,fp);
	fputs(" <",fp);
	fputs(author_email, fp);
	fputs(">",fp);
	fputc('\n',fp);


	fputs(file_name,fp);
	fputc('\n',fp);


	fprintf(fp, "%d", height);
	fputc('\n',fp);

	fprintf(fp, "%d", width);
	fputc('\n',fp);

	int i;
	int k;

	for(i=0; i<height; i++){
		for(k=0;k<width;k++){
			int characters = (int)mvwinch(my_win, i, k);
			fputc(getTranslatedChar(characters), fp);
			if(i==1&&k==4){
			}
		}
		printw("\n");
		fputc('\n', fp);
		refresh();	
	}
	fputc('\n',fp);
	fclose(fp);
}


char getTranslatedChar(int ch){
	//pacman
	if(ch == 4194912){
		return 'p';
	}
	//ghost
	else if(ch == 4195169){
		return 'g';
	}
	// upper left corner
	else if(ch == 4194668){
		return 'q';
	}
	// lower left corner
	else if(ch == 4194669){
		return 'z';
	}
	// upper right corner
	else if(ch == 4194667){
		return 'e';
	}
	// lower right corner
	else if(ch == 4194666){
		return 'c';
	}
	// right T line
	else if(ch == 4194677){
		return 'D';
	}
	// left T line
	else if(ch == 4194676){
		return 'A';
	}
	// top T line
	else if(ch == 4194679){
		return 'W';
	}
	// bottom T line
	else if(ch == 4194678){
		return 'X';
	}
	// Horizontal line
	else if(ch == 4194673){
		return 'w';
	}
	// vertical line
	else if(ch == 4194680){
		return 'a';
	}
	// pelet
	else if(ch == 4195710){
		return 's';
	}
	// power pellet
	else if(ch == 4195686){
		return 'S';
	}
	// fruit
	else if(ch == 4195453){
		return 'f';
	}
	// space
	else
		return 32;
}


