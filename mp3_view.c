#include<stdio.h>
#include<string.h>
#include "mp3_code.h"
#include<stdlib.h>
Status read_and_validate(char *argv[], mp3_reader *mp3)
{
    if(argv[2] != NULL)
    {
	mp3->fptr_mp3 = fopen(argv[2], "r");
	unsigned short sh;
	fread(&sh,1,2,mp3->fptr_mp3);
	if(sh == 0x4449)
	{
	    strcpy(mp3->mp3_fname,argv[2]);
	    fclose(mp3->fptr_mp3);
	}
	else
	{
	    printf("Error: Given file is not MP3 file\n");
	    return failure;
	}
    }
    else
	return failure;
  
    return success;
}
Status Open_files(mp3_reader *mp3)
{
    mp3->fptr_mp3 = fopen(mp3->mp3_fname, "r");
    if(mp3->fptr_mp3 == NULL)
    {
	printf("Error: Unable to open file %s\n",mp3->mp3_fname);
	return failure;
    }
    return success;
}

Operationtype check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-v")==0)
	return view;
    else if(strcmp(argv[1],"-e")==0)
	return edit;
    else
	return unsupported;
}
Status reading_data_from_mp3(char *argv[], mp3_reader *mp3)
{
   char temp;
    fseek(mp3->fptr_mp3,10,SEEK_SET);
    printf("\n");
    printf("-------------------------------------------------------------------\n");
    printf("MP3 Tag reader and editor for ID3v2\n");
    printf("-------------------------------------------------------------------\n");
    //for artist
    printf("TItle	:	");
    getting_data_from_mp3(mp3);
    //for song name
    printf("Artist	:	");
    getting_data_from_mp3(mp3);
    //for album
    printf("Album	:	");
    getting_data_from_mp3(mp3);
    //for year
    printf("Year	:	");
    getting_data_from_mp3(mp3);
    //for content type
    printf("Music	:	");
    getting_data_from_mp3(mp3);
    //for compose
    printf("Comment	:	");
    getting_data_from_mp3(mp3);
    printf("-------------------------------------------------------------------\n");
    return success;
}
Status getting_data_from_mp3(mp3_reader *mp3)
{
    int i;
    char ch[4];
    int sh,size;
    fread(ch,1,4,mp3->fptr_mp3);
    fread(&size,1,4,mp3->fptr_mp3);
    fseek(mp3->fptr_mp3,3,SEEK_CUR);
    char temp;
    char *ptr = (char *)&size;
    for(int i=0;i<2;i++)
    {
	temp = ptr[i];
	ptr[i] = ptr[4 - i - 1];
	ptr[4 - i - 1]=temp;
    }

    mp3->for_storing_data = malloc((size-1) * sizeof(char));
    fread(mp3->for_storing_data,1,size-1,mp3->fptr_mp3);
    for(i=0;i<=size;i++)
    {
	printf("%c",mp3->for_storing_data[i]);
    }
    printf("\n");
}

