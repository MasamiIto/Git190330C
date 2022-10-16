#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <locale.h>

//******************************
void main( int argc , char** argv )
{
        char    linebuf[1024];
        char    timebuf[ 128];
        char    *p;
        char    *p1,*p2,*p3,wk[256];
        int rc;

        time_t  *current_tt;
        time_t line_tt;

        struct tm   line_time;
        struct tm   *current_timep;
        struct tm   current_time;
        struct tm   *line_timep;
        //--------------------------------------------
        current_tt    = time(NULL);
        current_timep = localtime( &current_tt );
        strftime( timebuf, sizeof(timebuf) ,"%Y/%m/%d %H:%M:%S" ,current_timep );
        printf("CURENT_TIME %d = %s" , current_tt ,timebuf );
        printf("--------------------------------\n");

        current_time.tm_year  = 122;
        current_time.tm_mon   =  10;
        current_time.tm_mday  =  29;
        current_time.tm_hour  =  14;
        current_time.tm_min   =  11;
        current_time.tm_sec   =   4;
        current_tt = mktime( &current_time) ;
        current_timep = localtime( &current_tt );
        strftime( timebuf, sizeof(timebuf) ,"%Y/%m/%d %H:%M:%S" ,current_timep );
        printf("CURENT_TIME %d = %s" , current_tt ,timebuf );
        printf("--------------------------------\n");

        while( 1 ){
                memset( linebuf ,0,sizeof(linebuf));
                p = fgets(linebuf,sizeof(linebuf),stdin);
                if( p==NULL)break;

                if(( NULL!=(p1=strchr(p,'[' ))) && (NULL!=(p2=strchr(p1,']' )))){
                        memset( wk ,0 ,sizeof(wk));
                        memcpy( wk ,p1+1, (p2-p1-1));
                        p3 = strchr( wk, '.' );
                        line_tt    = current_tt + atoi( wk );
                        line_timep = localtime( &line_tt );
//                      strftime( timebuf, sizeof( timebuf ) ,"%Y/%m/%d %H:%M:%S" ,line_timep );
                        strftime( timebuf, sizeof( timebuf ) ,"%H:%M:%S" ,line_timep );
                        if( p3!=NULL ){
                                strcat( timebuf ,p3 );
                        }
                        *p1=0;
                        printf("%s[",linebuf );
                        printf("%s",timebuf );
                        printf("%s",p2 );

                }else{
                        printf("%s",linebuf );
                }

        }
}