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
	if(argc <= 1) {
		if(argv[0][0] == '.')
		version(argv[0]+2);
		usage(argv[0]);		
	}
	else {
		if(!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")) version(argv[0]+2);
		else if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) usage(argv[0]);
		else{
			initrd_file_t headers[MAX_FILES];
			int nfiles;
			nfiles = argc - 2;
			int i=0;
			int j=0;
			FILE *fsdest;
			fsdest = fopen(argv[argc-1], "w");
			if(fsdest == NULL) printf("Could not create FileSystem\n");
			printf("Welcome to Dreamos initfs file copier tool\n");
			printf("Size of headers: %ld %ld\n\n", sizeof(struct initrd_file_t)*32, sizeof(initrd_file_t));
			printf("Clearing headers structures ");
			offset = sizeof(struct initrd_file_t) * 32 + sizeof(int);
			/*Pulizia delle strutture dati*/
			for (i=0; i<MAX_FILES; i++){
				headers[i].magic = 0xBF;
				strcpy(headers[i].fileName, "");
				headers[i].file_type = 0;
				headers[i].uid = 0;
				headers[i].offset = 0;
				headers[i].length = 0;
			}			
			printf("[ \033[1;33mDONE\e[0m ]\n\n");	
			i=0;
			while(!strcmp(argv[j+1], "-m") || !strcmp(argv[j+1], "--mountpoint")){
				printf("Adding mountpoint: %s\n", argv[j+2]);
				strcpy(headers[i].fileName, argv[j+2]);
				headers[i].file_type = FS_MOUNTPOINT;
				headers[i].uid = 0;
				headers[i].offset = 0;
				headers[i].length = 0;
				j+=2;
				i++;
			}
			printf("Number of mountpoints: %d\n", j/2);
			nfiles = nfiles - j;
			printf("Number of files to copy %d\n", nfiles);
			printf("FileSystem name: %s\n\n", argv[argc-1]);
			printf("Creating File headers\n");			
			i=0;			
			for(i=0; i+j< argc - 2; i++){
				FILE *fd;
				fd = fopen(argv[i+j+1],"r+");
				if(fd == NULL){
					 printf("Error one or more files not found\n");
					 // Debug
      					 printf("Name of file: -->%s<--\n",argv[i+j+1]);
					 return -1;
				 }
				else {
					if ((strpbrk(argv[i+j+1], "/")) != NULL)
						strcpy(headers[i+(j/2)].fileName, strtok(strrchr(argv[i+j+1],'/'), "/"));
					else
						strcpy(headers[i+(j/2)].fileName, argv[i+j+1]);
					fseek(fd, 0, SEEK_END);
					printf("\tFile %s Found! Size: %ld\n", argv[i+j+1], ftell(fd));				
					headers[i+(j/2)].file_type = FS_FILE;					
					headers[i+(j/2)].length = ftell(fd);
					headers[i+(j/2)].offset = offset;
					fclose(fd);
					offset += headers[i+(j/2)].length;
				}				
			}
			nfiles = nfiles+(j/2);
			printf("[ \033[1;33mDONE\e[0m ]\n\n");		
			i=0;
			printf("Copying headers to %s filesystem  ", argv[argc-1]);		
			fwrite(&nfiles, sizeof(int), 1, fsdest);
			fwrite(headers, sizeof(struct initrd_file_t), 32, fsdest);
			printf("[ \033[1;33mDONE\e[0m ]\n\n");	
			printf("Copying data to %s filesystem\n", argv[argc-1]);		
			for(i=0; i+j<argc - 2; i++){
				FILE *fd2;
				char *buffer;
				fd2=fopen(argv[i+j+1], "r+");
				buffer = (unsigned char*) malloc(headers[i+(j/2)].length);
				fread(buffer, 1, headers[i+(j/2)].length, fd2);
				fwrite(buffer, 1, headers[i+(j/2)].length, fsdest);
				printf("\t\tFileName: %s Length: %d offset: %d\n", headers[i+(j/2)].fileName, headers[i+(j/2)].length, headers[i+(j/2)].offset);
			}
			printf("[ \033[1;33mDONE\e[0m ]\n\n");	
			fclose(fsdest);
		}		
	}	
}

void usage(char *prgname){
	printf("Usage:\n %s --help for this screen\n", prgname);
	printf(" %s [options] file1 file2 ... fsname\n", prgname);
	printf("      file1 file2 ... are source files\n");
	printf("      and fsname is the file that contains the initfs\n");
	printf("      where [options] are: -m mountpointname\n");
}

void version(char *prgname){
	printf("%s version: %s\n", prgname, INITFSCP_VER);
}
