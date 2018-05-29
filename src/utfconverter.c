#include "utfconverter.h"
static clock_t st_read_time;
static clock_t en_read_time;
static struct tms st_read_cpu;
static struct tms en_read_cpu;

static clock_t st_convert_time;
static clock_t en_convert_time;
static struct tms st_convert_cpu;
static struct tms en_convert_cpu;

static clock_t st_write_time;
static clock_t en_write_time;
static struct tms st_write_cpu;
static struct tms en_write_cpu;


struct stat in;
struct stat out;
bool exists = true;
/** The given filename. */
char* filename;

/** The output filename. */
char* output;

/** fd for IN_FILE */
int readfd = 0;

/** fd for OUT_FILE */
int writefd =0;

int main(int argc ,char** argv){
	/*******************PARSING FUNCTIONS*****************/
	glyph = malloc(sizeof(Glyph));
	filename = NULL;
	output = NULL;

	parse_args(argc, argv);

	if(argv[optind+1]!=NULL){
		if(argv[optind+2]==NULL){
			filename = malloc(((int)(strlen(argv[optind])+1))*sizeof(char));
			memset(filename, 0, ((int)(strlen(argv[optind])+1))*sizeof(char));
			strcpy(filename, argv[optind]);

			if(argv[optind+1]!=NULL){
				output = malloc(((int)(strlen(argv[optind+1])+1))*sizeof(char));
				memset(output, 0, ((int)(strlen(argv[optind+1])+1))*sizeof(char));
				strcpy(output, argv[optind+1]);
			}
			else{
				writefd = STDOUT_FILENO;
			}
		} else {
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);
		}
	}
	else{
		if(argv[optind+2]==NULL){
			filename = malloc(((int)(strlen(argv[optind])+1))*sizeof(char));
			memset(filename, 0, ((int)(strlen(argv[optind])+1))*sizeof(char));
			strcpy(filename, argv[optind]);

			if(argv[optind+1]!=NULL){
				output = malloc(((int)(strlen(argv[optind+1])+1))*sizeof(char));
				memset(output, 0, ((int)(strlen(argv[optind+1])+1))*sizeof(char));
				strcpy(output, argv[optind+1]);
			}
			else{
				writefd = STDOUT_FILENO;
			}
		} else{
			writefd = STDOUT_FILENO;
		}
	}

	if(argv[optind+1]!=NULL){
		stat(filename, &in);
		stat(output, &out);
		if(in.st_ino == out.st_ino){
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);
		}
		else{
			readfd = open(filename, O_RDONLY);
			if(reader(readfd,buf,1)==0){
				print_help();
				quit_converter(readfd, writefd);
				exit(EXIT_FAILURE);
			}
			close(readfd);
			if(stat(output, &out) != 0){
				if(strcmp(output,"stdout")!=0){
					fptr = fopen(argv[optind+1], "w");
					fclose(fptr);
					writefd = open(output, O_WRONLY);
				}
				else{
					writefd = STDOUT_FILENO;
				}
				readfd = open(filename, O_RDONLY);
				exists = true;
			}
			else{		
				readfd = open(filename, O_RDONLY);
				writefd = open(output, O_RDONLY);
				if(reader(writefd,buf,2)){
					if(buf[0] == 0xff && buf[1]==0xfe){
						if(conversion != LITTLE){
							print_help();
							quit_converter(readfd, writefd);
							exit(EXIT_FAILURE);
						}
					} else if(buf[0] == 0xfe && buf[1]==0xff){
						if(conversion != BIG){
							print_help();
							quit_converter(readfd, writefd);
							exit(EXIT_FAILURE);
						}
					}  else if(buf[0] == 0xef && buf[1]==0xbb){
						read(writefd,&buf[2],1);
						if(buf[2] != 0xbf){
							print_help();
							quit_converter(readfd, writefd);
							exit(EXIT_FAILURE);
						}
						else{
							if(conversion != EIGHT){
								print_help();
								quit_converter(readfd, writefd);
								exit(EXIT_FAILURE);
							}
						}
					}
					else{
						print_help();
						quit_converter(readfd, writefd);
						exit(EXIT_FAILURE);
					}
					exists = false;

				}
				close(writefd);
				if(strcmp(output,"stdout")!=0){
					writefd = open(output, O_APPEND | O_WRONLY);
				}
				else{
					writefd = STDOUT_FILENO;
				}


				exists = false;
			}
		}
	}
	else{/*UNKNOWN PURPOSE --------------- NEEDS TO BE FOUND OUT*/
	filename = malloc(((int)(strlen(argv[optind])+1))*sizeof(char));
	memset(filename, 0, ((int)(strlen(argv[optind])+1))*sizeof(char));
	strcpy(filename, argv[optind]);
	writefd = STDOUT_FILENO;
	readfd = open(filename, O_RDONLY);
	if(readfd == -1 || writefd == -1){
		print_help();
		quit_converter(readfd, writefd);
		exit(EXIT_FAILURE);
	}
}

	/*******************READING BOM*****************/
