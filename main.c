#include<stdio.h>
#include"mp3_code.h"
//#include"mp3_code.c"
int main (int argc, char *argv[])
{
    mp3_reader mp3;
    if(argc<3)
    {
	printf("Error: ./a.out : Invalid arguments\nUsage:\nTo view please pass like: ./a.out -v mp3filename\nTo edit please pass like -e -t/-a/-A/-y/-c ""new content"" mp3filename\n");
	return failure;
    }
    if(check_operation_type(argv)==view)
    {
	if(read_and_validate(argv,&mp3) == success)
	{
	    if(Open_files(&mp3)==success)
	    {	
		if(reading_data_from_mp3(argv,&mp3) == success)
		{
		    return success;
		}
		else
		{
		    printf("Error: Reading data from mp3 failed\n");
		    return failure;
		}
		fclose(mp3.fptr_mp3);
	    }
	}
    }
    else if(read_and_validate_for_edit(argv, &mp3) == success)
    {
	if(open_files_for_edit(&mp3) == success)
	{
	    if(check_for_which_to_edit(&mp3)== success)
	    {
		if(copy_to_temp(argv,&mp3) == success)
		{
		    printf("Info: Editing of the Mp3 file is done\n");
		}
		else
		{
		    printf("Error: Editing of the Mp3 file failed\n");
		    return failure;
		}
		fclose(mp3.fptr_mp3);
		fclose(mp3.fptr_temp);
	    }
	}
	else
	{
	    printf("Error: Unable to open files\n");
	    return failure; 
	}
    }
}
