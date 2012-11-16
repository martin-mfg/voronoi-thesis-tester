#ifndef LOGGING
#define LOGGING

/*
If you need to debug the program, you can include this function. It's a very simple Logger.
*/

inline void log(char * s) {
	FILE * file = fopen( "log.txt", "a" );
	fputs( s, file );
	fputs( "\n", file );
	fclose( file );
}

#endif