if(reader(readfd,buf,2)){
	if(buf[0] == 0xff && buf[1]==0xfe){
		source = LITTLE; 
	} else if(buf[0] == 0xfe && buf[1]==0xff){
		source = BIG;
	}  else if(buf[0] == 0xef && buf[1]==0xbb){
		startReading();
		rv = read(readfd,&buf[2],1);
		stopReading();
		if(rv != 1){
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);
		}
		if(buf[2]==0xbf){
			source = EIGHT;
		}
		else{
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);
		}

	}
	else {
		print_help();
		quit_converter(readfd, writefd);
		exit(EXIT_FAILURE);
	}
}
else{
	print_help();
	quit_converter(readfd, writefd);
	exit(EXIT_FAILURE);
}

if(exists == false){
	if(conversion == LITTLE){
		glyph->bytes[0] = '\n';
		glyph->bytes[1] = 0x0;
		write(writefd,glyph->bytes,2);
	}
	else if(conversion == BIG){
		glyph->bytes[1] = '\n';
		glyph->bytes[0] = 0;
		write(writefd,glyph->bytes,2);
	}
	else if(conversion == EIGHT){
		glyph->bytes[0] = '\n';
		write(writefd,glyph->bytes,1);
	}
}


	/*******************CONVERTER FUNCTIONS*****************/
if(source == LITTLE || source == BIG){
	if(conversion == LITTLE || conversion == BIG){
		glyph = fill_glyph(glyph, buf, source, &readfd);
		if(conversion!=LITTLE){
			glyph = swap_endianness(glyph);
		}
		if(exists){
			write_glyph(glyph,writefd);		
		}

		while(reader(readfd, buf, 2)){
			glyph = fill_glyph(glyph, buf, source, &readfd);
			if(conversion!=LITTLE){
				glyph = swap_endianness(glyph);
			};
			write_glyph(glyph,writefd);
		}
	}else if(conversion == EIGHT){
		convert_reverse(glyph, source);
	}
}
else if(source == EIGHT){
	startConverting();
	glyph = convert(glyph, conversion);
	if(conversion!=LITTLE){
		glyph = swap_endianness(glyph);
	}
	stopConverting();
	if(exists){
		write_glyph(glyph,writefd);		
	}

	while(reader(readfd, buf, 1)){
		startConverting();
		glyph = convert(glyph, conversion);
		if(conversion!=LITTLE){
			glyph = swap_endianness(glyph);
		};
		stopConverting();
		write_glyph(glyph,writefd);
	}
}
	/*******************VERBOSITY FUNCTIONS*****************/
if(VFLAG){
	write_verbosity(filename);
}
close(STDERR_FILENO);
quit_converter(readfd, writefd);
exit(EXIT_SUCCESS);
}

