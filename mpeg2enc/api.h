#ifndef MPEG2ENC_API_H
#define MPEG2ENC_API_H

typedef void (*SimpegWrite_error_cb)(const char * text);
typedef void (*SimpegWrite_warning_cb)(const char * text);
typedef int  (*SimpegWrite_progress_cb)(float sub, int current_frame, int num_frames);

int SimpegWrite_encode(const char * output_filename,
                       const char * parameter_filename,
                       SimpegWrite_error_cb error_cb, 
                       SimpegWrite_warning_cb warning_cb,
                       SimpegWrite_progress_cb progress_cb); 

/* returns handle */
void * SimpegWrite_begin_encode(const char * output_filename,
                                const char * parameter_filename,
                                SimpegWrite_error_cb error_cb, 
                                SimpegWrite_warning_cb warning_cb,
                                SimpegWrite_progress_cb progress_cb); 

int SimpegWrite_encode_bitmap(void * handle, const unsigned char * rgb_buffer);

int SimpegWrite_end_encode(void * handle);

#endif /* MPEG2ENC_API_H */
