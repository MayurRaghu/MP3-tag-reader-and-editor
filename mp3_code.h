#ifndef MP3_H
#define MP3_H
typedef struct MP3
{
    FILE *fptr_mp3;
    char mp3_fname[50];

    int title_size;
    char *title_fname;
    char *for_storing_data;


    char *new_content;
    char character_type[1];
    char frame_id[4];
    FILE *fptr_temp;
    char *temp_fname;

    char ch[4];
    char ch2[10];
    char ch3[4];
    char *for_storing_remains;
    int new_content_length;


} mp3_reader;
typedef enum
{
    success,
    failure,
    unsupported
} Status;
typedef enum
{
    view,
    edit,
    help
} Operationtype;
/*For View*/
Operationtype check_operation_type(char *argv[]);

Status Open_files(mp3_reader *mp3);

Status read_and_validate(char *argv[],mp3_reader *mp3);

Status reading_data_from_mp3(char *argv[], mp3_reader *mp3);

Status getting_data_from_mp3(mp3_reader *mp3);


Status read_and_validate_for_edit(char *argv[], mp3_reader *mp3);

Status open_files_for_edit(mp3_reader *mp3);

Status copy_to_temp(char *argv[], mp3_reader *mp3);

Status Copy_remaining_data(mp3_reader *mp3);

Status getting_frame_id(mp3_reader *mp3);

Status copy_back_to_original_mp3(mp3_reader *mp3);

Status endian_conversion(mp3_reader *mp3, int size);

Status check_for_which_to_edit(mp3_reader *mp3);

#endif