void convert_reverse(Glyph* glyph, endianness end){
	int bits = 0;
	long seg0 = 0;
	long seg1 = 0;
	long seg2 = 0;
	long seg3 = 0;
	long bitsHigh = 0;
	long bitsLow = 0;
	long vLow = 0;
	long vHigh = 0;
	glyph->bytes[0] = 0xef;
	glyph->bytes[1] = 0xbb;
	glyph->bytes[2] = 0xbf;
	if(exists == true){
		write(writefd , glyph->bytes, 3);
	}
	while(reader(readfd,buf,2)){
		bits = 0;
		if(end == LITTLE){
			bits |= (buf[FIRST] + (buf[SECOND]<<8));
		}
		else{
			bits |= ((buf[FIRST]<<8) + buf[SECOND]);
		}

		if(bits > 0xD800 && bits < 0xDBFF){
			bitsHigh = bits;
			if(reader(readfd, buf, 2)){
				bitsLow = 0;
				if(end == LITTLE){
					bitsLow |= (buf[FIRST] + (buf[SECOND]<<8));
				}
				else if(end == BIG){
					bitsLow |= ((buf[FIRST]<<8) + buf[SECOND]);
				}
				if(bitsLow > 0xDC00 && bitsLow < 0xDFFF){
					surrogateCounter++;
					glyph->surrogate = true;

					vLow = bitsLow - 0xDC00;
					vHigh = bitsHigh - 0xD800;

					bits = (((vHigh<<10)|(vLow)))+0x10000;

				} else {
					lseek(readfd, -OFFSET, SEEK_CUR);
					glyph->surrogate = false;
				}
			}
		}
		else{
			glyph->surrogate = false;
		}

		if(bits >= 0 && bits <0x7f){
			if(end == LITTLE){
				glyph->bytes[0] = buf[0];	
			}
			else if(end == BIG){
				glyph->bytes[0] = buf[1];
			}

			write(writefd , glyph->bytes, 1);
		}
		else if(bits >= 0x80 && bits <= 0x7ff){
			seg0 = (bits & 0x3F)|0x80;
			bits = bits >> 6;
			seg1 = ((bits & 0x1F)|0xC0);

			glyph->bytes[0] = seg1;
			glyph->bytes[1] = seg0;

			write(writefd,glyph->bytes,2);
		}
		else if(bits >= 0x800 && bits <= 0xFFFF){
			seg0 = (bits & 0x3F)|0x80;
			bits = bits >> 6;
			seg1 = ((bits & 0x3F)|0x80);
			bits = bits >> 6;
			seg2 = ((bits & 0xF)|0xE0);

			glyph->bytes[0] = seg2;
			glyph->bytes[1] = seg1;
			glyph->bytes[2] = seg0;
			write(writefd,glyph->bytes,3);
		}
		else if(bits >= 0x10000 && bits <= 0x1FFFFF){
			seg0 = (bits & 0x3F)|0x80;
			bits = bits >> 6;
			seg1 = ((bits & 0x3F)|0x80);
			bits = bits >> 6;
			seg2 = ((bits & 0x3F)|0x80);
			bits = bits >> 6;
			seg3 = ((bits & 0x7)|0xF0);

			glyph->bytes[0] = seg3;
			glyph->bytes[1] = seg2;
			glyph->bytes[2] = seg1;
			glyph->bytes[3] = seg0;
			write(writefd,glyph->bytes,4);
		}
		else{
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);

		}
	}
	glyphCounter++;

}

