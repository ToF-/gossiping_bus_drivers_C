#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#define DRIVER_LIMIT (32)
#define ROUTE_LIMIT (256)

int Max_drivers;
int Max_stops[DRIVER_LIMIT];
int Stops[DRIVER_LIMIT][ROUTE_LIMIT];
int Gossips[DRIVER_LIMIT];
int Drivers_at_stop[ROUTE_LIMIT];

int get_int() {
    int result;
    scanf("%d\n", &result);
    return result;
}

void add_gossip(int *gossip_set, int driver) {
    *gossip_set |= 1 << driver;
}

void get_max_drivers() {
    Max_drivers = get_int();
    if(Max_drivers > DRIVER_LIMIT || Max_drivers < 0) {
        printf("illegal number of drivers: %d\n"
            , Max_drivers);
        exit(1);
    }
}

void get_route(int driver) {
    Max_stops[driver] = get_int();
    if(Max_stops[driver] > ROUTE_LIMIT || Max_stops[driver] < 0) {
        printf("illegal number of stops (%d) for driver %d\n"
            , Max_stops[driver], driver);
        exit(1);
    }
    for(int stop = 0; stop < Max_stops[driver]; stop++) {
        Stops[driver][stop] = get_int();
    }
}

void init_gossips() {
    for(int driver = 0; driver < Max_drivers; driver++) {
        add_gossip(&Gossips[driver], driver);
    }
}

void init_drivers_at_stop() {
    for(int stop = 0; stop < ROUTE_LIMIT; stop++) {
        Drivers_at_stop[stop] = 0;    
    }
}

void add_driver_at_stop(int driver, int stop) {
    Drivers_at_stop[stop] |= (1 << driver);
}

int complete_gossip(gossips_set) {
    return (gossips_set == (1 << Max_drivers) - 1);
}

int all_complete() {
    for(int driver = 0; driver < Max_drivers; driver++) {
        if (!complete_gossip(Gossips[driver]))
            return 0;
    }
    return 1;
}

int locate_driver(int driver, int minute) {
    int stop_index = minute % Max_stops[driver];
    return Stops[driver][stop_index];
}

void add_drivers_at_stops(int minute) {
    init_drivers_at_stop();
    for(int driver = 0; driver < Max_drivers; driver++) {
        int stop = locate_driver(driver, minute);
        add_driver_at_stop(driver, stop);
    }
}

int is_driver_at_stop(int drivers_at_stop, int driver) {
    return drivers_at_stop & (1 << driver);
}
void update_gossips(int stop) {
    int drivers_at_this_stop = Drivers_at_stop[stop];
    int gossip_set = 0;
    /* collect all gossips at this stop */
    for(int driver = 0; driver < Max_drivers; driver++) {
          if(is_driver_at_stop(drivers_at_this_stop, driver))
                gossip_set |= Gossips[driver];
    }
    /* update all drivers gossip set */
    for(int driver = 0; driver < Max_drivers; driver++) {
          if(is_driver_at_stop(drivers_at_this_stop, driver))  
                Gossips[driver] |= gossip_set;
    }
}

void update_all_gossips() {
    for(int stop = 0; stop < ROUTE_LIMIT; stop++) {
        update_gossips(stop);
    }
}
void control_print() {
    printf("There are %d drivers.\n", Max_drivers);
    for(int driver = 0; driver < Max_drivers; driver++) {
        printf("route for driver %d:\t",driver);
        for(int stop = 0; stop < Max_stops[driver]; stop++) {
            printf(" %d", Stops[driver][stop]);
        }
        printf("\n");
    }
    for(int driver = 0; driver < Max_drivers; driver++) {
        printf("gossip set for driver %d : %x\n", driver, Gossips[driver]);
    }
    printf("all gossips should be %d\n", (1 << Max_drivers) - 1);
    printf("driver 0 at minute 5 is on %d\n", locate_driver(0, 5));
    int minute;
    for(minute=0; minute < 480 && ! all_complete(); minute++) {
        add_drivers_at_stops(minute);
        update_all_gossips();
        for(int driver = 0; driver < Max_drivers; driver++) {
            printf("gossips of driver %d at minute %d :%d\n", driver, minute, Gossips[driver]);
        }
    }
    if(minute < 480) {
        printf("complete at minute: %d\n", minute);
    }
    else {
        printf("never\n");
    }
}

int main() {
    get_max_drivers();
    for(int driver = 0; driver < Max_drivers; driver++) 
        get_route(driver);

    init_gossips();
    control_print();
    exit(0);
}
