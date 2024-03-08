#ifndef DISPATCHER_H
#define DISPATCHER_H

void *dispatcher(void *ignore);
long time_spec_to_micro(time_spec time);
time_spec diff(time_spec start, time_spec end);

#endif