Glyph* fill_glyph(Glyph * glyph, unsigned char data[2],endianness end,int* fd) {
	unsigned int bits = 0; 
	startConverting();
	if(end == LITTLE){
		if(((int)data[0]) < 128 && ((int)data[1]) == 0){
			asciiCounter++;
		}
		glyph->bytes[0] = data[0];
		glyph->bytes[1] = data[1];
		bits |= (data[FIRST] + (data[SECOND]<<8));

	}
	else if(end == BIG){
		if(((int)data[1]) < 128 && ((int)data[0]) == 0){
			asciiCounter++;
		}
		glyph->bytes[0] = data[1];
		glyph->bytes[1] = data[0];
		bits |= ((data[FIRST]<<8) + data[SECOND]);
	}
	stopConverting();

	if(bits > 0xD800 && bits < 0xDBFF){
		if(reader(*fd, data, 2)){
			bits = 0;
			if(end == LITTLE){
				bits |= (data[FIRST] + (data[SECOND]<<8));
			}
			else if(end == BIG){
				bits |= ((data[FIRST]<<8) + data[SECOND]);
			}
			if(bits > 0xDC00 && bits < 0xDFFF){
				surrogateCounter++;
				glyph->surrogate = true; 
			} else {
				lseek(*fd, -OFFSET, SEEK_CUR);
				glyph->surrogate = false;
			}
		}
	}
	else{
		glyph->surrogate = false; /*setting surrogate to false is not in the range*/
	}

	if(!glyph->surrogate){
		glyph->bytes[THIRD] = glyph->bytes[FOURTH] |= 0;
	} else {
		if(end == LITTLE){
			glyph->bytes[THIRD] = data[FIRST]; 
			glyph->bytes[FOURTH] = data[SECOND];
		}
		else{
			glyph->bytes[THIRD] = data[SECOND]; 
			glyph->bytes[FOURTH] = data[FIRST];
		}

	}
	glyph->end = conversion;
	return (Glyph * )glyph;
}


