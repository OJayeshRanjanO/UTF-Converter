#ifndef UTFCONVERTER_H
#define UTFCONVERTER_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <struct.txt>

#define MAX_BYTES 4
#define SURROGATE_SIZE 4
#define NON_SURROGATE_SIZE 2
#define NO_FD -1
#define OFFSET 2

#define FIRST  0
#define SECOND 1
#define THIRD  2
#define FOURTH 3

#ifdef __STDC__
#define P(x) x
#else
#define P(x) ()
#endif

/** The enum for endianness. */
typedef enum {LITTLE, BIG,EIGHT} endianness;

/** The struct for a codepoint glyph. */
typedef struct Glyph {
	unsigned char bytes[MAX_BYTES];
	endianness end;
	bool surrogate;
} Glyph;

int outputINODE = 0;
int inputINODE = 0;

/** The usage statement. */
char* USAGE[12] = {
	"Command line utility for converting files from UTF-16LE to UTF-16BE or vice versa.\n\n",
	"Usage:\n",
	"  ./utf [-h|--help] -u OUT_ENC | --UTF=OUT_ENC IN_FILE [OUT_FILE]\n\n",
	"  Option arguments:\n",
	"    -h, --help\t\tDisplays this usage.\n",
	"    -v, -vv\t\tToggles the verbosity of the program to level 1 or 2.\n\n",
	"  Mandatory argument:\n",
	"    -u OUT_ENC, --UTF=OUT_ENC\tSets the output encoding.\n",
	"                                  Valid values for OUT_ENC: 16LE, 16BE\n\n",
	"  Positional Arguments:\n",
	"    IN_FILE\t    The file to convert.\n",
	"    [OUT_FILE]\t    Output file name. If not present, defaults to stdout.\n"};

/** Which endianness to convert to. */
	endianness conversion;

/** Which endianness the source file is in. */
	endianness source;

/** Struct we are using to write glyphs to file. */
	Glyph* glyph;

/** buffer for bytes */
	unsigned char buf[4] = {0}; 



/** File pointer for creating files*/
	FILE * fptr;

	int VFLAG = 0;

	double asciiCounter = 0;

	double surrogateCounter = 0;

	int glyphCounter = 0;

	int rv = 1;

	double realRead = 0;
	double sysRead = 0;
	double userRead = 0;

	double realConvert = 0;
	double sysConvert = 0;
	double userConvert = 0;

	double realWrite = 0;
	double sysWrite = 0;
	double userWrite = 0;



/**
 * A function that swaps the endianness of the bytes of an encoding from
 * LE to BE and vice versa.
 *
 * @param glyph The pointer to the glyph struct to swap.
 * @return Returns a pointer to the glyph that has been swapped.
 */
 Glyph* swap_endianness P((Glyph*));

/**
 * Fills in a glyph with the given data in data[2], with the given endianness 
 * by end.
 *
 * @param glyph 	The pointer to the glyph struct to fill in with bytes.
 * @param data[2]	The array of data to fill the glyph struct with.
 * @param end	   	The endianness enum of the glyph.
 * @param fd 		The int pointer to the file descriptor of the input 
 * 			file.
 * @return Returns a pointer to the filled-in glyph.
 */
 Glyph* fill_glyph P((Glyph*, unsigned char[], endianness, int*));

/**
 * Writes the given glyph's contents to stdout.
 *
 * @param glyph The pointer to the glyph struct to write to stdout.
 */
 void write_glyph P((Glyph*,int));

/**
 * Calls getopt() and parses arguments.
 *
 * @param argc The number of arguments.
 * @param argv The arguments as an array of string.
 */
 void parse_args P((int, char**));

/**
 * Prints the usage statement.
 */
 void print_help P((void));

/**
 * Closes file descriptors and frees list and possibly does other
 * bookkeeping before exiting.
 *
 * @param The fd int of the file the program has opened. Can be given
 * the macro value NO_FD (-1) to signify that we have no open file
 * to close.
 */
 void quit_converter P((int, int));

 void write_verbosity P((char*));

 Glyph* convert P((Glyph*,endianness));

 Glyph* fill_glyph_utf8 P((Glyph*, unsigned char [],endianness,int*));

 void quit(int i);

 void startReading();

 void stopReading();

 void startConverting();

 void stopConverting();

 void startWriting();

 void stopWriting();

 int reader(int fd,unsigned char array[2],int counter);

 void write_glyph_reverse(Glyph* glyph,int fd, int bytes);

 void convert_reverse(Glyph* glyph, endianness end);
#endif