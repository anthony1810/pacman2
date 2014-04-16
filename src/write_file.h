#ifndef WRITE_FILE_H
#define WRITE_FILE_H

/*! @file write_file.h 
	@brief this is the header file of \ref write_file.c.
	@author Tran Nhat Quang <s3312399@rmit.edu.vn> 
*/

#define MAX_NAME_SZ 256

/*! \brief take int type of ncurses character and translate it to normal char in order to write to text file.
 *
 *	\param ch the int type of ncurses character
 *	\return the a char character type
 *
 *  \code 
 		if(ch == 4194912){
			return 'p';
		}
	\endcode
*/
char getTranslatedChar(int ch);

/*! \brief actual function which handle write to text file
 *
 *  \details loop through an ncurse window with specific row and col to read all characters appear in this window, translate 
 * these individual characters to normal char and write down to text file.
 *
 * \param file_name the name of the file to write to, overwrite content on exist and create when not found
 * \param author_name the name of the author who create the map
 * \param author_email the email of the author who create the map
 * \param my_win the ncurses window which will be targeted
 * \param map_col the collumn length of the map
 * \param map_row the row length of the map
 *
 *  \code 
		int characters = (int)mvwinch(my_win, i, k);
		fputc(getTranslatedChar(characters), fp);
	\endcode
*/

void write_to_file(char file_name[],char author_name[], char author_email[], WINDOW *my_win,int map_col, int map_row);


#endif