Glyph* convert(Glyph * glyph, endianness conversion) {
	long highSurrogate = 0;
	long lowSurrogate = 0;
	long codepoint = 0;
	long codepointPrime = 0;
	long codepointHigh = 0;
	long codepointLow = 0;
	startConverting();
	if(glyphCounter == 0){
		glyph->bytes[0] = 0xff;
		glyph->bytes[1] = 0xfe;
		glyph->surrogate = false;
	}
	else{
		if(buf[0] < 0x80){
			glyph->bytes[0] = buf[0];
			glyph->bytes[1] = 0x0;
			glyph->surrogate = false;
			asciiCounter++;
		}
		else if(buf[0] >= 0xC0 && buf[0] <= 0xDF){
			startReading();
			(read(readfd, &buf[1], 1));
			stopReading();
			glyph->bytes[0] = buf[0] & 0x1F;
			glyph->bytes[1] = buf[1] & 0x3F;

			codepoint += glyph->bytes[0];
			codepoint = codepoint << 6;
			codepoint += glyph->bytes[1];

			if(codepoint > 0xFF && codepoint < 0x10000){
				highSurrogate = codepoint & 0xFF00;
				highSurrogate = highSurrogate >> 8;
				lowSurrogate = codepoint & 0x00FF;
				glyph->bytes[0] = lowSurrogate;
				glyph->bytes[1] = highSurrogate;

				glyph->surrogate = false;
			}
			else if(codepoint > 0x10000){
				/*printf("%s\n", "surrogate Pair");*/
				codepointPrime= codepoint - 0x10000;

				codepointHigh = codepointPrime >> 10;

				codepointLow = codepointPrime & 0x3FF;

				lowSurrogate = 0xD800 + codepointHigh;

				highSurrogate = 0xDC00 + codepointLow;

				glyph->bytes[2] = highSurrogate;
				glyph->bytes[3] = highSurrogate >> 8;
				glyph->bytes[0] = lowSurrogate;
				glyph->bytes[1] = lowSurrogate >> 8;
				glyph->surrogate = true;
				surrogateCounter++;	
			}
			else if(codepoint > 0x1FFFFF){
				print_help();
				quit_converter(readfd, writefd);
				exit(EXIT_FAILURE);
			}
			else{

				glyph->bytes[0] = codepoint;
				glyph->bytes[1] = 0x0;
				glyph->surrogate = false;
			}
		}
		else if(buf[0] >= 0xE0 && buf[0] <= 0xEF){
			startReading();
			(read(readfd, &buf[1], 1));
			(read(readfd, &buf[2], 1));
			stopReading();

			buf[0] = buf[0] & 0xF;
			buf[1] = buf[1] & 0x3F;
			buf[2] = buf[2] & 0x3F;

			codepoint += buf[0];
			codepoint = codepoint << 6;
			codepoint += buf[1];
			codepoint = codepoint << 6;
			codepoint += buf[2];

			if(codepoint > 255 && codepoint < 65536){
				highSurrogate = codepoint & 0xFF00;
				highSurrogate = highSurrogate >> 8;
				lowSurrogate = codepoint & 0x00FF;
				glyph->bytes[1] = highSurrogate;
				glyph->bytes[0] = lowSurrogate;
				glyph->surrogate = false;
			}
			else if(codepoint > 0x10000){
				/*printf("%s\n", "surrogate Pair");*/
				codepointPrime= codepoint - 0x10000;

				codepointHigh = codepointPrime >> 10;

				codepointLow = codepointPrime & 0x3FF;

				lowSurrogate = 0xD800 + codepointHigh;

				highSurrogate = 0xDC00 + codepointLow;

				glyph->bytes[2] = highSurrogate;
				glyph->bytes[3] = highSurrogate >> 8;
				glyph->bytes[0] = lowSurrogate;
				glyph->bytes[1] = lowSurrogate >> 8;
				glyph->surrogate = true;
				surrogateCounter++;			
			}
			else if(codepoint > 0x1FFFFF){
				print_help();
				quit_converter(readfd, writefd);
				exit(EXIT_FAILURE);
			}
			else{
				glyph->bytes[0] = codepoint;
				glyph->bytes[1] = 0x0;
				glyph->surrogate = false;
			}
		}
		else if(buf[0] >= 0xF0){
			startReading();
			(read(readfd, &buf[1], 1));
			(read(readfd, &buf[2], 1));		
			(read(readfd, &buf[3], 1));
			stopReading();

			buf[0] = buf[0] & 0x7;
			buf[1] = buf[1] & 0x3F;
			buf[2] = buf[2] & 0x3F;
			buf[3] = buf[3] & 0x3F;

			codepoint += buf[0] ;
			codepoint = codepoint << 6;
			codepoint += buf[1];
			codepoint = codepoint << 6;
			codepoint += buf[2];
			codepoint = codepoint << 6;
			codepoint += buf[3];

			if(codepoint > 0xFF && codepoint < 0x10000){
				highSurrogate = codepoint & 0xFF00;
				highSurrogate = highSurrogate >> 8;
				lowSurrogate = codepoint & 0x00FF;
				glyph->bytes[1] = highSurrogate;
				glyph->bytes[0] = lowSurrogate;
				glyph->surrogate = false;
			}
			else if(codepoint >= 0x10000){
				codepointPrime= codepoint - 0x10000;

				codepointHigh = codepointPrime >> 10;

				codepointLow = codepointPrime & 0x3FF;

				lowSurrogate = 0xD800 + codepointHigh;

				highSurrogate = 0xDC00 + codepointLow;

				glyph->bytes[2] = highSurrogate;
				glyph->bytes[3] = highSurrogate >> 8;
				glyph->bytes[0] = lowSurrogate;
				glyph->bytes[1] = lowSurrogate >> 8;
				glyph->surrogate = true;
				surrogateCounter++;
			}
			else if(codepoint > 0x1FFFFF){
				print_help();
				quit_converter(readfd, writefd);
				exit(EXIT_FAILURE);
			}
			else{
				glyph->bytes[0] = codepoint;
				glyph->bytes[1] = 0x0;
				glyph->surrogate = false;
			}
		}
	}
	glyph->end = conversion;
	stopConverting();
	glyphCounter++;
	return (Glyph * )glyph;	
}

