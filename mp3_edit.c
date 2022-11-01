#include<stdio.h>
#include<string.h>
#include "mp3_code.h"
#include<stdlib.h>
Status read_and_validate_for_edit(char *argv[], mp3_reader *mp3)
{
    if(argv[2] != NULL)
    {
	printf("Info: Checking whether the given argument for editing is correct\n");
	if((strcmp(argv[2],"-t")==0) || (strcmp(argv[2],"-a")==0) ||(strcmp(argv[2],"-A")==0) ||(strcmp(argv[2],"-y")==0) ||(strcmp(argv[2],"-c")==0)||(strcmp(argv[2],"-g")==0))
	{
	    strcpy(mp3->character_type, argv[2]);
	    printf("Info: Agrument passed is valid\n");
	    printf("Info: User has passed the argument %s which is used to change: ",mp3->character_type);
	    if(strcmp(argv[2],"-t")==0)
	    {
		printf("Title of the mp3 file\n");
	    }

	    else if(strcmp(argv[2],"-a")==0) 
	    {
		printf("Artist of the mp3 file\n");
	    }
	    else if(strcmp(argv[2],"-A")==0) 
	    {
		printf("Album of the mp3 file\n");
	    }
	    else if(strcmp(argv[2],"-Y")==0)
	    {
		printf("Year of the mp3 file\n");
	    }
	    else if(strcmp(argv[2],"-C")==0) 
	    {
		printf("Composer of the mp3 file\n");
	    }
	}
	else
	{
	    printf("Error: Given title not matching\n");
	    return failure;
	}
    }
    else
    {
	printf("Error: Argument not passed\n");
	return failure;
    }
    printf("Info: Checking new content\n");
    if(argv[3] != NULL)
    {
	mp3->new_content = argv[3];
	printf("Info: Argument for new content to be edited in the file is passed successfully\n");
	mp3->new_content_length = strlen(mp3->new_content)+1;
    }
    else
    {
	printf("Info: Error new content is not present\n");
	return failure;
    }
    printf("Info: Checking for MP3 file\n");
    if(argv[4] != NULL)
    {
	mp3->fptr_mp3 = fopen(argv[4], "r");
	unsigned short sh;
	fread(&sh,1,2,mp3->fptr_mp3);
	printf("Info: Checking for argument is the given file passed is a valid Mp3 file\n");
	if(sh == 0x4449)
	{
	    strcpy(mp3->mp3_fname,argv[4]);
	    fclose(mp3->fptr_mp3);
	}
	else
	{
	    printf("Error: Given file is not a valid mp3 file\n");
	    return failure;
	}
    }
    else
    {
	printf("Error: mp3 file not found\n");
	return failure;
    }
    return success;
}
Status check_for_which_to_edit(mp3_reader *mp3)
{
    if(strcmp(mp3->character_type,"-t")==0)
	strcpy(mp3->frame_id, "TIT2");
    else if(strcmp(mp3->character_type,"-a")==0)
	strcpy(mp3->frame_id , "TPE1");
    else if(strcmp(mp3->character_type,"-y")==0)
	strcpy(mp3->frame_id ,"TYER");
    else if(strcmp(mp3->character_type,"-A")==0)
	strcpy(mp3->frame_id ,"TALB");
    else if(strcmp(mp3->character_type,"-c")==0)
	strcpy(mp3->frame_id ,"COMM");
    else if(strcmp(mp3->character_type,"-g")==0)
	strcpy(mp3->frame_id ,"TCON");

    return success;
}
Status open_files_for_edit(mp3_reader *mp3)
{
    printf("Info: Opening files\n");
    mp3->fptr_mp3 = fopen(mp3->mp3_fname, "r");
    if(mp3->fptr_mp3 == NULL)
    {
	printf("Error: Unable to open file %s\n",mp3->mp3_fname);
	return failure;
    }
    mp3->temp_fname = "temp_file.mp3";
    mp3->fptr_temp = fopen(mp3->temp_fname, "w+");
    if(mp3->fptr_temp == NULL)
    {
	printf("Error: Unable to open file %s\n",mp3->temp_fname);
	return failure;
    }
    printf("Info: Files opened successfully\n");
    return success;
}
Status copy_to_temp(char *argv[], mp3_reader *mp3)
{
    char ch[4];
    int flag=1;
    ch[4] = '\0';
    printf("Info: Writing the content which needs to be changed and remaining in Temporary file\n");
    fseek(mp3->fptr_mp3,0,SEEK_SET);
    fseek(mp3->fptr_temp,0,SEEK_SET);
    fread(mp3->ch2,1,10,mp3->fptr_mp3);
    fwrite(mp3->ch2,1,10,mp3->fptr_temp);
    fread(mp3->ch3,1,4,mp3->fptr_mp3);
    fwrite(mp3->ch3,1,4,mp3->fptr_temp);
    while(flag==1)
    {
	if(strcmp(mp3->ch3,mp3->frame_id)==0)
	{
	    Copy_remaining_data(mp3);
	    flag=0;
	}
	else 
	{
	    getting_frame_id(mp3);
	}
    }
    fclose(mp3->fptr_mp3);
    fclose(mp3->fptr_temp);
    printf("Info: Editing to the new file : DONE\n");
    printf("Info: Copying back the content of the temporary file to the original file\n");
    copy_back_to_original_mp3(mp3);
    return success;
}
Status Copy_remaining_data(mp3_reader *mp3)
{
    char ch[4];
    int size_of_mp3,size_2, flag, len_for_new_content_updated;
    len_for_new_content_updated = endian_conversion(mp3, mp3->new_content_length);
    fwrite(&len_for_new_content_updated,1,4,mp3->fptr_temp);
    fread(&size_2,1,4,mp3->fptr_mp3);
    fread(&flag,1,3,mp3->fptr_mp3);
    fwrite(&flag,1,3,mp3->fptr_temp);
    size_2 = endian_conversion(mp3,size_2);
    fwrite(mp3->new_content,1,mp3->new_content_length-1,mp3->fptr_temp);
    fseek(mp3->fptr_mp3,size_2-1,SEEK_CUR);
    char ch2;
    while(fread(&ch2, sizeof(char),1,mp3->fptr_mp3)>0)
    {
	fwrite(&ch2,1,1,mp3->fptr_temp);
    }
    return success;
}
Status endian_conversion(mp3_reader *mp3, int size)
{
    int i;
    char temp;
    char *ptr = (char *)&size;
    for(int i=0;i<2;i++)
    {
	temp = ptr[i];
	ptr[i] = ptr[4 - i - 1];
	ptr[4 - i - 1]=temp;
    }
    return size;
}
Status getting_frame_id(mp3_reader *mp3)
{
    int size,flag;
    fread(&size,1,4,mp3->fptr_mp3);
    fwrite(&size,1,4,mp3->fptr_temp);
    size = endian_conversion(mp3,size);
    fread(&flag,1,3,mp3->fptr_mp3);
    fwrite(&flag,1,3,mp3->fptr_temp);
    mp3->for_storing_remains = malloc(size *sizeof(char));
    fread(mp3->for_storing_remains,1,size-1,mp3->fptr_mp3);
    fwrite(mp3->for_storing_remains,1,size-1,mp3->fptr_temp);
    fread(mp3->ch3,1,4,mp3->fptr_mp3);
    fwrite(mp3->ch3,1,4,mp3->fptr_temp);
}
Status copy_back_to_original_mp3(mp3_reader *mp3)
{
    mp3->fptr_mp3 = fopen(mp3->mp3_fname, "w");
    mp3->fptr_temp = fopen(mp3->temp_fname, "r");
    fseek(mp3->fptr_temp,0,SEEK_SET);
    fseek(mp3->fptr_mp3,0,SEEK_SET);
    char ch2;
    while(fread(&ch2, sizeof(char),1,mp3->fptr_temp)>0)
    {
	fwrite(&ch2,1,1,mp3->fptr_mp3);
    }
    printf("Info: Copied the edited file back into the original file\n");
}

