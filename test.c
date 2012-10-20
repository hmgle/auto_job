#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "auto_find_extremun.h"
#include "sample_comm.h"

typedef enum 
{
    MOTOR_ZOOM = 0,
    MOTOR_FOCUS
} MOTOR_TYPE;


int Motor_Device_Open(void);
int Motor_Device_Close(void);
int16_t motor_focus_mov(int step);
int16_t motor_zoom_mov(int step);
int16_t motor_step_query(MOTOR_TYPE type);

void move_zoom(int x)
{
    int step_now;
    int diff;

    step_now = motor_step_query(MOTOR_ZOOM);
    diff = x - step_now;

    while (1) {
        if (diff) {
            usleep(1000 * 100);
            motor_zoom_mov(diff);
        } else
            break;

        usleep(1000 * 100);
        step_now = motor_step_query(MOTOR_ZOOM);
        diff = x - step_now;
    }

    return;
}

void move_focus(int x)
{
    int step_now;
    int diff;

    step_now = motor_step_query(MOTOR_FOCUS);
    // fprintf(stderr, "---%d step_now is %d\n", __LINE__, step_now);
    diff = x - step_now;
    // fprintf(stderr, "--%d line diff is %d\n", __LINE__, diff);

    while (1) {
        if (fabs(diff) > 0.1) {
            usleep(1000 * 100);
            motor_focus_mov(diff);
        } else
            break;

        usleep(1000 * 100);
        step_now = motor_step_query(MOTOR_FOCUS);
        diff = x - step_now;
        // fprintf(stderr, "%d line diff is %d\n",__LINE__, diff);
    }

    usleep(1000 * 100);
    return;
}

#if 1
uint32_t get_clear_info(void)
{
	ISP_FOCUS_STA_INFO_S defaultIspValue;
    HI_S32 ret;
    HI_S32 val;

    ret = HI_MPI_ISP_GetFocusStaInfo(&defaultIspValue);
    if (ret) {
        // fprintf(stderr, "HI_MPI_ISP_GetFocusStaInfo failed\n");
        exit(1);
    }
    
    val = defaultIspValue.u16ZoneMetrics[3][4] + 
          defaultIspValue.u16ZoneMetrics[3 - 1][4 - 1] + 
          defaultIspValue.u16ZoneMetrics[3 - 1][4] + 
          defaultIspValue.u16ZoneMetrics[3 - 1][4 + 1] + 
          defaultIspValue.u16ZoneMetrics[3][4 - 1] + 
          defaultIspValue.u16ZoneMetrics[3][4 + 1] + 
          defaultIspValue.u16ZoneMetrics[3 + 1][4 - 1] + 
          defaultIspValue.u16ZoneMetrics[3 + 1][4] + 
          defaultIspValue.u16ZoneMetrics[3 + 1][4 + 1];
    return val;
}
uint32_t x_focus_info(int x)
{
    uint32_t ret;
     /*
     * 1. focus电机移动到x的位置
     */
    // fprintf(stderr, "x is %d\n", x);
    move_focus(x);

    ret = get_clear_info();
    return ret;
}
#endif

int main(int argc, char **argv)
{
    int ret;
    int zoom;
    int min;
    int max;
    if (argc < 4) {
        fprintf(stderr, "usage: %s zoom focus_min, focus_max\n", argv[0]);
        exit(1);
    }
    zoom = atoi(argv[1]);
    min = atoi(argv[2]);
    max = atoi(argv[3]);

    Motor_Device_Open();

    move_zoom(zoom);
    ret = find_extremum_in_range(min, max, x_focus_info);
    fprintf(stderr, "%d\n", ret);

    Motor_Device_Close();
    return 0;
}