Glyph* swap_endianness(Glyph* glyph){
	unsigned int temp = 0;
	startConverting();
	temp = glyph->bytes[1];
	glyph->bytes[1] = glyph->bytes[0];
	glyph->bytes[0] = temp; 
	if(glyph->surrogate){
		temp = glyph->bytes[3];
		glyph->bytes[3] = glyph->bytes[2];
		glyph->bytes[2] = temp;
	}
	glyph->end = conversion;
	stopConverting();
	return glyph;
}


void write_glyph(Glyph* glyph,int fd){
	startWriting();
	glyphCounter++;
	if(glyph->surrogate){
		write(fd, glyph->bytes, SURROGATE_SIZE);
	} else {
		write(fd, glyph->bytes, NON_SURROGATE_SIZE);
	}
	stopWriting();

}


void parse_args(int argc, char**argv){
	int option_index, c, i;
	char* endian_convert = NULL;
	while((c = getopt_long(argc, argv, "hvu:", long_options, &option_index))!= -1){
		switch(c){ 
			case 'h':
			if(argc == 2){
				print_help();
				free(glyph);
				exit(EXIT_SUCCESS);
			}
			else{
				print_help();
				free(glyph);
				exit(EXIT_FAILURE);
			}
			break;

			case 'u':
			endian_convert = optarg;
			break;

			case 'v':
			VFLAG++;
			break;

			default:
			print_help();
			quit_converter(readfd, writefd);
			exit(EXIT_FAILURE);
			break;
		}

	}
	for(i =0; i < argc ; i++){
		if(strcmp(argv[i],"--UTF")==0){
			print_help();
			free(glyph);
			exit(EXIT_FAILURE);
		}
	}

	if(endian_convert == NULL){
		print_help();
		quit_converter(readfd,writefd);
		exit(EXIT_FAILURE);
	}

	if(strcmp(endian_convert, "16LE")==0){ 
		conversion = LITTLE;
	} else if(strcmp(endian_convert, "16BE")==0){
		conversion = BIG;
	} 
	else if(strcmp(endian_convert, "8")==0){
		conversion = EIGHT;
	}
	else {
		print_help();
		quit_converter(readfd, writefd);
		exit(EXIT_FAILURE);

	}
}

void print_help() {
	int i;
	for(i = 0; i < 12 ; i++){
		fprintf(stderr,"%s",USAGE[i]); 
	}
}

void quit_converter(int readfd, int writefd){
	free(glyph);
	free(filename);
	free(output);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if(readfd != NO_FD){
		close(readfd);
	}
	if(writefd != NO_FD){
		close(writefd);
	}
}

