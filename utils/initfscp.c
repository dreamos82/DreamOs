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
	unsigned int offset;
	if(argc <= 1) usage(argv[0]);
	else {
		if(!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) version(argv[0]+2);
		else if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) usage(argv[0]);
		else{
			initrd_file_t headers[MAX_FILES];
			int i=0;
			FILE *fsdest;
			fsdest = fopen(argv[argc-1], "w");
			if(fsdest == NULL) printf("Could not create FileSystem\n");
			printf("Welcome to Dreamos initfs file copier tool\n");
			printf("Clearing headers structures ");
			offset = sizeof(struct initrd_file_t) * 32;
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
					headers[i].length = ftell(fd);
					headers[i].offset = offset;
					fclose(fd);
					offset += headers[i].length;
				}				
			}
			printf("\t\tDONE\n");
			i=0;
			fwrite(headers, sizeof(struct initrd_file_t), 32, fsdest);
			for(i=0; i<argc - 2; i++){
				printf("FileName: %s Length: %d offset: %d\n", headers[i].fileName, headers[i].length, headers[i].offset);
			}
			fclose(fsdest);
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
