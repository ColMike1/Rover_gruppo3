#include "imu_snapshot.h"

int snapshot_write_called = 0;

void IMUSnapshot_Write(const IMUSnapshot_t *snap)
{
    snapshot_write_called++;
}