void write_verbosity(char*filename){
	int size = 0;
	char *symlinkpath = calloc(1024,sizeof(char));
	char *actualpath = calloc(1024,sizeof(char));
	char *hostname = calloc(1024,sizeof(char));
	double printAscii = 0;
	double printSurrogate = 0;
	struct utsname unameData;

	uname(&unameData);
	stat(filename, &in);
	size = in.st_size;
	fprintf(stderr,"Input file size: %f kb\n", (size/1024.0));

	strcpy(symlinkpath,filename);
	fprintf(stderr,"Input file path: %s\n",realpath(symlinkpath, actualpath));

	if(source == BIG){
		fprintf(stderr,"%s\n", "Input file encoding: UTF-16BE");
	}
	else if(source == LITTLE){
		fprintf(stderr,"%s\n", "Input file encoding: UTF-16LE");
	}
	else if(source == EIGHT){
		fprintf(stderr,"%s\n", "Input file encoding: UTF-8");
	}
	else{
		fprintf(stderr,"%s\n", "Input file encoding: -");
	}

	if(conversion == BIG){
		fprintf(stderr,"%s\n", "Output encoding: UTF-16BE");
	}
	else if(conversion == LITTLE){
		fprintf(stderr,"%s\n", "Output encoding: UTF-16LE");
	}
	else{
		fprintf(stderr,"%s\n", "Output encoding: -");
	}

	gethostname(hostname,1024);

	fprintf(stderr,"Hostmachine: %s\n", hostname);

	fprintf(stderr,"Operating System: %s\n", unameData.sysname);

	if(VFLAG > 1){
		fprintf(stderr,"Reading: real=%.1f, user=%.1f, sys=%.1f\n", (realRead/sysconf(_SC_CLK_TCK)) , (userRead/sysconf(_SC_CLK_TCK)) , (sysRead/sysconf(_SC_CLK_TCK)));
		fprintf(stderr,"Converting: real=%.1f, user=%.1f, sys=%.1f\n", (realConvert/sysconf(_SC_CLK_TCK)) , (userConvert/sysconf(_SC_CLK_TCK)) , (sysConvert/sysconf(_SC_CLK_TCK)));
		fprintf(stderr,"Writing: real=%.1f, user=%.1f, sys=%.1f\n", (realWrite/sysconf(_SC_CLK_TCK)) , (userWrite/sysconf(_SC_CLK_TCK)) , (sysWrite/sysconf(_SC_CLK_TCK)));

		if(glyphCounter != 0){
			printAscii = ((asciiCounter/glyphCounter)*100) - ((int)((asciiCounter/glyphCounter)*100));
			printSurrogate = ((surrogateCounter/glyphCounter)*100)  - ((int)((surrogateCounter/glyphCounter)*100));
			if(printAscii < 0.5){
				fprintf(stderr,"ASCII: %d%%\n", (int)((asciiCounter/glyphCounter)*100));
			}
			else{
				fprintf(stderr,"ASCII: %d%%\n", (int)(((asciiCounter/glyphCounter)*100))+1);
			}
			if(printSurrogate < 0.5){
				fprintf(stderr,"Surrogates: %d%%\n", ((int)((surrogateCounter/glyphCounter)*100)));
			}
			else{
				fprintf(stderr,"Surrogates: %d%%\n", (int)(((surrogateCounter/glyphCounter)*100))+1);
			}
		}
		else{
			fprintf(stderr,"ASCII: 0%%\n");
			fprintf(stderr,"Surrogates: 0%%\n");
		}

		fprintf(stderr,"Glyphs: %d\n", glyphCounter);
	}

	free(actualpath);
	free(hostname);
	free(symlinkpath);
}

int reader(int fd,unsigned char array[2],int counter){
	int i = 0;
	startReading();
	for(i = 0; i < counter; i++){
		if(read(fd, &array[i], 1) != 1){
			stopReading();
			return 0;
		}

	}
	stopReading();
	return 1;
}

void startReading(){
	st_read_time = times(&st_read_cpu);
}
void stopReading(){
	en_read_time = times(&en_read_cpu);
	realRead += (en_read_time - st_read_time);
	userRead += (en_read_cpu.tms_utime - st_read_cpu.tms_utime);
	sysRead += (en_read_cpu.tms_stime - st_read_cpu.tms_stime);
}

void startConverting(){
	st_convert_time = times(&st_convert_cpu);
}

void stopConverting(){
	en_convert_time = times(&en_convert_cpu);
	realConvert += (en_convert_time - st_convert_time);
	userConvert += (en_convert_cpu.tms_utime - st_convert_cpu.tms_utime);
	sysConvert += (en_convert_cpu.tms_stime - st_convert_cpu.tms_stime);
}

void startWriting(){
	st_write_time = times(&st_write_cpu);
}

void stopWriting(){
	en_write_time = times(&en_write_cpu);
	realWrite += (en_write_time - st_write_time);
	userWrite += (en_write_cpu.tms_utime - st_write_cpu.tms_utime);
	sysWrite += (en_write_cpu.tms_stime - st_write_cpu.tms_stime);
}