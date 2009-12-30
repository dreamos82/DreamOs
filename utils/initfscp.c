/*
 * initfscp
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <initfscp.h>

int main(int argc, char* argv[]){
	if(argc <= 1) usage(argv[0]);
	else {
		if(!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) version(argv[0]+2);
		else{
			initrd_file_t headers[MAX_FILES];
			int i=0;
			printf("Welcome to dreamos initfs file copier tool\n");
			printf("Clearing headers structures ");
			for (i=0; i<MAX_FILES; i++){
				headers[i].magic = 0xBF;
				strcpy(headers[i].fileName, "");
				headers[i].uid = 0;
				headers[i].offset = 0;
				headers[i].length = 0;
			}			
			printf("\t\tDONE\n");			
			printf("Number of files to copy %d\n", argc - 2);
			printf("FileSystem name: %s\n", argv[argc-1]);
			printf("Creating File headers\n\n");
			i=0;
			for(i=0; i< argc - 2; i++){
				FILE *fd;
				fd = fopen(argv[i+1], "r+");
				if(fd == NULL){
					 printf("Error one or more files not found\n");
					 return -1;
				 }
				else {
					strcpy(headers[i].fileName, argv[i+1]);
					fseek(fd, 0, SEEK_END);
					printf("File %s Found! Size: %d\n", argv[i+1], ftell(fd));				
					fclose(fd);
				}
			}
			printf("\t\tDONE\n");
			i=0;
			for(i=0; i<argc - 2; i++){
				printf("%s\n", headers[i].fileName);
			}
		}
	}
}

void usage(char *prgname){
	printf("Usage:\n %s --help for this screen\n", prgname);
	printf(" %s file1 file2 ... fsname\n", prgname);
	printf("      file1 file2 ... are source files\n");
	printf("      and fsname is the file that contains the initfs\n");
}

void version(char *prgname){
	printf("%s version: %s\n", prgname, INITFSCP_VER);
}
